const express = require("express");
const http = require("http");
const config = require("./config.json");
const indexRouter = require("./routes/index");
const serialPortControl = require("./src/serialPortControl");
const webSocketControl = require("./src/webSocketControl");
const commandsControl = require("./src/commandsControl");

const app = express();
const server = http.createServer(app);

webSocketControl.init(server, handleWebSocketClientConnected, handleWebSocketListen);
serialPortControl.init(handleSerialPortConnected, handleSerialPortListen, handleSerialPortClose, handleSerialPortInitFailed);

app.use(express.static("public"));
app.use("/css", express.static(__dirname + "public/css"));
app.use("/img", express.static(__dirname + "public/img"));
app.use("/js", express.static(__dirname + "public/js"));
app.set("views", "./views");
app.set("view engine", "ejs");
app.use("/", indexRouter);

server.listen(config.port, () => console.log(`Listening on port ${config.port}`));

function handleWebSocketClientConnected(){
}

function handleWebSocketListen(data) {
    commandsControl.sendRPCToSatellite(data);
}

function handleSerialPortConnected(path) {
}

function handleSerialPortListen(data) {
    try {
        if (data.startsWith(config.dataPrefix)) {
            data = data.substring(config.dataPrefix.length);
            data = data.replaceAll('\\', '/');

            const parsedData = JSON.parse(data);

            if (parsedData.type == "REMOTE_COMMAND") {
                commandsControl.runRPCFromSatellite(parsedData);
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
            serialPortControl.getPortPath(path => {
                if (path != serialPortControl.UNKNOWN_PATH) {
                    console.log("Found serial port: " + path);

                    serialPortControl.listen(path, handleSerialPortListen, handleSerialPortClose);

                    clearInterval(serialPortInterval);
                }
            });
        }

    }, 500);
}