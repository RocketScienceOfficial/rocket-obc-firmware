const url = "ws://localhost:8080";
const socket = new WebSocket(url);

const msgButton = document.getElementById("msgButton");
const myMessages = document.getElementById("messages");

function msgGeneration(msg) {
    const newMessage = document.createElement("h3");

    newMessage.innerText = msg;
    myMessages.appendChild(newMessage);
}

function sendMessage(msg) {
    console.log("Send message: " + msg);
    
    socket.send(msg);
}

socket.addEventListener("open", function (event) {
    console.log("Connected to server");
});

socket.addEventListener("message", function (event) {
    console.log("Received message from server: " + event.data);

    msgGeneration(event.data);
});

msgButton.onclick = function () {
    sendMessage("Hello");
};