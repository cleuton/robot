#include <Wire.h>
#include <LSM303.h>

LSM303 compass;

void setup() {
  Serial.begin(9600);
  Serial.print("Begin");
  Wire.begin();
  Serial.print("B1");
  compass.init();
  Serial.print("B2");
  compass.enableDefault();
  Serial.print("B3");
  compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
  Serial.print("ready");
}

void loop() {
  
  compass.read();
  
  float heading = compass.heading();
  
  Serial.println(heading);
  delay(100);
}
