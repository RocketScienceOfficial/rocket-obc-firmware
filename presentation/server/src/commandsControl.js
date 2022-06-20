const webSocketControl = require("./webSocketControl");
const serialPortControl = require("./serialPortControl");

function sendRPCToSatellite(data) {
    serialPortControl.write(data + '\r', function () { });
}

function runRPCFromSatellite(data) {
    if (data.level == "CLIENT") {
        sendRPCToWebSocket(data);
    }
    else if (data.level == "SERVER") {
        runServerRPC(data);
    }
}

function runServerRPC(data) {

}

function sendRPCToWebSocket(data) {
    webSocketControl.sendToClients(JSON.stringify(data));
}

function runRPCFromWebSocket(data) {

}

module.exports.sendRPCToSatellite = sendRPCToSatellite;
module.exports.runRPCFromSatellite = runRPCFromSatellite;
module.exports.sendRPCToWebSocket = sendRPCToWebSocket;
module.exports.runRPCFromWebSocket = runRPCFromWebSocket;