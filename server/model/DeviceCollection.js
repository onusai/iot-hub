const mongoose = require("mongoose");
var Schema = mongoose.Schema;

var DeviceCollectionModel = new Schema({
  name: {
    type: String,
    required: true,
    //    unique: true,
  },
  owner: {
    type: Schema.Types.ObjectId,
    ref: "user",
    //    unique: true,
  },
  readme: {
    type: String,
    required: false,
    default: "",
  },
});

module.exports = mongoose.model("DeviceCollection", DeviceCollectionModel);
