//#include <SI114X.h>

#define SPEEDA D4
#define UPA D3
#define DOWNA D2

#define SPEEDB D6
#define UPB D7
#define DOWNB D8

#define STANDBY D0

//SI114X SI1145 = SI114X();
const int initial_value_speed = PWMRANGE;
void setup_light_pins() {
  pinMode(D0, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
}

void setup_light() {
  //while (!SI1145.Begin()) {
  //  debug("SI1145 is not ready!");
  //  delay(500);
  //}
  pinMode(SPEEDA, OUTPUT);
  pinMode(UPA, OUTPUT);
  pinMode(DOWNA, OUTPUT);
  pinMode(SPEEDB, OUTPUT);
  pinMode(UPB, OUTPUT);
  pinMode(DOWNB, OUTPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  debug("Done configuring SI1145");
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


BLYNK_WRITE(V0) {
  int pinData = param.asInt(); 
  Serial.print("D0:"); Serial.println(pinData);
  digitalWrite(D0,pinData); 
}

BLYNK_WRITE(V2) {
  int pinData = param.asInt(); 
  Serial.print("D2:"); Serial.println(pinData);
  analogWrite(D2,pinData); 
}

BLYNK_WRITE(V3) {
  int pinData = param.asInt(); 
  Serial.print("D3:"); Serial.println(pinData);
  digitalWrite(D3,pinData); 
}

BLYNK_WRITE(V4) {
  int pinData = param.asInt(); 
  Serial.print("D4:"); Serial.println(pinData);
  digitalWrite(D4,pinData); 
}

