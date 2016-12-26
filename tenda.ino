#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "configuration.h"
//#include "touch.h"
#include "util.h"
#include "light.h"
#include <SimpleTimer.h>

// Initialize variables
bool started=false;
int manual=1;

int targetLight=0;
int lastLightMillis = 0;
int lastTouchMillis = 0;

void setup() {
  // Set console baud rate
  Serial.begin(9600);
  while (!Serial);
  
  // Setup the touch handling
  //setup_touch();
  setup_light_pins();

  // Setup the light handling
  // setup_light();
  
  Blynk.begin(AUTH, WIFI_NAME, WIFI_PASSWORD);
  debug("\nConnecting to Blynk through WIFI: ");
  debug(WIFI_NAME);
  debug("\nBlynk v" BLYNK_VERSION ": Device started\n");
  initialize_values();
}

BLYNK_WRITE(V13) {
  started=true;
  targetLight=param.asInt();
}

BLYNK_WRITE(V10) {
  int newmanual=param.asInt();
  if (manual == 0 && newmanual == 1) {
    stopAll();
  }
  manual = newmanual;
}


void logic() {
  if (!manual) {    
    int internalLight=fotoInt();
    int fineCorsaU = digitalRead(4);
    int fineCorsaD = digitalRead(5);
    
//    if (DEBUG) BLYNK_LOG("Luce interna: %d",internalLight);
//    if (DEBUG) BLYNK_LOG("Fine corsa Up: %d",fineCorsaU);
//    if (DEBUG) BLYNK_LOG("Fine corsa Down: %d",fineCorsaD);    
 
    if (fineCorsaUp() || fineCorsaDown()) {
    stopAll();
    } else if (started && moving(internalLight, targetLight)) {
    if (directionUp(internalLight, targetLight) && !fineCorsaUp()){
        tendaUpA(speed(internalLight, targetLight));
        tendaUpB(speed(internalLight, targetLight));
    } else if (!fineCorsaDown()){
        tendaDownA(speed(internalLight, targetLight));
        tendaDownB(speed(internalLight, targetLight));
    } 
    } else {
    stopAll();
    }

  }


 
}

void loop() {
  Blynk.run();
  if (LIGHT_INTERVAL_MS < (millis()-lastLightMillis)) {
    lastLightMillis = millis();
    logic();
  }
  //if (TOUCH_INTERVAL_MS < (millis()-lastTouchMillis)) {
  //  lastTouchMillis = millis();
  //  touch_loop();
  //}
  delay(TOUCH_INTERVAL_MS);
  if (DEBUG) delay(DEBUG_EXTRA_INTERVAL_MS);
}
