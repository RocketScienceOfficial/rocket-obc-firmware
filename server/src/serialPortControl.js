const { SerialPort } = require("serialport");

const UNKNOWN_PATH = "UNKNOWN";
const VENDOR_ID = "2E8A";
const PRODUCT_ID = "000A";

function listen(callback, index = 0) {
    getPortPath(path => {
        if (path != UNKNOWN_PATH) {
            const port = new SerialPort({
                path: path,
                baudRate: 9600,
            });

            port.on('readable', function () {
                const txt = port.read();

                callback(new Buffer.from(txt).toString())
            });
        }
        else {
            console.error("Could not find serial port");
        }
    }, index);
}

function getPortPath(callback, index = 0) {
    SerialPort.list().then(ports => {
        let count = 0;

        for (let i = 0; i < ports.length; i++) {
            if (ports[i].vendorId == VENDOR_ID && ports[i].productId == PRODUCT_ID) {
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

module.exports.listen = listen;