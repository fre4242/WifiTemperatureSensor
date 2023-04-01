# WifiTemperatureSensor
This project provides a wemos d1 mini (esp8266) based temperature sensor with a simple web page. 
It's based on esp8266 connected to a HTU21D temperature sensor. The sensor is connected to the ESP8266 via I2C. The ESP8266 provides a simple web page to display the temperature and humidity. The configuration is stored in the ESP8266's flash memory.
The device does not have a display. It provides a web page which is intended to be used on a mobile device to display the temperature and humidity as large as possible.
The web page also shows an indicator for the temperature gradient which is intended to be used to show if the temperature is increasing or decreasing. It is not intended to show the exact temperature gradient.
The device could be powered by a 5V USB power supply. The ESP8266 is powered by the 5V USB power supply and the HTU21D is powered by the 3.3V output of the ESP8266.
In my environment it is used for a temperature monitoring of a garden sauna with a wood stove. The temperature is displayed on a mobile phone and allows to wait for a good temperature before entering the sauna room.

## Hardware
* Wemos D1 Mini (ESP8266)
* HTU21D
* The wiring is as follows:
    * D1 (GPIO5) -> SCL
    * D2 (GPIO4) -> SDA
    * 3.3V -> VCC
    * GND -> GND
    * For an example look here: http://www.esp8266learning.com/esp8266-sht21-humidity-temperature-sensor-example.php

It uses the following libraries:
    * [iotWebConf](https://registry.platformio.org/libraries/prampec/IotWebConf?version=2.3.3) for configuration
    * [HTU21D](https://registry.platformio.org/libraries/enjoyneering/HTU21D) for the temperature sensor
    * see also here https://github.com/enjoyneering/HTU21D

## Configuration
The configuration is done via the web page. As many esp8266 projects do the device initially provides an access point with the name "WifiTemperatureSensor". Connect to this access point and open the web page. The web page will show the configuration options. The configuration options are:
* SSID: The SSID of the WiFi network to connect to
* Password: The password of the WiFi network to connect to
* Hostname: The hostname of the device. This is used to access the web page. Th default is "WifiTemperatureSensor"
* Temperature calibration: This value is used to calibrate the temperature sensor. The default is 0. If the temperature is too high set a negative value. If the temperature is too low set a positive value.


## Usage
After the configuration is done the device will connect to the WiFi network and the web page will be available. The web page is available at http://<hostname>/. The hostname is the hostname configured in the configuration. The default is "WifiTemperatureSensor".The web page shows the temperature and humidity. The temperature is shown in °C. The humidity is shown in %. 
The temperature is shown in a color which indicates good for sauna usage depending upon the gradient.  

You may all http://<<<hostname>>>/json to get the temperature and humidity in JSON format. This is intended to be used by other applications. The JSON format is:
```
{
  "temperature": 23.5,
  "humidity": 45.2
}
```
To get the initial setup page again you may call http://<hostname>/setup.

## Example image

<img src="https://github.com/fre4242/WifiTemperatureSensor/blob/main/WifiTemperatureSensorSample.png" width="300">

## Build
The project is build with [PlatformIO](https://platformio.org/). The project is configured to build for the Wemos D1 Mini. To build the project you may open it in Visual Studio Code and use the PlatformIO extension. 
The content of the web page (index.html and favicon.ico) is not embedded into the binary. You need to flash the web page content in the data folder to the wemos d1 mini by calling the platformio command "upload filesystem image". 

## Android app
There is an android app which can be used to display the temperature and humidity. The app is not available in the Google Play Store, but here in this repository.
The app is a simple webview allowing easy access to the temperature sensor without the need to remember the hostname. The app is available in the android folder of this repository. The app is not signed. You need to enable "Install from unknown sources" in the Android settings to install the app or build it yourself by using AndroidStudio.

## License
This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details
