const url = "ws://localhost:8080";
const socket = new WebSocket(url);
const callbacks = [];

function sendMessage(msg) {
    console.log("Send message: " + msg);

    socket.send(msg);
}

function onMessage(callback) {
    callbacks.push(callback);
}

socket.addEventListener("open", function (event) {
    console.log("Connected to server");
});

socket.addEventListener("message", function (event) {
    console.log("Received message from server: " + event.data);

    callbacks.forEach(callback => {
        callback(event.data);
    });
});