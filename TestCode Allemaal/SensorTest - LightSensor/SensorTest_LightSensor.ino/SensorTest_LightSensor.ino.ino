#include "LIB_sensoren.h"

// Sensor
// minLux(lx), maxLux(lx)
LightSensor lightSensor(300,750);

// logging to serial


void setup() {

  // Seriële output
  Serial.begin(9600);
  Serial.println("Serial monitor test");

    delay(500);

  if (lightSensor.Start()){
    Serial.println("Light sensor ok!");
  }
  else {
    Serial.println("Failed to start Light sensor!");
  }

  
  // Logging
  Serial.println("Lux (lx)");
}
void loop() {

  // light

  lightSensor.Read();
  lightSensor.PrintSerial();
  Serial.println(",");

  delay(250);

}
