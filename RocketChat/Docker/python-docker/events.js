
const eventSource = new EventSource("/events");

eventSource.onmessage = function (event) {
    const payloadDiv = document.getElementById("payload");
    const data = JSON.parse(event.data);
    payloadDiv.textContent = JSON.stringify(data, null, 2);
};

eventSource.onerror = function () {
    console.error("Connection lost. Trying to reconnect...");
};
