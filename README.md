# Home Security IoT WiFly

Project is for PIR detectors interfaced to a [Security Sensor Shield](http://www.freetronics.com.au/collections/shields/products/security-sensor-shield) from Freetronics and using MQTT to pass the sensor states to other systems. In addition, code is included to support the [OLED Shield](http://www.freetronics.com.au/collections/display/products/oled-shield-lcd-adapter#.V1avRuZ97eQ) from Freetronics to provide a local audio alert.

The following libraries are required to compile the code:
* MQTT (PubSubClient): https://github.com/knolleary/pubsubclient
* SDConfigFile: https://github.com/bneedhamia/sdconfigfile

Also include in the repository are [python scripts](extras/python) used to test the MQTT interface and an [OpenHAB](http://openhab.org) [configuration](extras/openhab) which provides an example of how the module could be incorporated into a home automation environment.

The following images show the OpenHAB GUI

![OpenHAB main](docs/openhab-home.png)

![OpenHAB sensors](docs/openhab-temperatures.png)

![OpenHAB airboardstatus](docs/openhab-airboardstatus.png)
