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
});

module.exports = mongoose.model("DeviceCollection", DeviceCollectionModel);
