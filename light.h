void setup_light() {
  while (!SI1145.Begin()) {
    debug("Si1145 is not ready!");
    delay(1000);
  }
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
}

bool tendaUpA(int s) {
   digitalWrite(6,s); 
   digitalWrite(7,HIGH); 
   digitalWrite(8,LOW);
}

bool tendaUpB(int s) {
   digitalWrite(11,s); 
   digitalWrite(12,HIGH); 
   digitalWrite(13,LOW);
}

bool tendaDownA(int s) {
   digitalWrite(6,s); 
   digitalWrite(7,LOW); 
   digitalWrite(8,HIGH);
}

bool tendaDownB(int s) {
   digitalWrite(11,s); 
   digitalWrite(12,LOW); 
   digitalWrite(13,HIGH);
}

void stopAll() {
  digitalWrite(11,0); 
  digitalWrite(6,0); 
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
