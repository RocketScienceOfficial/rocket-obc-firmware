const { SerialPort } = require("serialport");

const UNKNOWN_PATH = "UNKNOWN";
const VENDOR_ID = "2E8A";
const PRODUCT_ID = "000A";

var port;
var onConnected;

function isConnected() {
    return port != null && port != undefined;
}

function init(onConnect, onRead, onClose, onInitFailed, index = 0) {
    onConnected = onConnect;

    getPortPath(path => {
        if (path != UNKNOWN_PATH) {
            listen(path, onRead, onClose);
        }
        else {
            console.error("Could not find serial port");

            onInitFailed();
        }
    }, index);
}

function listen(path, onRead, onClose) {
    console.log("Listening for serial port: " + path);

    port = new SerialPort({
        path: path,
        baudRate: 115200,
    });

    port.on("close", function () {
        console.log("Serial port: " + path + " closed");

        onClose();

        port = null;
    });

    port.on('data', function (data) {
        const splittedData = data.toString().split('\n');
        splittedData.pop();

        splittedData.forEach(d => {
            console.log("Received message from serial port: " + d);

            onRead(d);
        });
    });

    onConnected(path);
}

function write(data, callback) {
    if (port != undefined) {
        port.write(data, function (err) {
            if (err) {
                console.error("Error writing to serial port: " + err);
            }
            else {
                console.log("Written to serial port: " + data);

                callback();
            }
        });
    }
    else {
        console.error("Port is not initialized");
    }
}

function getPortPath(callback, index = 0) {
    SerialPort.list().then(ports => {
        let count = 0;
        
        for (let i = 0; i < ports.length; i++) {
            const vendor = ports[i].vendorId.toUpperCase();
            const product = ports[i].productId.toUpperCase();

            if (vendor == VENDOR_ID && product == PRODUCT_ID) {
                if (index == count) {
                    callback(ports[i].path);

                    return;
                }

                count++;
            }
        }

        callback(UNKNOWN_PATH);
    });
}

module.exports.isConnected = isConnected;
module.exports.init = init;
module.exports.listen = listen;
module.exports.write = write;
module.exports.getPortPath = getPortPath;
module.exports.UNKNOWN_PATH = UNKNOWN_PATH;