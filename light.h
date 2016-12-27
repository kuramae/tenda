#include <SI114X.h>

#define SPEEDA D5
#define SPEEDAV V5
#define UPA D3
#define UPAV V3
#define DOWNA D4
#define DOWNAV V4

#define SPEEDB D6
#define SPEEDBV V6
#define UPB D7
#define UPBV V7
#define DOWNB D8
#define DOWNBV V8

#define STANDBY D0
#define STANDBYV V0

#define FINEUP D9
#define FINEDOWN D10

#define MANUAL V51
#define TARGETLIGHT V52

int logFrequency = 10;
int logCount = 0;

SI114X SI1145 = SI114X();
const int initial_value_speed = PWMRANGE;

// Initialize variables
bool started=false;
int manual=1;
int targetLight=0;
int lastLightMillis = 0;
int lastTouchMillis = 0;

void setup_light_pins() {
  pinMode(STANDBY, OUTPUT);
  pinMode(SPEEDA, OUTPUT);
  pinMode(UPA, OUTPUT);
  pinMode(DOWNA, OUTPUT);
  pinMode(SPEEDB, OUTPUT);
  pinMode(UPB, OUTPUT);
  pinMode(DOWNB, OUTPUT);

  if (USE_FINE_CORSA) {
    pinMode(FINEUP, INPUT);
    pinMode(FINEDOWN, INPUT);
  }
  
  debug("\nPins are set");
}

void debug_lumen() {
  Serial.print("//--------------------------------------//\r\n");
  Serial.print("Vis: "); Serial.println(SI1145.ReadVisible());
  Serial.print("IR: "); Serial.println(SI1145.ReadIR());
  //the real UV value must be div 100 from the reg value , datasheet for more information.
  Serial.print("UV: ");  Serial.println((float)SI1145.ReadUV()/100);
}

void initialize_values() {
  digitalWrite(STANDBY, 1);
  analogWrite(SPEEDA, initial_value_speed);
  analogWrite(SPEEDB, initial_value_speed);
  debug("\nValues initialized");
}

void init_light_sensors() {
  while (!SI1145.Begin()) {
    Serial.print("SI1145 is not ready on "); Serial.println(SI114X_ADDR);
    delay(1000);
  }
  Serial.println("SI1145 is ready!");
}

int fotoInt() {
  return SI1145.ReadVisible();
}

int fotoEst() {
  // Currently not supported
  return -1;
}


bool tendaUpA(int s) {
   analogWrite(SPEEDA,s); 
   digitalWrite(UPA,HIGH); 
   digitalWrite(DOWNA,LOW);
}

bool tendaUpB(int s) {
   analogWrite(SPEEDB,s); 
   digitalWrite(UPB,HIGH); 
   digitalWrite(DOWNB,LOW);
}

bool tendaDownA(int s) {
   analogWrite(SPEEDA,s); 
   digitalWrite(UPA,LOW); 
   digitalWrite(DOWNA,HIGH);
}

bool tendaDownB(int s) {
   digitalWrite(SPEEDB,s); 
   digitalWrite(UPB,LOW); 
   digitalWrite(DOWNB,HIGH);
}

void stopAll() {
  analogWrite(SPEEDA,0); 
  analogWrite(SPEEDB,0); 
}


bool fineCorsaUp() {
  if (USE_FINE_CORSA) {
    return digitalRead(FINEUP) != LOW;
  } else {
    return false;
  }
}

bool fineCorsaDown() {
  if (USE_FINE_CORSA) {
    return digitalRead(FINEDOWN) != LOW;
  } else {
    return false;
  }
}

bool directionUp(int luce, int target) {
    return luce < target;
}

bool moving(int luce, int target) {
    return abs(luce - target) > MARGIN;
}

int speed(int luce, int target) {
  return initial_value_speed;
//  int diff = abs(luce - target);
//    if (diff < PWMRANGE) {
//      return diff;
//    } else {
//      return PWMRANGE;
//    }
}

void light_logic() {
  if (!manual) {    
    int internalLight = fotoInt();
    bool fineCorsaU = fineCorsaUp();
    bool fineCorsaD = fineCorsaDown();

    if (logCount > logFrequency) {
      terminal.print(F("Internal light:")); terminal.println(internalLight); 
      terminal.print(F("Limit up reached:")); terminal.println(fineCorsaU); 
      terminal.print(F("Limit down reached:")); terminal.println(fineCorsaD); 
      terminal.flush();
      logCount = 0;
    } else {
      logCount++;
    }
    
    if (fineCorsaUp() || fineCorsaDown()) {
      stopAll();
    } else if (started && moving(internalLight, targetLight)) {
        int speed_required = speed(internalLight, targetLight);
        terminal.print(F("Moving, target needed is ")); terminal.print(targetLight); terminal.print(F(" but light is ")); terminal.println(internalLight);
        terminal.print(F("Speed ")); terminal.println(speed_required);       
        if (directionUp(internalLight, targetLight) && !fineCorsaUp()) {
          terminal.println(F("Moving up:"));
          tendaUpA(speed_required);
          tendaUpB(speed_required);
        } else if (!fineCorsaDown()) {
          terminal.println(F("Moving down:"));
          tendaDownA(speed_required);
          tendaDownB(speed_required);
        } 
    } else {
      stopAll();
    }
  } // END !manual
  terminal.flush();
}

// STANDBY
BLYNK_WRITE(STANDBYV) {
  int pinData = param.asInt(); 
  terminal.print(F("Standby:")); terminal.println(pinData); terminal.flush();
  digitalWrite(STANDBY,pinData); 
}

// ENGINE A
BLYNK_WRITE(SPEEDAV) {
  int pinData = param.asInt(); 
  terminal.print(F("Speed A:")); terminal.println(pinData); terminal.flush();
  analogWrite(SPEEDA,pinData); 
}

BLYNK_WRITE(UPAV) {
  int pinData = param.asInt(); 
  terminal.print(F("Up A:")); terminal.println(pinData); terminal.flush();
  digitalWrite(UPA,pinData); 
}

BLYNK_WRITE(DOWNAV) {
  int pinData = param.asInt(); 
  terminal.print(F("Down A:")); terminal.println(pinData); terminal.flush();
  digitalWrite(DOWNA,pinData); 
}

// ENGINE B
BLYNK_WRITE(SPEEDBV) {
  int pinData = param.asInt(); 
  terminal.print(F("Speed B:")); terminal.println(pinData); terminal.flush();
  analogWrite(SPEEDB,pinData); 
}

BLYNK_WRITE(UPBV) {
  int pinData = param.asInt(); 
  terminal.print(F("Up B:")); terminal.println(pinData); terminal.flush();
  digitalWrite(UPB,pinData); 
}

BLYNK_WRITE(DOWNBV) {
  int pinData = param.asInt(); 
  terminal.print(F("Down B:")); terminal.println(pinData); terminal.flush();
  digitalWrite(DOWNB,pinData); 
}

// AUTOMATIC PINS
BLYNK_WRITE(TARGETLIGHT) {
  started=true;
  targetLight=param.asInt();
}

BLYNK_WRITE(MANUAL) {
  int newmanual=param.asInt();
  if (manual == 0 && newmanual == 1) {
    stopAll();
  }
  manual = newmanual;
}

