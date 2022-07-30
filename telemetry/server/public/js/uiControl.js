const commandInputField = document.getElementById("commandInputField");
const submitButton = document.getElementById("submitButton");
const messages = document.getElementById("messages");

function msgGeneration(msg) {
    const newMessage = document.createElement("h3");

    let newMsgJson = JSON.parse(msg);
    let converted = `[${newMsgJson.level} ${newMsgJson.minutes}:${newMsgJson.seconds}:${newMsgJson.miliseconds}] ${newMsgJson.message}`;

    newMessage.innerText = converted;
    messages.appendChild(newMessage);
}

onMessage(msgGeneration);

submitButton.onclick = function () {
    sendMessage(commandInputField.value);

    commandInputField.value = "";
};