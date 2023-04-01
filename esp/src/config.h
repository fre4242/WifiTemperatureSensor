#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

extern const char *VERSION;

// Modifying the config version will probably cause a loss of the existig configuration.
// Be careful!
extern const char *CONFIG_VERSION;

extern const char *WIFI_AP_SSID;
extern const char *WIFI_AP_DEFAULT_PASSWORD;

#endif