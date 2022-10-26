const express = require("express");
const router = express.Router();
const DeviceModel = require("../model/Device");
const UserModel = require("../model/User");

router.post("/register", async (req, res, next) => {
  let device = await DeviceModel.findOne({
    _id: req.body.deviceID,
  });

  if (!device) {
    return res
      .status(404)
      .json({ message: "Unable to register, DeviceID not found." });
  }

  device.lastIP = req.connection.remoteAddress;
  if (!device.isRegistered) device.isRegistered = true;
  await device.save();

  return res.status(200).json({
    MQTT_ADDRESS: process.env.MQTT_ADDRESS,
    MQTT_PORT: process.env.MQTT_PORT,
  });
});

module.exports = router;
