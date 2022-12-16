const express = require("express");
const router = express.Router();
const UserModel = require("../model/User");
const CollectionModel = require("../model/DeviceCollection");
const DeviceModel = require("../model/Device");

router.get("/user", (req, res, next) => {
  res.json({
    message: "User Data",
    user: req.user,
    token: req.query.secret_token,
  });
});

router.post("/collection/create", async (req, res, next) => {
  try {
    const collection = await CollectionModel.create({
      name: req.body.name,
      owner: req.user._id,
    });
    res.json(collection);
  } catch (error) {
    if (error.code == 11000)
      res.status(400).send("Collection with that name already exists");
    else {
      res.status(400).send("Internal Error: Unable to create collection");
    }
  }
});

router.get("/collection/list", async (req, res, next) => {
  const collections = await CollectionModel.find({ owner: req.user._id });
  res.json(collections);
});

router.post("/device/create", async (req, res, next) => {
  try {
    const device = await DeviceModel.create({
      name: req.body.name,
      deviceCollection: req.body.deviceCollection,
      template: req.body.template,
    });
    res.json(device);
  } catch (error) {
    if (error.code == 11000)
      res.status(400).send("Device with that name already exists");
    else {
      res.status(400).send("Internal Error: Unable to create device");
    }
  }
});

router.post("/device/list", async (req, res, next) => {
  const device = await DeviceModel.find({
    deviceCollection: req.body.deviceCollection,
  });
  res.json(device);
});

router.post("/device/connectionstring", async (req, res, next) => {
  const device = await DeviceModel.find({
    _id: req.body._id,
  });

  let str = {
    a: `${process.env.SERVER_ADDRESS}:${process.env.LISTEN_PORT}`,
    c: req.body.deviceCollection,
    d: req.body._id,
  };
  str = JSON.stringify(str);
  buf = Buffer.from(str).toString("base64");
  str_len = str.length.toString().padStart(3, "0");
  res.json({ buf: str_len + buf });
});

router.post("/device/save", async (req, res, next) => {
  let device = await DeviceModel.findOne({
    _id: req.body._id,
  });

  if (!device) {
    return res
      .status(404)
      .json({ message: "Error: Device ID not found." });
  }

  device.name = req.body.name;
  device.description = req.body.description;
  device.template = req.body.template;
  await device.save();

  return res.status(200).send();
});

router.post("/collection/save", async (req, res, next) => {
  let collection = await CollectionModel.findOne({
    _id: req.body._id,
  });

  if (!collection) {
    return res
      .status(404)
      .json({ message: "Error: Collection ID not found." });
  }

  collection.name = req.body.name;
  collection.readme = req.body.readme;
  await collection.save();

  return res.status(200).send();
});

router.post("/collection/delete", async (req, res, next) => {
  await DeviceModel.deleteMany({
    deviceCollection: req.body._id,
  });

  await CollectionModel.deleteOne({
    _id: req.body._id,
  });

  return res.status(200).send();
});

router.post("/device/delete", async (req, res, next) => {
  await DeviceModel.deleteOne({
    _id: req.body._id,
  });

  return res.status(200).send();
});


router.post("/device/deregister", async (req, res, next) => {
  let device = await DeviceModel.findOne({
    _id: req.body._id,
  });

  if (!device) {
    return res
      .status(404)
      .json({ message: "Error: Device ID not found." });
  }

  device.isRegistered = false;
  await device.save();

  return res.status(200).send();
});

module.exports = router;
