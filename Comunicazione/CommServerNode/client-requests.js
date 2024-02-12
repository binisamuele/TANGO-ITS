const { arduinoHost, arduinoPort } = require('./costants.js');
const http = require('http');

//
// forward direction to Arduino
//

forwardToArduino = (direction, lastDirection) => {
    if (!isValidDirection(direction)){
        console.log('Invalid direction. Not forwarding to Arduino.');
        return;
    }

    if (direction === lastDirection && direction != "emergencyStop") {
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
        path: '', 
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
            'Content-Length': JSON.stringify(jsonData).length
        }
    };

    try {
        const req = http.request(options, (res) => {
            console.log(`Arduino server responded with status code: ${res.statusCode}`);
        });

        req.on('error', (error) => {
            comStatus = false;
            if (error.code === 'EHOSTUNREACH') {
                console.error('>> Error sending request to Arduino: Arduino is unreachable.');
            } else {
                console.error('An error occurred:', error);
            }
        });

        req.write(JSON.stringify(jsonData));
        req.end();
    }
    catch(error){
        console.error('An error occurred:', error);
    }
    
    return lastDirection;
};

//
// valid directions for Arduino
//
isValidDirection = (direction) => {
    const validDirections = ['up', 'down', 'left', 'right', "released", "emergencyStop"];
    return validDirections.includes(direction);
};

//
// periodic connection checks between node and Arduino 
//
periodicCheck = (value) => {
    let comStatus = true;

    const jsonData = {
        arduinoCheck: value
    };
    
    const options = {
        hostname: arduinoHost,
        port: arduinoPort,
        path: '', 
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
            'Content-Length': JSON.stringify(jsonData).length
        }
    };
    
    try {
        const req = http.request(options, (res) => {
            console.log(`Arduino server responded with status code: ${res.statusCode}`);
        });
        
        req.on('error', (error) => {
            comStatus = false;
            if (error.code === 'EHOSTUNREACH') {
                console.error('>> Error sending request to Arduino: Arduino is unreachable.');
            } else {
                console.error('An error occurred:', error);
            }
        });
        
        req.write(JSON.stringify(jsonData));
        req.end();
    }
    catch(error){
        console.error('An error occurred:', error);
    }

    return comStatus;
}

module.exports = {
    forwardToArduino,
    periodicCheck
}
    