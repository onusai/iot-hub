
const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const io = require('./socket.js').init(server);

const port = 8080;

app.use(express.static('public'));

var iot_route = require('./routes/iot');
app.use('/api/iot', iot_route)

io.on('connection', (socket) => {
    // console.log('a user connected');
    socket.on('disconnect', () => {
        // console.log('user disconnected');
    });
});

server.listen(port, () => {
    console.log(`Web server listening on: http://localhost:${port}`)
});

