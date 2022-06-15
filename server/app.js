const express = require("express");
const http = require("http");
const website = require("./src/website/index");
const serialPortControl = require("./src/serial/serialPortControl");
const api = require("./src/api/index");
const webSocketControl = require("./src/rtc/webSocketControl");

const PORT = 8080;
const DATA_PREFIX = "DATA:";

const app = express();
const server = http.createServer(app);

webSocketControl.init(server, handleWebSocketListen);
serialPortControl.init(handleSerialPortConnected, handleSerialPortListen, handleSerialPortClose, handleSerialPortInitFailed);

app.use(express.static("public"));
app.use("/css", express.static(__dirname + "public/css"));
app.use("/img", express.static(__dirname + "public/img"));
app.use("/js", express.static(__dirname + "public/js"));

app.set("views", "./views");
app.set("view engine", "ejs");

app.use("/", website);
app.use("/api", api);

server.listen(PORT, () => console.log(`Listening on port ${PORT}`));

function handleWebSocketListen(data) {
    serialPortControl.write(data + '\r', function () { });
}

function handleSerialPortConnected(path) {

}

function handleSerialPortListen(data) {
    try {
        if (data.startsWith(DATA_PREFIX)) {
            data = data.substring(DATA_PREFIX.length);
            data = data.replaceAll('\\', '/');

            const parsedData = JSON.parse(data);

            if (parsedData.type == "REMOTE_COMMAND") {
                if (parsedData.level == "CLIENT") {
                    webSocketControl.sendToClients(data);
                }
                else if (parsedData.level == "SERVER") {

                }
            }
            else {
                webSocketControl.sendToClients(data);
            }
        }
    } catch (e) {
        console.error(e);
    }
}

function handleSerialPortClose() {
    findSerialPort();
}

function handleSerialPortInitFailed() {
    findSerialPort();
}

function findSerialPort() {
    const serialPortInterval = setInterval(() => {
        if (!serialPortControl.isConnected()) {
            console.log("Looking for serial port...");

            serialPortControl.getPortPath(path => {
                if (path != serialPortControl.UNKNOWN_PATH) {
                    console.log("Found serial port: " + path);

                    serialPortControl.listen(path, handleSerialPortListen, handleSerialPortClose);

                    clearInterval(serialPortInterval);
                }
                else {
                    console.log("Couldn't find serial port");
                }
            });
        }

    }, 500);
}