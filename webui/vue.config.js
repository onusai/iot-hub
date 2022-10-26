const { defineConfig } = require("@vue/cli-service");
require("dotenv").config({ path: "../.env" });

const PORT =
  process.env.NODE_ENV == "production"
    ? process.env.PROD_PORT
    : process.env.DEV_PORT;

module.exports = defineConfig({
  transpileDependencies: true,
  devServer: {
    proxy: {
      "^/api": {
        target: `http://localhost:${PORT}`,
      },
    },
  },
});
