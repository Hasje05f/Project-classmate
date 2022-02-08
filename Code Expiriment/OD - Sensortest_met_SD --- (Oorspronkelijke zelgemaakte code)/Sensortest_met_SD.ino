
////////////////////////////////////////////////////////// Defineren //////////////////////////////////////////////////////////

#include <SD.h>
#include "LIB_sensoren.h"

#define SWITCH 5
#define LED 2
#define DELAY 70

bool SDCard = false;
bool datalogging = false;
bool permission = true;

const int CHIPSELECT = 4;
const String FILENAME = "data4.log";

int i = 1;

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

                                                    //

 if (temperatureSensor.Start())
      Serial.println("TemperatureSensor alright!");
  else 
      Serial.println(" Failed to start TemperatureSensor :/  ");

      delay(DELAY);

                                                    //


  if (lightSensor.Start())
      Serial.println("lightSensor alright!");
  else 
      Serial.println(" Failed to start lightSensor :(  ");
      
      delay(DELAY);

                                                   //

  if (airSensor.Start())
      Serial.println("AirSensor alright!");
  else 
      Serial.println(" Failed to start AirSensor :/  ");

      delay(DELAY);

                                                   //
                                                   
   SDSetup();

   readDataLogSwitch();   
  
   File dataFile = SD.open(FILENAME, FILE_WRITE); 

  
  while(datalogging && SDCard){

      if (permission){
   
            dataFile.println(" ");
            dataFile.println("   New start: 03/01 20u  ");
      //    dataFile.println("Temperature (°C), Humidity (%), Brightness (lx), Co2(ppm), volatile organic compounds(ppm) ");
           
            Serial.println("File opened & Start marked"); 
            Serial.println(permission);
            permission = false;

        }
        
          SDWrite();
          
          dataFile.print(i);
          dataFile.println(": ");   
          Serial.print("Number of times printed: ");
          Serial.println(i);
          delay(DELAY*7);
          i++;        
          
         readDataLogSwitch();
       
      }

      dataFile.close(); 
      Serial.println("Datalogging off");
}

    

////////////////////////////////////////////////////////// Loop //////////////////////////////////////////////////////////

void loop() {

}

////////////////////////////////////////////////////////// Functies //////////////////////////////////////////////////////////

void SDWrite(){                                                                                                    //SDWrite
  
     File dataFile = SD.open(FILENAME, FILE_WRITE);
      
    // Temperature

        temperatureSensor.Read();
        
        dataFile.print(temperatureSensor.GetValTemp());
        dataFile.print(" (°C),    ");

        delay(50);

    // Humidity

        dataFile.print(temperatureSensor.GetValHum());
        dataFile.print(" (%),    ");

        delay(50);
        
    // Light
    
        lightSensor.Read();
        
        dataFile.print(lightSensor.GetValLight());
        dataFile.print(" (Lx),    ");
  
        delay(50);
  
    // CO2

        airSensor.Read();
          
        dataFile.print(airSensor.GetValCO2());
        dataFile.print(" (ppm),    ");

        delay(50);
  
    // VOC

        dataFile.print(airSensor.GetValtVOC());
        dataFile.print(" (ppm)    ");
      
      dataFile.println(" ");

}

void SDSetup(){                                                                                                    //SDSetup


  Serial.print("Searching SD Card:  ");
  delay(DELAY);

    if (!SD.begin(CHIPSELECT))  
            Serial.println("No SD card found");      
       
    else {
            Serial.println(" OK!");
            SDCard = true;
       }
        
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
