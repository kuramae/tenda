//#include <SI114X.h>

#define SPEEDA D2
#define SPEEDAV V2
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

//SI114X SI1145 = SI114X();
const int initial_value_speed = PWMRANGE;

void setup_light_pins() {
  pinMode(STANDBY, OUTPUT);
  pinMode(SPEEDA, OUTPUT);
  pinMode(UPA, OUTPUT);
  pinMode(DOWNA, OUTPUT);
  pinMode(SPEEDB, OUTPUT);
  pinMode(UPB, OUTPUT);
  pinMode(DOWNB, OUTPUT);
  debug("\nPins are set");
}

void initialize_values() {
  digitalWrite(STANDBY, 1);
  analogWrite(SPEEDA, initial_value_speed);
  analogWrite(SPEEDB, initial_value_speed);
  debug("\nValues initialized");
}

bool tendaUpA(int s) {
   digitalWrite(SPEEDA,s); 
   digitalWrite(UPA,HIGH); 
   digitalWrite(DOWNA,LOW);
}

bool tendaUpB(int s) {
   digitalWrite(SPEEDB,s); 
   digitalWrite(UPB,HIGH); 
   digitalWrite(DOWNB,LOW);
}

bool tendaDownA(int s) {
   digitalWrite(SPEEDA,s); 
   digitalWrite(UPA,LOW); 
   digitalWrite(DOWNA,HIGH);
}

bool tendaDownB(int s) {
   digitalWrite(SPEEDB,s); 
   digitalWrite(UPB,LOW); 
   digitalWrite(DOWNB,HIGH);
}

void stopAll() {
  digitalWrite(SPEEDA,0); 
  digitalWrite(SPEEDB,0); 
}


bool fineCorsaUp() {
  return digitalRead(4) != LOW;
}

bool fineCorsaDown() {
  return digitalRead(5) != LOW;
}

bool directionUp(int luce, int target) {
    return luce < target;
}

bool moving(int luce, int target) {
    return abs(luce - target) > MARGIN;
}

bool speed(int luce, int target) {
  int diff = abs(luce - target);
    if (diff < 255) {
      return diff;
    } else {
      return 255;
    }
}


int fotoInt() {
//  SI1145.ReadVisible();
}

int fotoEst() {
  // Currently not supported
  return -1;
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

