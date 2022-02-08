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


/*  Deze c++ code zal om de 2 seconden data van de sensoren opnemen en deze in een tekstbestand op de SD kaart schrijven. We 
    maken hiervoor gebruik van zelfgeschreven functies, libraries en de seriële monitor.

    Dit bestand zullen we uiteindelijk gebruiken in ons datalogexpiriment om onze gegevens in grafieken samen te vatten en
    conlusies uit te trekken.

    ~ Hannes Flament & Mattice Luwaert
    5IW

*/

////////////////////////////////////////////////////////// Defineren //////////////////////////////////////////////////////////

#include "LIB_sensoren.h"
#include <SPI.h>
#include <SD.h>

#define LED 2
#define SWITCH 5

bool datalogging = false; 
bool SDCard = false;

const int CHIPSELECT = 4; // is specifiek voor Ethernet Shield 2
const String FILENAME = "data.log";

void SDSetup();
void readDataLogSwitch();
void SDWrite();

int i = 1;

////////////////////////////////////////////////////////// Sensoren //////////////////////////////////////////////////////////

// minT(C), maxT(C), minHum(%), maxHum(%)
TemperatureSensor temperatureSensor(21, 24, 35, 70);

// min(lux), ma(lux)
LightSensor lightSensor(300, 1000);

// minCO2(ppm), maxCO2(ppm), mintVOC(ppb), maxtVOC(ppb)
AirSensor airSensor(0, 1200, 0, 1200);

////////////////////////////////////////////////////////// Setup //////////////////////////////////////////////////////////

void setup() {


  Serial.begin(9600);

  pinMode(SWITCH, INPUT);
  pinMode(LED, OUTPUT);


  if (temperatureSensor.Start())                                                                          // Initializing
    Serial.println("Temperature sensor alright!");
  else
    Serial.println("Failed to start temperature sensor!");
    
  delay(200);

  
  if (airSensor.Start())
    Serial.println("Air sensor alright!");
  else
    Serial.println("Failed to start air sensor?");
    
  delay(200);
  

  if (lightSensor.Start())
    Serial.println("Light sensor alright!");
  else
    Serial.println("Failed to start light sensor!");
        
  delay(200);

  SDSetup();

  readDataLogSwitch();

  Serial.println(" ");
 

  // Logging
  // Serial.println("Temp (C), Hum (%), CO2 (ppm), tVOC (ppb), Light (Lux)");

 while(datalogging && SDCard){                                                                             // Printloop
        
          // Temperature and humidity

            temperatureSensor.Read();
         /* temperatureSensor.PrintSerial();
            Serial.print(" °C, %,    ");
         */
 

          // CO2 and TVOC

            if (airSensor.Read()) {
               /* airSensor.PrintSerial();
                  Serial.print(" ppm, ppb,   ");
               */
            }

  
          // Ambient light

            if (lightSensor.Read()) {   
              /* lightSensor.PrintSerial();
                 Serial.println(" Lx    ");    
              */
             }


            if (datalogging) {
    
                SDWrite();
                Serial.print("Number of times printed: ");
                Serial.println(i);
                i++;
             }
            
             delay(90000);
             readDataLogSwitch();
   }
  
    Serial.println("datalogging off");                                                                          // End of Loop 
}

//////////////////////////////////////////////////////////// Loop ////////////////////////////////////////////////////////////

 void loop() {

}

////////////////////////////////////////////////////////// Functies //////////////////////////////////////////////////////////

void SDSetup() {                                                                                                    // SDSetup
  
  Serial.print("Searching SD Card:  ");
  delay(70);

    if (!SD.begin(CHIPSELECT))  
            Serial.println("No SD card found");      
       
    else {
            Serial.println(" OK!");
            SDCard = true;
       }

  if (SDCard) {

    File dataFile = SD.open(FILENAME, FILE_WRITE);

    if (dataFile) {
      
    //  dataFile.println("  -----  New Start -----  ");                                                                // ***
      dataFile.println("Temp (C), Hum (%), CO2 (ppm), tVOC (ppb), Light (Lux)");
      dataFile.close();
    }
    else {
      Serial.println("error opening file!");
    }
  }

}

void readDataLogSwitch() {                                                                                 // readDataLogSwitch

  if (digitalRead(SWITCH) == true) {
    digitalWrite(LED, HIGH);
    datalogging = true;
  }
  
  else {
    digitalWrite(LED, LOW);
    datalogging = false;
  }
  
}


void SDWrite() {                                                                                                     // SDWrite


  if (SDCard) {
    File dataFile = SD.open(FILENAME, FILE_WRITE);

    if (dataFile) {
      dataFile.print(i);
      dataFile.print(":   ");
      dataFile.print(temperatureSensor.GetValTemp());
      dataFile.print(", ");
      dataFile.print(temperatureSensor.GetValHum());
      dataFile.print(",    ");
      dataFile.print(lightSensor.GetVal());
      dataFile.print(",    ");
      dataFile.print(airSensor.GetValCO2());
      dataFile.print(", ");
      dataFile.println(airSensor.GetValtVOC());
      dataFile.close();
    }
    else {
      Serial.println("error opening file!");
    }
  }
}
