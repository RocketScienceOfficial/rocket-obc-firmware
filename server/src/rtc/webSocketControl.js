const WebSocket = require("ws");

var webSocketServer;

function init(server, onReceive) {
    webSocketServer = new WebSocket.Server({ server });

    webSocketServer.on("connection", function (ws) {
        console.log("A Client Connected");

        ws.on("message", function (data) {
            console.log("Received message from client: " + data);
            
            onReceive(data);
        });
    });
}

function sendToClients(data) {
    console.log("Sending message to clients: " + data);

    webSocketServer.clients.forEach(function each(client) {
        if (client.readyState === WebSocket.OPEN) {
            client.send(data);
        }
    });
}

module.exports.init = init;
module.exports.sendToClients = sendToClients;