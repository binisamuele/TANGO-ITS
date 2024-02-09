const { arduinoHost, arduinoPort } = require('./costants.js');
const http = require('http');


forwardToArduino = (direction, lastDirection) => {
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
        path: '', 
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
            'Content-Length': JSON.stringify(jsonData).length
        }
    };

    const req = http.request(options, (res) => {
        console.log(`Arduino server responded with status code: ${res.statusCode}`);
    });

    req.on('error', (error) => {
        console.error('Error sending request to Arduino:', error);
    });

    req.write(JSON.stringify(jsonData));
    req.end();
    
    return lastDirection;
};

isValidDirection = (direction) => {
    const validDirections = ['up', 'down', 'left', 'right', "released", "emergencyStop"];
    return validDirections.includes(direction);
};


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
    
    const req = http.request(options, (res) => {
        console.log(`Arduino server responded with status code: ${res.statusCode}`);
    });
    
    req.on('error', (error) => {
        comStatus = false;
        console.error('Error sending request to Arduino:', error);
    });
    
    req.write(JSON.stringify(jsonData));
    req.end();

    return comStatus;
}

module.exports = {
    forwardToArduino,
    periodicCheck
}
    