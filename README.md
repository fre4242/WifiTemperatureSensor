# WifiTempertureSensor
This project provides a wemos d1 mini (esp8266) based temperature sensor with a simple web page. 
It's based on esp8266 connected to a HTU21D temperature sensor. The sensor is connected to the ESP8266 via I2C. The ESP8266 provides a simple web page to display the temperature and humidity. The configuration is stored in the ESP8266's flash memory.
The device does not have a display. It provides a web page which is intended to be used on a mobile device to display the temperature and humidity as large as possible.
The web page also shows an indicator for the temperature gradient which is intended to be used to show if the temperature is increasing or decreasing. It is not intended to show the exact temperature gradient.
The device could be powered by a 5V USB power supply. The ESP8266 is powered by the 5V USB power supply and the HTU21D is powered by the 3.3V output of the ESP8266.
In my environment it is used for a temperature monitoring of a garden sauna with a wood stove. The temperature is displayed on a mobile phone and allows to wait for a good temperature before entering the sauna room.

## Hardware
* Wemos D1 Mini (ESP8266)
* HTU21D

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

You may all http://<hostname>/json to get the temperature and humidity in JSON format. This is intended to be used by other applications. The JSON format is:
```
{
  "temperature": 23.5,
  "humidity": 45.2
}
```
To get the initial setup page again you may call http://<hostname>/setup.

## Example image

![example](https://github.com/fre4242/WifiTemperatureSensor/image.jpg?raw=true)

## License
This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details