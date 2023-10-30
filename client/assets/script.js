function toggleModal() {
    document
        .querySelector("#connection-dialog")
        .classList.toggle("visually-hidden");
    console.log("toggled ! by");
}

function sendMessage(message) {
    console.log("message", message.value);
    message.value = "";
}

function receiveMessage(content, author = "") {
    const messageList = document.querySelector("#messageHistory");
    const newMessage = document.createElement("div");

    const newAuthor = document.createElement("div");
    newAuthor.classList = "author";
    newAuthor.innerText = author;
    newMessage.appendChild(newAuthor);

    const newTime = document.createElement("div");
    newTime.classList = "time";
    newTime.innerText = new Date().toLocaleTimeString();
    newMessage.appendChild(newTime);

    const newContent = document.createElement("div");
    newContent.classList = "content";
    newContent.innerText = content;
    newMessage.appendChild(newContent);

    messageList.appendChild(newMessage);
    window.scrollTo(0, document.body.scrollHeight);
}
