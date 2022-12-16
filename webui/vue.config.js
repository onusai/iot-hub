const { defineConfig } = require("@vue/cli-service");
require("dotenv").config({ path: "../.env" });

module.exports = defineConfig({
  transpileDependencies: true,
  devServer: {
    proxy: {
      "^/api": {
        target: `http://${process.env.SERVER_ADDRESS}:${process.env.LISTEN_PORT}`,
      },
      "^/socket.io": {
        target: `http://${process.env.SERVER_ADDRESS}:${process.env.LISTEN_PORT}`,
      },
    },
  },
});
