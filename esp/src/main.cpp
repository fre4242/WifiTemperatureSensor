#include <string.h>
#include "config.h"
#include "debug.h"
#include <IotWebConf.h>
#include <ESP8266WiFi.h>
#include <HTU21D.h>
#include <FS.h>
HTU21D htu;
DNSServer dnsServer;
WebServer server(80);
HTTPUpdateServer httpUpdater;
WiFiClient wifiClient;
boolean validConfig = false;
#define STRING_LEN 128
#define HTU_ENABLED

char calibrationParamValue[STRING_LEN] = "0";
int tempCalibration;

char humidity[30] = "-";
char temperature[30] = "-";
float temp = 0;
float tempOld = 0;
int gradient = 0;

IotWebConf iotWebConf(WIFI_AP_SSID, &dnsServer, &server, WIFI_AP_DEFAULT_PASSWORD, CONFIG_VERSION);
IotWebConfParameter tempCalibrationParam = IotWebConfParameter("Temperature calibration", "temp calibration", calibrationParamValue, STRING_LEN);

boolean needReset = false;
boolean connected = false;

void configSaved()
{
	DEBUG("Configuration was updated.");
	needReset = true;
}

void wifiConnected()
{
	String myIP = WiFi.localIP().toString();
	DEBUG("WiFi connection established. Local Ip: %s", myIP.c_str());
	connected = true;
}

bool loadFromSPIFFS(String path) {
  String dataType = "text/html";
  DEBUG("Requested page %s", path.c_str());
  if (SPIFFS.exists(path)){
      File dataFile = SPIFFS.open(path, "r");
      if (!dataFile) {
          iotWebConf.handleNotFound();
          return false;
      }
      if (server.streamFile(dataFile, dataType) != dataFile.size()) {
        DEBUG("Sent less data than expected!");
      } else {
        DEBUG("Page served!");
      }
 
      dataFile.close();
  } else {
      DEBUG("Did not find file %s!", path.c_str());
      iotWebConf.handleNotFound();
      return false;
  }
  return true;
}

void handleRoot() {
    loadFromSPIFFS("/index.html");
}
 
void jsonConnect() {
	char buffer[120];
  	sprintf(buffer, "{\"temp\":\"%s\",\"humidity\":\"%s\",\"gradient\":\"%d\"}", temperature, humidity, gradient);
	server.send(200, "application/json", buffer);
}

void heatOn() {
	server.send(200, "text/html", "<h1>Heater: ON</h1>");
	#ifdef HTU_ENABLED
	htu.setHeater(HTU21D_ON);
	#endif
}

void heatOff() {
	server.send(200, "text/html", "<h1>Heater: OFF</h1>");
	#ifdef HTU_ENABLED
	htu.setHeater(HTU21D_OFF);
	#endif
}

void readData() {
	static int hFail = 0;
	static int tFail = 0;

	#ifdef HTU_ENABLED
	float t = htu.readTemperature();
	#else
	float t = temp == 0 ? 70 : temp;
	#endif
	if (t < 255) {
		temp = t + tempCalibration;
	  	sprintf(temperature, "%.1f", temp);
		tFail = 0;
		DEBUG("Read temp %f°C", t);
	}
	else {
		tFail++;
		if (tFail > 10) {
		  	sprintf(temperature, "---");
		}
		DEBUG("Failed to read temp #%d", tFail);
	}
	
	#ifdef HTU_ENABLED
	float h = htu.readCompensatedHumidity();
	#else
	float h = 80;
	#endif
	if (h < 255) {
	  	sprintf(humidity, "%.0f", h);
		hFail = 0;
		DEBUG("Read humidity %f%%", h);
	}
	else {
		hFail++;
		if (hFail > 10) {
		  	sprintf(humidity, "---");
		}
		DEBUG("Failed to read humidity #%d", hFail);
	}
	//htu.softReset();
}

void calcGradient() {
	#ifndef HTU_ENABLED
		if (temp > 85) {
			temp+=1;
			if (temp > 90) {
				temp = 70;
			}
		}
		else {
			temp +=5;
		}
	#endif
	if (tempOld == 0) tempOld = temp;
	int diff = temp - tempOld;
	if (diff >= 5) {
		gradient = 2;
	}
	else if (diff > 0) {
		gradient = 1;
	}
	else if (diff <= -5) {
		gradient = -2;
	}
	else if (diff < 0) {
		gradient = -1;
	}
	else {
		gradient = 0;
	}
	tempOld = temp;
}

void setup()
{
	// Setup debugging stuff
	SERIAL_DEBUG_SETUP(115200);

#ifdef DEBUG
	// Delay for getting a serial console attached in time
	delay(2000);
#endif

	if (!SPIFFS.begin())
	{
		DEBUG("Failed to load SPIFFS.");
	}
	// Setup WiFi and config stuff
	DEBUG("Setting up WiFi and config stuff.");

	iotWebConf.setConfigSavedCallback(&configSaved);
	iotWebConf.setWifiConnectionCallback(&wifiConnected);
	iotWebConf.setupUpdateServer(&httpUpdater);
    iotWebConf.addParameter(&tempCalibrationParam);

	validConfig = iotWebConf.init();
	if (!validConfig)
	{
		DEBUG("Missing or invalid config.");
		calibrationParamValue[0] = '\0';
	}

	server.on("/", [] { handleRoot(); });
	server.on("/json", [] { jsonConnect(); });
	server.on("/setup", [] { iotWebConf.handleConfig(); });
	server.on("/heat", [] { heatOn(); });
	server.on("/heatoff", [] { heatOff(); });
	server.onNotFound([]() { iotWebConf.handleNotFound(); });

   	delay(2000);
	#ifdef HTU_ENABLED
	while (!htu.begin()) {
    	DEBUG(F("HTU21D, SHT21 sensor failed or not connected. Retry after 5s ..."));
    	delay(5000);
  	}
	#else
    	DEBUG(F("HTU21D demo mode ..."));
	#endif
	tempCalibration = atoi(calibrationParamValue);
  	DEBUG(F("HTU21D, SHT21 sensor is active"));
	DEBUG("Temperatur calibration %d", tempCalibration);
}

void loop()
{
	static int timeCount = 0;
	static int gradientCount = 0;
	iotWebConf.doLoop();
	timeCount++;
	if (timeCount > 300000) { // ca. 8 Sekunden
		timeCount = 0;
		readData();
		gradientCount++;
  	//DEBUG("gradient count %d", gradientCount);
		if (gradientCount > 70) {
			calcGradient();
			gradientCount = 0;
		}
	}
	if (needReset)
	{
		// Doing a chip reset caused by config changes
		DEBUG("Rebooting after 1 second.");
		delay(1000);
		ESP.restart();
	}
	yield();
}