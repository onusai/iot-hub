<template>
    <div class="device-register">
        <h1>Register IoT Device</h1>
        <hr>

        <ol>
            <li>Use a mobile device (smartphone / tablet) to scan the QR Code below
                <ul>
                    <li>You can use any QR code scanner. You may need to download one from the app store</li>
                    <li>If you have this page open on a mobile device, you can just copy the text code below the QR code</li>
                </ul>
            </li>
            <div class="qrcode"><VueQRCodeComponent :size="200" bg-color="#fff" class="qrcode" :text="connectionString" v-if="connectionString" /></div>
            <p class="cstring"><span v-if="connectionString">{{connectionString}}</span></p>
            <li>Copy the code</li>
            <li>Power on your IoT Device</li>
            <li>On your mobile device, open WiFi settings and look for a WiFi named "Setup IoT Device"</li>
            <li>Connect to the WiFi named "Setup IoT Device". Once you are connected you will be prompted with a web page
                <ul>
                    <li>If you connected to the WiFi but are not prompted with a webpage
                        <ul>
                            <li>Try pressing the "Setup IoT Device" WiFi again to "Sign Into the Network"</li>
                            <li>If that doesnt work, turn off your mobile data, open any web browser and type this ip into the address bar: 192.168.4.1</li>
                        </ul>
                    </li>
                </ul>
            </li>
            <li>Once you open the setup web page, click "Configure WiFi Device"</li>
            <img class="register-img" src="@/assets/register/1.jpg" />
            <li>You will be brought to a page with a list of nearby WiFi access points. Click on the WiFi that you want your IoT Device to connect too.
                <ul>
                    <li>In the image, my Wifi network is simply called "WiFi". Your's will likely be different.</li>
                    <li>IoT devices often dont support 5 GHz Wifi. If you don't see your WiFi network, make sure you have a 2.4 GHz WiFi</li>
                </ul>
            </li>
            <img class="register-img" src="@/assets/register/2.jpg" />
            <li>If you clicked on your WiFi name, it should automatically put it in the "SSID" input field. If not, try again or type it in there manually</li>
            <li>Fill in the password corresponding to your WiFi network</li>
            <li>Finally, paste the code you copied from the QR Code into the "Connection String" field</li>
            <img class="register-img" src="@/assets/register/3.jpg" />
            <li>Press Save</li>
            <li>It will take a moment to save the data and reboot the device
                <ul>
                    <li>If everything succeded, the device will reboot and automatically connet to the IoT Hub server</li>
                    <li>If the device reboots and nothing happens, there was probably a typo somewhere. In which case, go back to step 4 and repeat the process</li>
                </ul>
            </li>
            <li>If everything succeded, you should be able to refresh the IoT Hub web page and see that your device is now registered</li>
        </ol>

    </div>
</template>

<script>
import axios from 'axios';
import VueQRCodeComponent from 'vue-qrcode-component'

export default {
  name: "DeviceRegister",
  props: ['devData'],
  components: {
    VueQRCodeComponent
  },
  data() {
    return {
        connectionString: "",
    }
  },
  methods: {
    update() {
        this.connectionString = "";
        this.getConnectionString();
    },
    getConnectionString() {
        axios
            .post('/api/device/connectionstring', this.devData)
            .then(res => {
                if (res.data) {
                    this.connectionString = res.data.buf;
                }
            })
            .catch(err => {
                console.log(err);
                if (err.response) {
                    this.alertMsg = err.response.data;
                }
            });
    }
  },
  watch: {
    devData: function() {
        this.update();
    }
  },
  mounted() {
    this.update();
  },
};
</script>

<style scoped>
.device-register {
    text-align: left;
    padding: 10px;
    overflow-x: hidden;
}

.qrcode {
    padding-left: 100px;
    padding-top: 20px;
    padding-bottom: 15px;
    min-height: 250px;
}

.cstring {
    max-width: 400px;
    border: 2px solid #727c8e;
    padding: 10px;
    border-radius: 3px;
    background-color: #586473;;
    color: white;
    font-size: 11px;
    min-height: 50px;
}

.register-img {
    height: 400px;
    margin-left: 100px;
    margin-top: 10px;
    margin-bottom: 10px;
}
</style>