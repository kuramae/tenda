#include <Wire.h>
#include "Adafruit_MPR121.h"

const int ROWS = 12;
const int SIDE_PIN = 6;
const int SWIPE_SECS = 3;

const int SLIDING_INTERVAL_POINTS = 5;
const int POINTS = SWIPE_SECS * 1000 / TOUCH_INTERVAL_MS;

const int LEFT_PIN[SIDE_PIN] = {0, 1, 2, 3, 4, 5};
const int RIGHT_PIN[SIDE_PIN] = {6, 7, 8, 9, 10, 11};
const int CENTER_PIN[ROWS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

const float VARIANCE_THRESHOLD = 30;

const int NONE = -1;

// You can have up to 4 on one i2c bus
Adafruit_MPR121 cap = Adafruit_MPR121();

int points_in_array = 0;
int** pin_values = (int**)malloc(ROWS * sizeof(int*));


void debug_state() {
  debug("\nFilt: ");
  for (uint8_t i=0; i<ROWS; i++) {
    debug(cap.filteredData(i)); debug("\t");
  }
  debug("\nBase: ");
  for (uint8_t i=0; i<ROWS; i++) {
    debug(cap.baselineData(i)); debug("\t");
  }
  // Just to avoid that the debugging is too much
}

void debug_matrix() {
  debug("\n");
  for (uint8_t i=0; i<ROWS; i++) {
    for (uint8_t j=1; j<POINTS; j++) {
      debug(pin_values[i][j]);debug("\t");
    }  
    debug("\n");
  }
}

boolean initialized() {
  return points_in_array == POINTS;
}

// Highly unoptimized
void move_values() {
  for (uint8_t i=0; i<ROWS; i++) {
    for (uint8_t j=1; j<POINTS; j++) {
      pin_values[i][j-1] = pin_values[i][j];
    }  
  }
}

void save_new_values() {
  move_values();
  for (uint8_t i=0; i<ROWS; i++) {
    int value = cap.filteredData(i);
    pin_values[i][POINTS - 1] = value;
  }
  points_in_array++;
}

int find_maximum(int a[], int n) {
  int c, max, index;
 
  max = a[0];
  index = 0;
 
  for (c = 1; c < n; c++) {
    if (a[c] > max) {
       index = c;
       max = a[c];
    }
  } 
  return index;
}

int get_point_of_max_variance(const int * pin) {
  // Sliding window, for each of them we calculate the variance
  int max_variance_point = NONE;
  float max_value = 0;
  
  for (uint8_t i=0; i<POINTS - SLIDING_INTERVAL_POINTS; i++) { 
    int sum = 0;  
    for (uint8_t j=0; j<SLIDING_INTERVAL_POINTS; j++) { 
      sum += pin[i + j];
    }
    float mean = (float) sum / (float) SLIDING_INTERVAL_POINTS; 
    float variance_sum = 0;
    for (uint8_t j=0; j<SLIDING_INTERVAL_POINTS; j++) { 
      variance_sum += (float) pow(mean - pin[i + j], 2);
    }
    float variance = variance_sum / (float) SLIDING_INTERVAL_POINTS;
    if (variance > VARIANCE_THRESHOLD && variance > max_value) {
      max_value = variance; 
      max_variance_point = i;
    }
  }
  debug("\nMax variance: "); debug(max_value); 
  debug(" -  point: "); debug(max_variance_point); 
  return max_variance_point;
}

float evaluate_movement(const int * const pin, const int pin_number) {
  int t[pin_number];
  for (int i=0; i<pin_number; i++) {    
    t[i] = get_point_of_max_variance(pin_values[pin[i]]);
  }
  debug("\n");
}
  

void setup_touch_sensors() {
  // debug("Adafruit MPR121 Capacitive Touch sensor test\n"); 
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  while (!Serial) { 
    delay(10);
  }
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  debug("MPR121 found!\n");
}

void init_touch() {
  for(int i = 0; i < ROWS; i++) pin_values[i] = (int *) malloc(POINTS * sizeof(int));  
}


void touch_loop() {
  // debug_state();
  debug_matrix();
  save_new_values();
  float left_intensity = evaluate_movement(LEFT_PIN, SIDE_PIN);
  terminal.print("Left intensity: "); terminal.println(left_intensity);
  float right_intensity = evaluate_movement(RIGHT_PIN, SIDE_PIN);
  terminal.print("Right intensity: "); terminal.println(right_intensity);
  float center_intensity = evaluate_movement(CENTER_PIN, ROWS);
  terminal.print("Center intensity: "); terminal.println(center_intensity);
  // Remember to check initialized() before deciding
  terminal.flush();
}

