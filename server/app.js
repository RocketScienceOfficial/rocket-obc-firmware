const express = require("express");
const http = require("http");
const website = require("./src/website/index");
const serialPortControl = require("./src/serial/serialPortControl");
const api = require("./src/api/index");
const webSocketControl = require("./src/rtc/webSocketControl");

const PORT = 8080;

const app = express();
const server = http.createServer(app);

function handleSerialPortListen(data) {
    console.log("Received message from serial port: " + data);

    webSocketControl.sendToClients(data);
}

function handleWebSocketListen(data) {
    console.log("Received message from client: " + data);

    serialPortControl.write(data, function () { })
}

webSocketControl.init(server, handleWebSocketListen);
serialPortControl.listen(handleSerialPortListen);

app.use(express.static("public"));
app.use("/css", express.static(__dirname + "public/css"));
app.use("/img", express.static(__dirname + "public/img"));
app.use("/js", express.static(__dirname + "public/js"));

app.set("views", "./views");
app.set("view engine", "ejs");

app.use("/", website);
app.use("/api", api);

server.listen(PORT, () => console.log(`Listening on port ${PORT}`));