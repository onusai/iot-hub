const mongoose = require("mongoose");
var Schema = mongoose.Schema;

var DeviceModel = new Schema({
  name: {
    type: String,
    required: true,
  },
  deviceCollection: {
    type: Schema.Types.ObjectId,
    ref: "DeviceCollection",
    required: true,
  },
  isRegistered: {
    type: Boolean,
    required: true,
    default: false,
  },
  description: {
    type: String,
    required: false,
    default: "",
  },
  template: {
    type: String,
    required: true,
  },
  lastIP: {
    type: String,
  },
});

module.exports = mongoose.model("Device", DeviceModel);
