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
TemperatureSensor temperatureSensor(21, 24, 35, 70);

// logging to serial
long count = 0;

void setup() {

  // Seriële output
  Serial.begin(9600);

  if (temperatureSensor.Start())
    Serial.println("Temperature sensor ok!");
  else
    Serial.println("Failed to start temperature sensor!");

  // Logging
  Serial.println("Temp (C), Hum (%)");
}

void loop() {

  // Temperature and humidity

  temperatureSensor.Read();
  //temperatureSensor.PrintSerial();
  //Serial.println();

   delay(25);


  // log everty 10th measurement to serial
  if ((float)count / 10 == float(count / 10)) {
    temperatureSensor.PrintSerial();
    Serial.println();
  }
  count++;
}
