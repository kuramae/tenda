#define BLYNK_PRINT Serial 


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SI114X.h>
#include "configuration.h"
//#include "touch.h"
#include "util.h"
#include "light.h"
#include <SimpleTimer.h>

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

SimpleTimer timer; // Create a Timer object called "timer"! 
// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V10);

// Hardware Serial on Mega, Leonardo, Micro...
// #define EspSerial Serial

// Initialize variables
bool started=false;
int manual=1;

int targetLight=0;
int lastLightMillis = 0;
int lastTouchMillis = 0;

void setup() {
  // Set console baud rate
  Serial.begin(9600);
  // Setup the touch handling
  //setup_touch();

  // Setup the light handling
  // setup_light();
  Blynk.begin(AUTH, WIFI_NAME, WIFI_PASSWORD);
  BLYNK_LOG("Connecting to Blynk through WIFI: %s", WIFI_NAME);
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.flush();
  timer.setInterval(1000L, sendUptime); 
}

void sendUptime()
{
  String currentTime = String(millis());
  terminal.println(); 
  terminal.print(F("Runtime in millis "));
  terminal.println(currentTime);
  terminal.flush(); 
}

BLYNK_WRITE(V3) {
  terminal.println("Received new V1");
  terminal.println(param.asStr());
  terminal.flush();
  started=true;
  targetLight=param.asInt();
}

BLYNK_WRITE(V0) {
  terminal.println("Received new V3");
  terminal.println(param.asStr());
  terminal.flush();
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
    int internalLight=fotoInt();
    int fineCorsaU = digitalRead(4);
    int fineCorsaD = digitalRead(5);
    
    if (DEBUG) BLYNK_LOG("Luce interna: %d",internalLight);
    if (DEBUG) BLYNK_LOG("Fine corsa Up: %d",fineCorsaU);
    if (DEBUG) BLYNK_LOG("Fine corsa Down: %d",fineCorsaD);    
    
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
