/*
                                --- NODE SERVER ---

>> This server recives http requests from the android app and forward them to arduino

*/

// Import modules
const express = require('express');
const { forwardToArduino, periodicCheck } = require ('./client-requests.js');
const { nodePort } = require('./costants.js');

//Misc variables
var app = express();

let lastDirection = null;

//Communication variables
let comExtableshed = false; // control if the communication has been extableshed
                            // emergency stop will be sent only if the communication has been extableshed
let isAndroidAlive = false;
let firstFail = true;

// Start server
app.use(express.json());

app.listen(nodePort, () => {
    console.log(`Server listening on port: ${nodePort}`);
    console.log('---------------------------------');
});

app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "*");
    res.contentType('application/json');
    next();
});

//
// endpoint GET for test purposes
//
app.get("/", (req, res) => {
    res.send('OK');

    console.log('Incoming GET request at /');
})

//
// endpoint POST to control arduino 
//
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
        // otherwise send an emergency stop to arduino

        if (!comExtableshed && req.body.direction !== "commStop"){
            console.log(">> Controller communication started!");
            comExtableshed = true;
        }

        if(comExtableshed && req.body.direction === "commStop"){
            console.log(">> Controller communication has been stopped!");
            isAndroidAlive = false;
            comExtableshed = false;
            periodicCheck("stop");
        }

        lastDirection = forwardToArduino(direction, lastDirection);
        
        res.send('OK');
    } catch (error) {
        console.error("Request error:", error);
        res.status(500).json({ error: 'Server Error (500)' });
    }
});

//
// endpoint POST for connection check with android
//
app.post("/connection-check", (req, res) => {
    try {
        //check if json contains key "androidCheck"
        if (req.body.hasOwnProperty("androidCheck")){
            let lastAndroidCheck = req.body.androidCheck;
            isAndroidAlive = true;
            console.log(`Android check recieved: ${lastAndroidCheck}`);
        } else{
            console.log("Invalid check recieved!");
            return;
        }

        res.send(`OK`);
    } catch (error) {
        console.error("Request error:", error);
        res.status(500).json({ error: 'Server Error (500)'});
    }
});

//
// periodically check com with android and send checks to arduino
//
setInterval(() => {
    if (comExtableshed){
        comExtableshed = periodicCheck("ok");
    }

    if (isAndroidAlive){  // connection check passed! 
        isAndroidAlive = false;
        firstFail = true;
    } else if (comExtableshed){
        if (firstFail){
            console.log(">> Error: Android not responding...");
            firstFail = false;
        }
        else{
            console.log(">> Error: stopping communication...");
            comExtableshed = false;
            isAndroidAlive = false;
            firstFail = true;
            lastDirection = "";
            //send emergency stop to arduino
            forwardToArduino("emergencyStop", lastDirection);
        }
    } 
}, 4000);
 