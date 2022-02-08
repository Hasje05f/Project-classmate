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


#include <SPI.h>
#include <SD.h>

const int CHIPSELECT = 4; // is specifiek voor Ethernet Shield 2
const String DATA = "hallo"; 
const String FN = "data.log";
#define DELAY 500

void setup() {

  // Om feedack te geven

  Serial.begin(9600);


  // Check of SD kaart aanwezig is

  Serial.print("Zoek SD kaart...");

  bool SDKaart = false;
  if (!SD.begin(CHIPSELECT)) {
    Serial.println(" geen SD kaart gevonden!");
  }
  else {
    Serial.println(" OK!");
    SDKaart = true;
  }

  if (SDKaart) {

    // Schrijf 10 maal een lijn data naar de SD kaart

    for (int i = 1; i < 11; i++) {
      File dataFile = SD.open(FN, FILE_WRITE);
      Serial.println(dataFile); 

      if (dataFile) {
        dataFile.print(i);
        dataFile.print(": ");
        dataFile.println(DATA);
        dataFile.close();
      }
      else {
        Serial.println("error opening datalog.txt");
      }
      delay(DELAY);
    }
    Serial.println("Data weggeschreven");
  }
}

void loop() {


}
