const io = require("./socket.js").get();
const mqttClient = require("./mqttClient").get();
const UserModel = require("../model/User");
const DeviceModel = require("../model/Device");
const DeviceCollectionModel = require("../model/DeviceCollection");
const jwt = require("jsonwebtoken");

let sockets = {};

io.on("connection", (socket) => {
  // console.log('a user connected');
  socket.on("disconnect", () => {
    delete sockets[socket.user_id];
  });

  socket.on("mqttListen", async (token) => {
    // console.log('user disconnected');
    const decoded = jwt.verify(token, process.env.JWT_SECRET);
    if (!decoded) {
      console.log("error decoding mqttListen token");
      return;
    }
    let user_id = decoded.user._id;
    socket.user_id = user_id;
    sockets[user_id] = socket;

    const user = await UserModel.findOne({ _id: user_id });
    await updateSocketRooms(user);
  });

  socket.on("updateRooms", async () => {
    const user = await UserModel.findOne({ _id: socket.user_id });
    if (user) await updateSocketRooms(user);
  });

  socket.on("mqttCMD", async (data) => {
    mqttClient.publish(`cmd/${data.topic}`, data.message);
  });
});

async function updateSocketRooms(user) {
  const collections = await DeviceCollectionModel.find({ owner: user._id });
  let rooms = collections.map((e) => String(e._id));
  sockets[user._id].join(rooms);
}

mqttClient.on("connect", () => {
  console.log(`MQTT connnected`);
  mqttClient.subscribe(["data/#"], (err) => {
    if (err) console.log(err);
    else console.log(`MQTT subscribed to data/#`);
  });
});

mqttClient.on("message", (topic, message) => {
  message = String(message);
  let [topic_type, collectionID, deviceID] = topic.split("/");
  if (topic_type == "data") {
    io.sockets
      .in(collectionID)
      .emit("mqttData", { collectionID, deviceID, message });
  }
});
