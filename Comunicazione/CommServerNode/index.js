/*
                                --- NODE SERVER ---

>> This server recive http request from the android app and send it to the arduino

*/

// Import modules
var express = require('express');

//Consts 
const port = 3000;


var app = express();
app.use(express.json());

app.listen(port, () => {
    console.log(`Server listening on port: ${port}`);
});

app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "*");
    res.contentType('application/json');
    next();
});

app.post("/control", (req, res) => {
    try {
        //const direction = req.body.direction;
        
        console.log(`>>> Direction recieved: ${req.body.direction}`);
        
        /*
        Use this to view the request coming from the client

        console.log(`Incoming ${req.method} request at ${req.url}`);
        console.log('Headers:', req.headers);
        console.log('Body:', req.body);
        */

        res.send('OK');

        //TODO: Forward the request to the arduino 
        //if the direction is not changed, do not send the request to the arduino
    } catch (error) {
        console.error("Request error:", error);
        res.status(500).json({ error: 'Server Error (500)' });
    }
});

app.get("", (req, res) => {
    try {
        console.log(`>>> GET request`);
        res.send(`Server is running`);
    } catch (error) {
        console.error("Request error:", error);
        res.status(500).json({ error: 'Server Error (500)' });
    }
});
