let io;
module.exports = {
  init: function (server) {
    // start socket.io server and cache io object
    const { Server } = require(`socket.io`);
    io = new Server(server, { cors: { origin: "*" } });
    return io;
  },
  get: function () {
    // return previously cached object
    if (!io) {
      throw new Error("must call .init(server) before you can call .getio()");
    }
    return io;
  },
};
