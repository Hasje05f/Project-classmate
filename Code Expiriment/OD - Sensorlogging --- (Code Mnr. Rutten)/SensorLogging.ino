/* Testprogramma voor sensoren die in het Classmate project gebruikt worden.

   Dit programma gebruikt de LIB_sensoren library om met de sensoren te
   werken. Deze library vergt zelf nog een aantal libraries,
   toe te voegen via Hulpmiddelen - Bibliotheken beheren:

   SparkFun Ambient Light Sensor Arduino library (by Elias Santistevan)
   SparkFun CS811 Arduino Library (by SparkFun electronics)
   DHT Sensor Library (by Adafruit)

   Sensoren moeten aangesloten worden
   zoals beschreven in de documentatie van LIB_sensoren.

   Data logging wordt via de SD aansluiting op een Ethernet Shield gedaan
   die gemonteerd is op de Arduino.

   Data logging kan aan- en afgezet worden via een switch. Een
   led toont of data logging actief is.

   Peter Rutten
*/


#include "LIB_sensoren.h"

#define DATALOGGINGLEDPIN 2
#define DATALOGGINGSWITCHPIN 5
bool logdata = false; // gecontroleerd door schakelaar

// functie om data logging switch te lezen en led in te stellen
void readDataLogSwitch();

// SD kaart logger
#include <SPI.h>
#include <SD.h>
const int CHIPSELECT = 4; // is specifiek voor Ethernet Shield 2
const String FN = "data.log";
bool SDKaart = false;

// functie om SD kaart te initialiseren
void initSD();

// functie om data naar SD kaart te schrijven
void SDWrite();

// Sensor
// minT(C), maxT(C), minHum(%), maxHum(%)
TemperatureSensor temperatureSensor(21, 24, 35, 70);

// min(lux), ma(lux)
LightSensor lightSensor(300, 1000);

// minCO2(ppm), maxCO2(ppm), mintVOC(ppb), maxtVOC(ppb)
AirSensor airSensor(0, 1200, 0, 1200);

void setup() {

  // Seriële output
  Serial.begin(9600);

  // data logging switch en led

  pinMode(DATALOGGINGSWITCHPIN, INPUT);
  pinMode(DATALOGGINGLEDPIN, OUTPUT);

  // check data logging switch
  readDataLogSwitch();

  // Check of SD kaart aanwezig is, en schrijf header als
  // kaart gevonden is
  initSD();

  if (temperatureSensor.Start())
    Serial.println("Temperature sensor ok!");
  else
    Serial.println("Failed to start temperature sensor!");
  delay(500);
  
  if (airSensor.Start())
    Serial.println("Air sensor ok!");
  else
    Serial.println("Failed to start air sensor? Lijkt toch te werken...");
  delay(500);

  if (lightSensor.Start())
    Serial.println("Light sensor ok!");
  else
    Serial.println("Failed to start light sensor!");
  delay(500);

  // Logging
  Serial.println("Temp (C), Hum (%), CO2 (ppm), tVOC (ppb), Light (Lux)");


}

void loop() {

  // Temperature and humidity

  temperatureSensor.Read();
  temperatureSensor.PrintSerial();
  Serial.print(", ");

  // CO2 and TVOC

  if (airSensor.Read()) {

    airSensor.PrintSerial();
    Serial.print(", ");

  }

  // Ambient light

  if (lightSensor.Read()) {
    lightSensor.PrintSerial();
    Serial.println();

  }


  // write to SD card
  if (logdata) SDWrite();

  delay(10000);

  // check data logging switch
  readDataLogSwitch();
}


void initSD() {

  Serial.print("Zoek SD kaart...");

  if (!SD.begin(CHIPSELECT)) {
    Serial.println(" geen SD kaart gevonden!");
  }
  else {
    Serial.println(" OK!");
    SDKaart = true;
  }

  if (SDKaart) {

    File dataFile = SD.open(FN, FILE_WRITE);

    if (dataFile) {

      dataFile.println("Temp (C), Hum (%), CO2 (ppm), tVOC (ppb), Light (Lux)");
      dataFile.close();
    }
    else {
      Serial.println("error opening file!");
    }
  }

}

void readDataLogSwitch() {

  if (digitalRead(DATALOGGINGSWITCHPIN) == true) {
    //Serial.println("Data logging switch on");
    digitalWrite(DATALOGGINGLEDPIN, HIGH);
    logdata = true;
  }
  else {
    digitalWrite(DATALOGGINGLEDPIN, LOW);
    //Serial.println("Data logging switch off");
    logdata = false;
  }
}
void SDWrite() {

  //Serial.println("Writing to SD");
  if (SDKaart) {
    File dataFile = SD.open(FN, FILE_WRITE);

    if (dataFile) {
      dataFile.print(millis() / 1000);
      dataFile.print(": ");
      dataFile.print(temperatureSensor.GetValTemp());
      dataFile.print(", ");
      dataFile.print(temperatureSensor.GetValHum());
      dataFile.print(", ");
      dataFile.print(airSensor.GetValCO2());
      dataFile.print(", ");
      dataFile.print(airSensor.GetValtVOC());
      dataFile.print(", ");
      dataFile.println(lightSensor.GetVal());
      dataFile.close();
    }
    else {
      Serial.println("error opening file!");
    }
  }
}
