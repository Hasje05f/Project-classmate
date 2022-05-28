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
 //    #include <SD.h>

  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #include <SoftwareSerial.h>
  
      #define LED 2
      #define BUTTON 3
      #define POTSWITCH A2
      #define SWITCH 5

  #define Txd 10
  #define RxD 11
     
    int MaxLeerlingen;
    int OldMaxleerlingen;
    int AantLln;
    long randNumber;

  LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
  SoftwareSerial bluetoothSerial(10, 11);

     bool datalogging = false;
     bool SDCard = false;

  const int CHIPSELECT = 10; // is specifiek voor Velleman
  const String FILENAME = "dataHanMat.log";

    void SensorSetup();
//  void SDSetup();
    void SensorRead();
//  void SDWrite();
    void readDataLogSwitch();
    void LCDPrint();

 int x = 1;

////////////////////////////////////////////////////////// Sensoren //////////////////////////////////////////////////////////

  // minT(C), maxT(C), minHum(%), maxHum(%)
    TemperatureSensor temperatureSensor(21, 24, 35, 70);

  // min(lux), ma(lux)
    LightSensor lightSensor(300, 1000);

  // minCO2(ppm), maxCO2(ppm), mintVOC(ppb), maxtVOC(ppb)
    AirSensor airSensor(0, 1200, 0, 1200);

////////////////////////////////////////////////////////// Setup //////////////////////////////////////////////////////////

  void setup() {


  //  Serial.begin(9600);

       pinMode(SWITCH, INPUT);
       pinMode(LED, OUTPUT);
       pinMode(BUTTON, INPUT);
       pinMode(POTSWITCH, INPUT);

       randomSeed(analogRead(0));

    lcd.init();                      // initialize the lcd
    lcd.clear();
    lcd.backlight();
    
      SensorSetup();                                        
//      SDSetup();
      readDataLogSwitch();
      
      // Serial.println(" ");

  }     
    
//////////////////////////////////////////////////////////// Loop ////////////////////////////////////////////////////////////

void loop() {

  if (datalogging) {                                                                           // Printloop

          SensorRead();
          x = 1;

                int detectie = analogRead(POTSWITCH);
                delay(40);
                int detectie2 = analogRead(POTSWITCH);
 
                   OldMaxleerlingen = map(detectie, 0, 1023, 0, 30);
                   MaxLeerlingen = map(detectie2, 0, 1023, 0, 30);
                   
                    if (MaxLeerlingen == OldMaxleerlingen){
                         LCDPrint();
                    }

              
                    if (MaxLeerlingen != OldMaxleerlingen){
                            
                             long Tijd = 0;
                             Serial.println(Tijd);
                             lcd.clear();
                           
                                 while (Tijd < 1000){
                                  
                                      Tijd = Tijd + 25;
                                      Serial.println(Tijd);
                                      
                                      int detectie3 = analogRead(POTSWITCH);
                                      AantLln = map(detectie3, 0, 1023, 0, 30);
                                      
                                      lcd.setCursor(0,0);
                                      lcd.print("Max. lln.:  ");
                                      lcd.print(AantLln);
                                      delay(100);
                                  }
                                  
                            lcd.clear();

                        }
  
                          if(digitalRead(BUTTON) == HIGH) {
   
                          randNumber = random(1, AantLln);
                       
                            lcd.clear();
                            lcd.setCursor(0,0);
                            delay(10);
                            lcd.print("Rdm KlsNr.: ");
                            lcd.print(randNumber);
   
                            delay(2200);
                            lcd.clear();
                    }           
       delay(500); 
   }

   readDataLogSwitch();    
   delay(50);
       
   if(datalogging == false && x == 1){
    Serial.println("uit");
           lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("Switch is Off ");
        //   lcd.setCursor(0,1);
        //   lcd.print("HanMat off. ");
           delay(3000);
           lcd.clear();
           x = 0;
        }
   }



////////////////////////////////////////////////////////// Functies //////////////////////////////////////////////////////////


  void SensorSetup() {                                                                                       // SensorSetup

      if (temperatureSensor.Start())                                                // Initializing
         // Serial.println("Temperature sensor alright!");
   /*   else
         // Serial.println("Failed to start temperature sensor!");
         */

      delay(200);


      if (airSensor.Start())
        // Serial.println("Air sensor alright!");
  /*    else
        // Serial.println("Failed to start air sensor?");
        */

      delay(200);


      if (lightSensor.Start())
        // Serial.println("Light sensor alright!");
 /*     else
        // Serial.println("Failed to start light sensor!");
        */

      delay(200);
  }





  /* void SDSetup() {                                                                                                    // SDSetup

      // Serial.print("Search. SD:  ");                                                                                                                                    //// Serial.print("Searching SD Card:  ");
      delay(150);

      if (!SD.begin(CHIPSELECT))   {
          // Serial.println(" No SD ");    //// Serial.println(" No SD card found");
           SDCard = false;

      }
      
      else {
           // Serial.println(" OK!");                                                                                                                                 //// Serial.println(" OK!");
            SDCard = true;
      }

      if (SDCard) {

          File dataFile = SD.open(FILENAME, FILE_WRITE);

          if (dataFile) {

            dataFile.println("  -----  New Start -----  ");                                                                // ***                                         //STORAGEEEEE
              dataFile.println("Temp (C), Hum (%), CO2 (ppm), tVOC (ppb), Light (Lux)");
              dataFile.close();
      

          }

      }
       
     
     else 
      // // Serial.println("error open.");         
 

      delay(200);
 }
  
*/

  void SensorRead() {                                                                                                     // SensorRead

    // Temperature and humidity

      temperatureSensor.Read();
      temperatureSensor.PrintSerial();
      // Serial.print(" (°C,%)");
      // Serial.print(",   ");

    // Light

      lightSensor.Read();
      lightSensor.PrintSerial();
      // Serial.print(" (lx)");
      // Serial.print(",   ");


    // Air

      airSensor.Read();
      airSensor.PrintSerial();
      // Serial.print(",");
      // Serial.print(" (ppm, VOC)");
      // Serial.println();

    delay(200);
  }




 /* void SDWrite() {                                                                                                     // SDWrite


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
    
      else 
      // Serial.println("error open.");     
      // Serial.println("error opening file!");
    }
  }
  
*/

  void readDataLogSwitch() {                                                                                 // readDataLogSwitch

    if (digitalRead(SWITCH) == true) {
      
        digitalWrite(LED, HIGH);
        datalogging = true;
    }

    else {
        digitalWrite(LED, LOW);
        datalogging = false;
    }
    
    delay(200);
}




  void LCDPrint() {                                                                                                   // LCDPrint

      lcd.setCursor(0, 0);
  //  lcd.print("CO2:");
      lcd.print(airSensor.GetValCO2());
      lcd.print(" ppm");

      lcd.setCursor(0, 1);
  //  lcd.print("Licht:");
      lcd.print(lightSensor.GetVal());
      lcd.print(" lux");

      lcd.setCursor(9, 0);
  //  lcd.print("Hum:");
      lcd.print(temperatureSensor.GetValHum());
      lcd.print("%");

      lcd.setCursor(13, 0);
  //  lcd.print("Temp:");
      lcd.print(temperatureSensor.GetValTemp());
      lcd.print("C");

      lcd.setCursor(9, 1);
      lcd.print("Rec:");
      
//  File dataFile = SD.open(FILENAME, FILE_WRITE);
  
//    if(datalogging && dataFile)
      lcd.print("On");
 //   else
 //     lcd.print("Off");

  }
