function toggleModal() {
    document
        .querySelector("#connection-dialog")
        .classList.toggle("visually-hidden");
    console.log("toggled ! by");
}

let connected = false;

function receiveMessageFromServer() {
    // Call the native function `receiveMessage` which we have exposed to the JS context.
    // It will return a string if there is a message, `null` if there is no message,
    // or `undefined` if there was an error.

    const message = receiveMessage();

    // Check the result and handle it appropriately
    if (message === undefined) {
        console.error('An error occurred while trying to receive a message.');
        return;
    }

    if (message === null) {
        console.log('No new message received.');
        return;
    }

    // If we get here, we have a message string.
    console.log('Received message:', message);

    // Process the received message as needed
    processReceivedMessage(message);
}

// Example usage
// This could be called in response to an event, such as a button click or a loop



function processReceivedMessage(content, author = "") {
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

function handleNewConnection() {
    const address = document.querySelector('#connection-form input[name="address"]').value;
    const port = document.querySelector('#connection-form input[name="port"]').value;

    processReceivedMessage("Debug: handle new connection");

    setInterval(() => {
        receiveMessageFromServer();
    }, 1000);

    // Call the native function with the address and port
    window.formSubmit(address, port);
}