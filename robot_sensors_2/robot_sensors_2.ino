#include <NewPing.h>
#include <Wire.h>
#include <LSM303.h>

LSM303 compass;

// 6 - front sonar trig
// 7 - front sonar echo
// a2/16 - rear  sonar trig
// a3/17 - rear  sonar echo
// 2 - left  sonar trig
// 10 - left  sonar echo
// d4    - right sonar trig
// d5    - right sonar echo

#define front_trig 6
#define front_echo 7
#define rear_trig 16
#define rear_echo 17
#define left_trig 2
#define left_echo 10
#define right_trig 4
#define right_echo 5

#define SONAR_NUM 4      // Number of sensors.
#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(front_trig, front_echo, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(rear_trig, rear_echo, MAX_DISTANCE), 
  NewPing(left_trig, left_echo, MAX_DISTANCE),
  NewPing(right_trig, right_echo, MAX_DISTANCE)  
};

void setup() {
  Serial.begin(115200);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};  
}

void loop() {
  compass.read();
  
  float heading = compass.heading();
  Serial.print("direcao: "); 
  Serial.println(heading);  
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through each sensor and display results.
    delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    Serial.print(i);
    Serial.print("=");
    Serial.print(sonar[i].ping_cm());
    Serial.print("cm ");
  }
  Serial.println();
}
