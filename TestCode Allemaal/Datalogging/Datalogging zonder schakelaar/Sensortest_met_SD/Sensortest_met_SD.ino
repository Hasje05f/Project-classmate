
/*  Deze c++ code zal om de 2 seconden data van de sensoren opnemen en deze in een tekstbestand op de SD kaart schrijven. We 
    maken hiervoor gebruik van zelfgeschreven functies, libraries en de seriële monitor.

    Dit bestand zullen we uiteindelijk gebruiken in ons datalogexpiriment om onze gegevens in grafieken samen te vatten en
    conlusies uit te trekken.

    ~ Hannes Flament & Mattice Luwaert
    5IW

*/

////////////////////////////////////////////////////////// Defineren //////////////////////////////////////////////////////////

#include <SD.h>
#include "LIB_sensoren.h"

#define SWITCH 5
#define LED 2
#define DELAY 50
#define TIMEBETWEEN 1800

bool SDKaart = false;
bool datalogging = true;

const int CHIPSELECT = 4;
const String FILENAME = "data.log";

int n = 0;                                      // Variabele om eenmalige start in het bestand te markeren.
int i = 0;                                      // variabele om datalogging af te zetten wanneer de schakelaar uitstaat tot
                                                                           // een nieuwe positieverandering (Zie regel 109).
int Teller = 1;

////////////////////////////////////////////////////////// Sensoren //////////////////////////////////////////////////////////

// minT(C), maxT(C), minHum(%), maxHum(%)
TemperatureSensor temperatureSensor(20, 25, 40, 70);                                                         // Temperature //

// minLux(lx), maxLux(lx)
LightSensor lightSensor(300, 700);                                                                                 // Light //

//minCO2(ppm), maxCO2(ppm), minVOC(ppm), maxVOC(ppm)
AirSensor airSensor(400, 900, 0, 100);                                                                               // Air //


////////////////////////////////////////////////////////// Setup //////////////////////////////////////////////////////////

void setup() {

  Serial.begin(9600);   // Bitsnelheid Seriële monitor definiëren

  pinMode(LED, OUTPUT);     // pinModes definiëren
//  pinMode(SWITCH, INPUT);

  if (temperatureSensor.Start())                        // Start temperatuursensor controleren
    Serial.println("TemperatureSensor alright!");
  else
    Serial.println(" Failed to start TemperatureSensor :/  ");

  delay(DELAY);

  if (lightSensor.Start())                        // Start lichtsensor controleren
    Serial.println("lightSensor alright!");
  else
    Serial.println(" Failed to start lightSensor :/  ");

  delay(DELAY);

  if (airSensor.Start())                        // Start luchtsensor controleren
    Serial.println("AirSensor alright!");
  else
    Serial.println(" Failed to start AirSensor :/  ");


  SDSetup();                          // SD opstarten

}

////////////////////////////////////////////////////////// Loop //////////////////////////////////////////////////////////

void loop() {

for(n; n<1; n++){

    if (SDKaart == true && datalogging == true){
                
        File dataFile = SD.open(FILENAME, FILE_WRITE);

        dataFile.println("  New start: 08/12");
       // dataFile.println("Temp(°C), Hum(%), Bright(lx), Co2(ppm), VOC(ppm)");  // Eenheden verklaren (Te weinig geheugen voor)
        dataFile.close();

            for (int i = 0; i < 10; i++){
              
                SDWrite();
               
                Serial.print("Number of times printed: ");
                Serial.println(i+1);
                Serial.println("Data written on SD");   
                delay(TIMEBETWEEN);
            }
            
          dataFile.close();
          Serial.println("Loop ended");  
      }

      else
          Serial.println("Datalogging off or No SD found");
   
   }
 
}

////////////////////////////////////////////////////////// Functies //////////////////////////////////////////////////////////

void SDWrite() {                                                                                                   //SDWrite

  File dataFile = SD.open(FILENAME, FILE_WRITE);


  // Temperature

  temperatureSensor.Read();

  dataFile.print(temperatureSensor.GetValTemp());
  dataFile.print(" (°C),   ");
  
  delay(DELAY);


  // Humidity

  dataFile.print(temperatureSensor.GetValHum());
  dataFile.print(" (%),   ");

  delay(DELAY);


  // Light

  lightSensor.Read();

  dataFile.print(lightSensor.GetValLight());
  dataFile.print(" (Lx),     ");

  delay(DELAY);


  // CO2

  airSensor.Read();

  dataFile.print(airSensor.GetValCO2());
  dataFile.print(" (ppm),   ");

  delay(DELAY);


  // VOC

  dataFile.print(airSensor.GetValtVOC());
  dataFile.print(" (ppm)   ");

  dataFile.println(" ");
  dataFile.close();
}

void SDSetup() {                                                                                                   //SDSetup


  Serial.print("Searching SD card");
  delay(DELAY);

  if (!SD.begin(CHIPSELECT))
    Serial.println(" no SD card found!");

  else {
    Serial.println(" OK!");
    SDKaart = true;
  }

  Serial.println(" ");
}

/*
 
 
void readDataLogSwitch() {                                                                             // readDataLogSwitch //
  if (digitalRead(SWITCH) == HIGH) {
    datalogging = true;
    digitalWrite(LED, HIGH);
  }

  if (digitalRead(SWITCH) == LOW) {
    datalogging = false;
    digitalWrite(LED, LOW);
  }

}

*/
