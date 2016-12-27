#include <ESP8266WiFi.h>
#include <Wire.h>

#include <BlynkSimpleEsp8266.h>

WidgetTerminal terminal(V50);

#include "configuration.h"
#include "util.h"
#include "touch.h"
#include "light.h"
#include <SimpleTimer.h>


void setup() {
  // Set console baud rate
  Serial.begin(9600);
  while (!Serial);
  Wire.begin();
  // Setup the touch handling
  // setup_touch_sensors();
  // init_touch();
  
  // Setup the light handling
  setup_light_pins();
  
  Blynk.begin(AUTH, WIFI_NAME, WIFI_PASSWORD);
  debug("\nConnecting to Blynk through WIFI: ");
  debug(WIFI_NAME);
  debug("\nBlynk v" BLYNK_VERSION ": Device started\n");
  initialize_values();
  init_light_sensors();
}

void loop() {
  Blynk.run();
  if (LIGHT_INTERVAL_MS < (millis()-lastLightMillis)) {
    lastLightMillis = millis();
    light_logic();
  }
  if (TOUCH_INTERVAL_MS < (millis()-lastTouchMillis)) {
    lastTouchMillis = millis();
    //touch_loop();
  }
  delay(TOUCH_INTERVAL_MS);
  if (DEBUG) delay(DEBUG_EXTRA_INTERVAL_MS);
}
