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

//Misc variables
var app = express();

let lastDirection = null;

//Communication variables
let comExtableshed = false; // control if the communication has been extableshed
                            // emergency stop will be sent only if the communication has been extableshed
let isAndroidAlive = false;
let isArduinoAlive = false;


// Start server
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
        
        // you recieved a direction from the client. 
        // now node server expect to recieve a periodical
        // request from the client to keep the connection alive

        if (!comExtableshed && req.body.direction !== "commStop"){
            console.log(">> Controller communication started!");
            comExtableshed = true;
        }

        if(comExtableshed && req.body.direction === "commStop"){
            console.log(">> Controller communication has been stopped!");
            isAndroidAlive = false;
            isArduinoAlive = false;
            comExtableshed = false;
        } 

        forwardToArduino(direction);
        
        res.send('OK');
    } catch (error) {
        console.error("Request error:", error);
        res.status(500).json({ error: 'Server Error (500)' });
    }
});

app.post("/connection-check", (req, res) => {    //change to post
    try {
        //check if json contains key "check"
        if (req.body.hasOwnProperty("androidCheck")){
            let lastAndroidCheck = req.body.androidCheck;
            isAndroidAlive = true;
            isArduinoAlive = true;
            console.log(`Android check recieved: ${lastAndroidCheck}`);
        }
        else if (req.body.hasOwnProperty("arduinoCheck")){
            let lastArduinoCheck = req.body.arduinoCheck;
            isArduinoAlive = true;
            console.log(`Arduino check recieved: ${lastArduinoCheck}`);
        }
        else{
            console.log("Invalid check recieved!");
            return;
        }

        res.send(`OK`);
    } catch (error) {
        console.error("Request error:", error);
        res.status(500).json({ error: 'Server Error (500)'});
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
    const validDirections = ['up', 'down', 'left', 'right', "released", "emergencyStop"];
    return validDirections.includes(direction);
};


setInterval(() => {
    if (isAndroidAlive && isArduinoAlive){  //connection check passed! 
        isAndroidAlive = false;  
        isArduinoAlive = false;
    } else if (comExtableshed){
        console.log(">>Error: stopping communication...");
        comExtableshed = false;
        isAndroidAlive = false;
        isArduinoAlive = false;
        //send emergency stop to arduino
        forwardToArduino("emergencyStop");
    } 
}, 11000);
