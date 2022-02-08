
#include "LIB_sensoren.h"
#include "wire.h"
#define DELAY 500

// Sensors

// minT(C), maxT(C), minHum(%), maxHum(%)
TemperatureSensor temperatureSensor(20, 25, 40, 70);
// minLux(lx), maxLux(lx)
LightSensor lightSensor(300, 700);
//minCO2(ppm), maxCO2(ppm), minVOC(ppm), maxVOC(ppm)
AirSensor airSensor(400, 900, 0, 100);


// logging to serial
long count = 0;

bool SDKaart = false;

void setup() {

  // Seriële output
  
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Serial monitor test");
  
 if (temperatureSensor.Start())
    Serial.println("Temperature sensor ok!");
  else
    Serial.println("Failed to start temperature sensor!");

      delay(DELAY);

      

  if (lightSensor.Start())
    Serial.println("Light sensor ok!");
  else 
    Serial.println("Failed to start Light sensor!");

      delay(DELAY); 

      
    
  if (airSensor.Start())
    Serial.println("Air sensor ok!");
  else
    Serial.println("Failed to start Air sensor!");

}

void loop() {

  // Temperature and humidity

    temperatureSensor.Read();
    temperatureSensor.PrintSerial();
    Serial.print(" (°C,%)");  
    Serial.print(",   ");

   // Light

    lightSensor.Read();
    lightSensor.PrintSerial();
    Serial.print(" (lx)");
    Serial.print(",   ");



  // Air
  
    airSensor.Read();
    airSensor.PrintSerial();
    Serial.print(",");
    Serial.print(" (ppm, VOC)");
    Serial.println();

  delay(1000);
}
