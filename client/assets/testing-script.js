const rmModal = document.createElement("button");
rmModal.onclick = function () {
    toggleModal();
};
rmModal.innerText = "Toggle modal";
rmModal.style = "position:fixed;top:0";
document.body.appendChild(rmModal);

const mkMsg = document.createElement("button");
mkMsg.onclick = function () {
    const alphabet = [
        "a",
        "z",
        "e",
        "r",
        "t",
        "y",
        "u",
        "i",
        "o",
        "p",
        "q",
        "s",
        "d",
        "f",
        "g",
        "h",
        "j",
        "k",
        "l",
        "m",
        "w",
        "x",
        "c",
        "v",
        "b",
        "n",
        " ",
    ];
    var sentence = [];
    const rng = Math.floor(Math.random() * 2000);
    console.log(rng);
    for (let letter = 0; letter < rng; letter++) {
        sentence[letter] = alphabet[Math.floor(Math.random() * 27)];
        console.log(sentence.join(""));
    }
    receiveMessage(sentence.join(""));
};
mkMsg.innerText = "Generate message";
mkMsg.style = "position:fixed;top:0;right:0";
document.body.appendChild(mkMsg);
