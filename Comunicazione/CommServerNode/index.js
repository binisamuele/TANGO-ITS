/*
                                --- NODE SERVER ---

>> This server recives http requests from the android app and forward them to arduino

*/

// Import modules
var express = require('express');
const http = require('http');

//Consts 
const port = 3000;
const arduinoHost = '192.168.0.2';
const arduinoPort = '80'; 

//Vars
let lastDirection = null;
var app = express();


app.use(express.json());

app.listen(port, () => {
    console.log(`Server listening on port: ${port}`);
    console.log('---------------------------------');
});

app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "*");
    res.contentType('application/json');
    next();
});


app.post("/control", (req, res) => {
    try {
        const direction = req.body.direction;
        
        console.log(`>> Direction recieved: ${req.body.direction}`);
        
        /*
        Use this to view the request coming from the client
        console.log(`Incoming ${req.method} request at ${req.url}`);
        console.log('Headers:', req.headers);
        console.log('Body:', req.body);
        */
        forwardToArduino(direction);

        res.send('OK');
    } catch (error) {
        console.error("Request error:", error);
        res.status(500).json({ error: 'Server Error (500)' });
    }
});

app.get("", (req, res) => {
    try {
        console.log(`>> GET request`);
        res.send(`Server is running`);
    } catch (error) {
        console.error("Request error:", error);
        res.status(500).json({ error: 'Server Error (500)' });
    }
});


forwardToArduino = (direction) => {
    if (!isValidDirection(direction)){
        console.log('Invalid direction. Not forwarding to Arduino.');
        return;
    }

    if (direction === lastDirection) {
        console.log('Direction is the same as the previous one. Not forwarding to Arduino.');
        return;
    }

    lastDirection = direction;

    const jsonData = {
        direction: direction
    };

    const options = {
        hostname: arduinoHost,
        port: arduinoPort,
        path: '/', 
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
            'Content-Length': JSON.stringify(jsonData).length
        },
    };

    const req = http.request(options, (res) => {
        console.log(`Arduino server responded with status code: ${res.statusCode}`);
    });

    req.on('error', (error) => {
        console.error('Error sending request to Arduino:', error);
    });

    req.write(JSON.stringify(jsonData));
    req.end();
};

isValidDirection = (direction) => {
    const validDirections = ['up', 'down', 'left', 'right', "stop"];
    return validDirections.includes(direction);
};
