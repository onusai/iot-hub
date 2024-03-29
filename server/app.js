const express = require("express");
const mongoose = require("mongoose");
const passport = require("passport");
const bodyParser = require("body-parser");
const cookieParser = require("cookie-parser");
require("dotenv").config({ path: "../.env" });

const UserModel = require("./model/User");

mongoose.connect(process.env.MONGODB_CONNECTION_STRING, {
  useNewUrlParser: true,
  useUnifiedTopology: true,
});
mongoose.set("useCreateIndex", true);
mongoose.connection.on("error", (error) => console.log(error));
mongoose.Promise = global.Promise;

require("./auth/auth");

const routes = require("./routes/routes");
const secureRoute = require("./routes/secure-routes");
const deviceRoute = require("./routes/device");

const app = express();

app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.json());
//app.use(express.static("public"));

app.use("/api/auth", routes);
app.use("/api/dev", deviceRoute);
app.use("/api/", passport.authenticate("jwt", { session: false }), secureRoute);

app.use(function (err, req, res, next) {
  res.status(err.status || 500);
  res.json({ error: err });
});

const server = app.listen(process.env.LISTEN_PORT, process.env.LISTEN_ADDRESS, () => {
  console.log(`Server started on ${process.env.LISTEN_ADDRESS}:${process.env.LISTEN_PORT}`);
});

// init io and mqtt
const io = require("./tunnel/socket").init(server);
const mqttClient = require("./tunnel/mqttClient").init();
require("./tunnel/tunnel.js");