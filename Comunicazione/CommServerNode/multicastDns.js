const globals = require('./globals.js');
const mdns = require('mdns');

multicastDNS = () => {

const browser = mdns.createBrowser(mdns.udp('arduino'));

// Listen for service up events
browser.on('serviceUp', service => {
    console.log("mDNS >> Arduino found at:", service.addresses[0]);
    globals.arduinoHost = service.addresses[0];
});

// Listen for service down events
browser.on('serviceDown', service => {
    console.log("mDNS >> Arduino went down:", service.addresses[0]);
});

// Start the browser
browser.start();
}

module.exports = {
    multicastDNS
}