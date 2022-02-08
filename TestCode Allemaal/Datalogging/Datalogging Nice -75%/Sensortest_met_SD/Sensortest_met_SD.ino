
////////////////////////////////////////////////////////// Defineren //////////////////////////////////////////////////////////

#include <SD.h>
#include "LIB_sensoren.h"

#define SWITCH 5
#define LED 2
#define DELAY 70

bool SDKaart = false;
bool datalogging = false;

const int CHIPSELECT = 4;
const String FILENAME = "data.log";

int n = 0;

////////////////////////////////////////////////////////// Sensoren //////////////////////////////////////////////////////////

// minT(C), maxT(C), minHum(%), maxHum(%)
TemperatureSensor temperatureSensor(20, 25, 40, 70);                                                         // Temperature //    

// minLux(lx), maxLux(lx)
LightSensor lightSensor(300, 700);                                                                                 // Light //  

//minCO2(ppm), maxCO2(ppm), minVOC(ppm), maxVOC(ppm)
AirSensor airSensor(400, 900, 0, 100);                                                                               // Air //


////////////////////////////////////////////////////////// Setup //////////////////////////////////////////////////////////

void setup() {
  
Serial.begin(9600);

pinMode(LED, OUTPUT);
pinMode(SWITCH, INPUT);

 if (temperatureSensor.Start())
      Serial.println("TemperatureSensor alright!");
  else 
      Serial.println(" Failed to start TemperatureSensor :/  ");

      delay(DELAY);

  if (lightSensor.Start())
      Serial.println("lightSensor alright!");
  else 
      Serial.println(" Failed to start lightSensor :/  ");
      
      delay(DELAY);

  if (airSensor.Start())
      Serial.println("AirSensor alright!");
  else 
      Serial.println(" Failed to start AirSensor :/  ");


   SDSetup();
      
}

////////////////////////////////////////////////////////// Loop //////////////////////////////////////////////////////////

void loop() {

//readDataLogSwitch();
datalogging = true;
     
  for(n; n<1; n++){

    if (SDKaart == true && datalogging == true){
                
        File dataFile = SD.open(FILENAME, FILE_WRITE);

        dataFile.println("  New start: 28/12");
   //   dataFile.println("Temperature (°C), Humidity (%), Brightness (lx), Co2(ppm), volatile organic compounds(ppm) ");
        dataFile.close();
   //   Serial.println("Bestand geopend");

            for (int i = 0; i < 30; i++){
              
                SDWrite();
               
                Serial.print("Number of times printed: ");
                Serial.println(i+1);
                delay(DELAY);
                        
            }
            
          dataFile.close();
          Serial.println(" ");
          Serial.println("Data written on SD");
      }

      else
          Serial.println("Datalogging off or No SD found");
   
   }

      
}

////////////////////////////////////////////////////////// Functies //////////////////////////////////////////////////////////

void SDWrite(){                                                                                                    //SDWrite
  
     File dataFile = SD.open(FILENAME, FILE_WRITE);
     
    // Temperature

        temperatureSensor.Read();
        
        dataFile.print(temperatureSensor.GetValTemp());
        dataFile.print(" (°C),     ");

        delay(50);

    // Humidity

        dataFile.print(temperatureSensor.GetValHum());
        dataFile.print(" (%),     ");

        delay(50);
        
    // Light
    
        lightSensor.Read();
        
        dataFile.print(lightSensor.GetValLight());
        dataFile.print(" (Lx),     ");
  
        delay(50);
  
    // CO2

        airSensor.Read();
          
        dataFile.print(airSensor.GetValCO2());
        dataFile.print(" (ppm),     ");

        delay(50);
  
    // VOC

        dataFile.print(airSensor.GetValtVOC());
        dataFile.print(" (ppm)     ");
      
      dataFile.println(" ");
      dataFile.close();
}

void SDSetup(){                                                                                                    //SDSetup


  Serial.print("Searching SD card");
  delay(DELAY);

    if (!SD.begin(CHIPSELECT))  
            Serial.println(" no SD card gevonden!");      
       
    else {
            Serial.println(" OK!");
            SDKaart = true;
       }
        
       Serial.println(" ");
}


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
