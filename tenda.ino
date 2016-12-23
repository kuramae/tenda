#define BLYNK_PRINT Serial 
#include <ESP8266_HardSer.h>
#include <BlynkSimpleShieldEsp8266_HardSer.h>
#include <SI114X.h>
#include "configuration.h"
//#include "touch.h"

void debug(const char *text) {
  if (DEBUG) { 
    Serial.print(text); 
  }
}

void debug(const float text) {
  if (DEBUG) { 
    Serial.print(text); 
  }
}



SI114X SI1145 = SI114X();

#include "light.h"

// Set ESP8266 Serial object
#define EspSerial Serial
ESP8266 wifi(EspSerial);

// Initialize variables
bool started=false;
int luceTarget=0;
int manual=1;
int lastLightMillis = 0;
int lastTouchMillis = 0;

void setup() {
  // needed to avoid starting too fast  
  while (!Serial);
  // Set console baud rate
  Serial.begin(9600);
  delay(10);

  //setup_touch();
  setup_light();
  
  // Set ESP8266 baud rate
  EspSerial.begin(115200);
  delay(10);
  BLYNK_LOG("WIFI NAME: %s",WIFI_NAME);

  Blynk.begin("c67c1b85900a4907821524e304a2e45f", wifi, "iPhone 6S Anto", "Wiiiphone5");
}

BLYNK_WRITE(V3) {
  // BLYNK_LOG("Received new V3: %s", param.asStr());
  started=true;
  luceTarget=param.asInt();
}

BLYNK_WRITE(V0) {
  // BLYNK_LOG("Received new V3: %s", param.asStr());
  int newmanual=param.asInt();
  if (manual == 0 && newmanual == 1) {
    stopAll();
  }
  manual = newmanual;
}

int fotoInt() {
  SI1145.ReadVisible();
}

int fotoEst() {
  // Currently not supported
  return -1;
}


void logic() {
  if (!manual) {    
    int luceInt=fotoInt();
    int fineCorsaU = digitalRead(4);
    int fineCorsaD = digitalRead(5);
    
    if (DEBUG) BLYNK_LOG("Luce interna: %d",luceInt);
    if (DEBUG) BLYNK_LOG("Fine corsa Up: %d",fineCorsaU);
    if (DEBUG) BLYNK_LOG("Fine corsa Down: %d",fineCorsaD);    
    
    if (fineCorsaUp() || fineCorsaDown()) {
    stopAll();
    } else if (started && moving(luceInt, luceTarget)) {
    if (directionUp(luceInt, luceTarget) && !fineCorsaUp()){
        tendaUpA(speed(luceInt, luceTarget));
        tendaUpB(speed(luceInt, luceTarget));
    } else if (!fineCorsaDown()){
        tendaDownA(speed(luceInt, luceTarget));
        tendaDownB(speed(luceInt, luceTarget));
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
