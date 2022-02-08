/* Testprogramma voor sensoren die in het Classmate project gebruikt worden.
 * 
 * Dit programma gebruikt de LIB_sensoren library om met de sensoren te
 * werken. Deze library vergt zelf nog een aantal libraries,
 * toe te voegen via Hulpmiddelen - Bibliotheken beheren:
 * 
 * SparkFun Ambient Light Sensor Arduino library (by Elias Santistevan)
 * SparkFun CS811 Arduino Library (by SparkFun electronics)
 * DHT Sensor Library (by Adafruit)
 * PMS Library (by Mariusz Kacki)
 * RTCDS1307 (by Helder Rodriuez)
 * 
 * Sensoren, RGB led en LCD scherm moeten aangesloten worden
 * zoals beschreven in de documentatie van LIB_sensoren.
 * 
 * Dit programma leest de temperatuur en de vochtigheid in. 
 * Deze waarden worden naar seriële monitor, LCD scherm geschreven. 
 * De RGB led geeft een indicatie van de status.
 * 
 * Peter Rutten
 */


#include "LIB_sensoren.h"

// Sensor
// minT(C), maxT(C), minHum(%), maxHum(%)
AirSensor airSensor(0,100,0,100);


void setup() {

  // Seriële output
  Serial.begin(9600);
  Serial.println("Serial monitor test");

  if (airSensor.Start())
    Serial.println("Air sensor ok!");
  else
    Serial.println("Failed to start Air sensor!");

  // Logging
  Serial.println("Co2(ppm), VOC(ppm)");
}

void loop() {

  // Air

  airSensor.Read();
  airSensor.PrintSerial();
  Serial.println(",");

  delay(500);

}
