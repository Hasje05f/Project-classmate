/*  Library voor het werken met sensoren (Classmate project)
 *   
 *  Aansluitingen:
 *  
 *  Temperatuur/vochtigheidssensor DHT22
 *  
 *  DHT22     Arduino UNO
 *  
 *  AZ1       GND
 *  AZ2       -
 *  AZ3       D6
 *  AZ4       3.3V
 *  (AZ = achterzijde, van links naar rechts)
 *  
 *  Lichtsensor VEML6030
 *  
 *  VEML      Arduino UNO
 *  
 *  GND       GND
 *  3V3       3.3V
 *  SDA       SDA
 *  SCL       SCL
 *  
 *  Luchtkwaliteit (CO2 en VOC)
 *  
 *  GND       GND
 *  3V3       3.3V
 *  SDA       SDA
 *  SCL       SCL  
 *  WAK       GND
 *  
 *  
 */


// status values
typedef enum {TOO_LOW, TOO_HIGH, OK} SensorStatusType;

//
// CLASS light sensor
//

#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"
#define AL_ADDR 0x48

class LightSensor {
  private:
    SparkFun_Ambient_Light *lightSensor = NULL;
    long val = 0;
    long minVal = 0;
    long maxVal = 0;
    SensorStatusType sensorStatus = OK;

  public:
    LightSensor(long minVal, long maxVal);
    ~LightSensor();

    bool Start();
    bool Read();
    void PrintSerial();
    long GetVal();
    SensorStatusType GetStatus();
};

//
// CLASS air quality sensor
//

#include <Wire.h>
#include "SparkFunCCS811.h" //Click here to get the library: http://librarymanager/All#SparkFun_CCS811
#define CCS811_ADDR 0x5A //Default I2C Address

class AirSensor {

  private:

    CCS811 *airSensor = NULL;

    long valCO2 = 0;
    long valtVOC = 0;
    long minValCO2 = 0;
    long maxValCO2 = 0;
    long minValtVOC = 0;
    long maxValtVOC = 0;

  public:

    AirSensor(long minValCO2, long maxValCO2, long minValtVOC, long maxValtVOC);
    ~AirSensor();

    bool Start();
    bool Read();
    void PrintSerial();
    long GetValCO2();
    long GetValtVOC();
    SensorStatusType GetStatusCO2();
    SensorStatusType GetStatustVOC();
};


//
// CLASS humidity and temperature sensor
//

#include <DHT.h>
#include <DHT_U.h>
#define DHT22_PIN 6

class TemperatureSensor {

  private:

    DHT *dht = NULL;

    long valTemp = 0;
    long valHum = 0;

    long minValTemp = 0;
    long maxValTemp = 0;
    long minValHum = 0;
    long maxValHum = 0;

  public:

    TemperatureSensor(long minValTemp, long maxValTemp, long minValHum, long maxValHum);
    ~TemperatureSensor();

    bool Start();
    bool Read();
    void PrintSerial();
    long GetValTemp();
    long GetValHum();
    SensorStatusType GetStatusTemp();
    SensorStatusType GetStatusHum();
};
