/*

  SD card datalogger

  De SD card is via de Ethernet Shield 2 verbonden met de Arduino Uno,
  en gebruikt volgende pinnen:


  SDO - pin 11
  SDI - pin 12
  CLK - pin 13
  CS  - pin 4
  
  Dit programma checkt of er een SD kaart aanwezig is,
  maakt een file met filenaam "DataLog.txt",
  en schrijft hier 10 maal een string "tijd(s) hallo" naar toe,
  met 0,5 s tussen elke schrijfactie.

  Peter Rutten

*/

#include "LIB_sensoren.h"
#include <SPI.h>
#include <SD.h>

const int CHIPSELECT = 4; // is specifiek voor Ethernet Shield 2
const String DATA = "hallo"; 
const String FN = "data.log";
#define DELAY 500


////////////////////////////////////////////////////////// Sensoren //////////////////////////////////////////////////////////


// minT(C), maxT(C), minHum(%), maxHum(%)
TemperatureSensor temperatureSensor(20, 25, 40, 70);                                                         // Temperature //    

// minLux(lx), maxLux(lx)
LightSensor lightSensor(300, 700);                                                                                 // Light //  

//minCO2(ppm), maxCO2(ppm), minVOC(ppm), maxVOC(ppm)
AirSensor airSensor(400, 900, 0, 100);                                                                               // Air //


////////////////////////////////////////////////////////// Functie //////////////////////////////////////////////////////////

void writeSD() {                                                                                                // writeSD //

  }


void setup() {

  // Om feedack te geven

  Serial.begin(9600);

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

  // Check of SD kaart aanwezig is

  Serial.print("Zoek SD kaart...");

  bool SDKaart = false;
  if (!SD.begin(CHIPSELECT)) {
    Serial.println(" geen SD kaart gevonden!");
    delay(100);
  }
  else {
    Serial.println(" OK!");
    SDKaart = true;
    delay(100);
  }

  if (SDKaart) {

  
      File dataFile = SD.open(FN, FILE_WRITE);
      Serial.println(dataFile);
      delay(100); 

      if (dataFile) {


         // Temperature and humidity

        bool DataTemp = temperatureSensor.Read();
        temperatureSensor.Read();
        temperatureSensor.PrintSerial();
        Serial.println("Temp");
        dataFile.print(DataTemp);
        dataFile.print(" (°C,%)");
        dataFile.print(",   ");


        // Light

        bool DataLight = lightSensor.Read();
        lightSensor.Read();
        lightSensor.PrintSerial();
        Serial.println("Light");
        dataFile.print(DataLight);
        dataFile.print(" (lx)");
        dataFile.print(",   ");


        // Air

        bool DataAir =  airSensor.Read();
        airSensor.Read();
        airSensor.PrintSerial();
        Serial.println("Air");
        dataFile.println(DataAir);
        dataFile.print(" (ppm, VOC)");
        dataFile.println();


        dataFile.close();

        
         Serial.println("Data weggeschreven");
         
      }
      
      else {
        Serial.println("error opening datalog.txt");
      }
      
      delay(DELAY);
    }
      }


void loop() {

}
