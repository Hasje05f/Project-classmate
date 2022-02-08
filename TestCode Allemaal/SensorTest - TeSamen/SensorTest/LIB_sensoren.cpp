#include "LIB_sensoren.h"

//
// CLASS light sensor
//

LightSensor::LightSensor(long minVal, long maxVal) {
  this->lightSensor = new SparkFun_Ambient_Light(AL_ADDR);
  this->minVal = minVal;
  this->maxVal = maxVal;
  this->sensorStatus = OK;
};

LightSensor::~LightSensor() {
  delete this->lightSensor;
  this->lightSensor = NULL;
};

bool LightSensor::Start() {
  bool ret = this->lightSensor->begin();
  
  // Possible integration times in milliseconds: 800, 400, 200, 100, 50, 25
  // Higher times give higher resolutions and should be used in darker light.
 
  return ret;
};

bool LightSensor::Read() {
  this->val = this->lightSensor->readLight();
  return true;
};

void LightSensor::PrintSerial() {
  Serial.print(this->GetVal());
};

long LightSensor::GetVal() {
  return this->val;
};

SensorStatusType LightSensor::GetStatus() {

  if (this->val < this->minVal) return TOO_LOW;
  if (this->val > this->maxVal) return TOO_HIGH;
  return OK;
};

//
// CLASS air quality sensor
//


AirSensor::AirSensor(long minValCO2, long maxValCO2, long minValtVOC, long maxValtVOC) {
  this->airSensor = new CCS811(CCS811_ADDR);
  this->minValCO2 = minValCO2;
  this->maxValCO2 = maxValCO2;
  this->minValtVOC = minValtVOC;
  this->maxValtVOC = maxValtVOC;
};

AirSensor::~AirSensor() {
  delete this->airSensor;
  this->airSensor = NULL;
};

bool AirSensor::Start() {

  Wire.begin(); //Inialize I2C Hardware (? hoort dit hier thuis ?)

  if (this->airSensor->begin() == false)
  {
    return false;
  }
  else return true;
};

bool AirSensor::Read() {
  if (this->airSensor->dataAvailable())
  {
    this->airSensor->readAlgorithmResults();
    this->valCO2 = this->airSensor->getCO2();
    this->valtVOC = this->airSensor->getTVOC();
    return true;
  }
  else return false;
};


void AirSensor::PrintSerial() {
  Serial.print(String(this->GetValCO2()) + ", " + String(this->GetValtVOC()));
};

long AirSensor::GetValCO2() {
  return this->valCO2;
};

long AirSensor::GetValtVOC() {
  return this->valtVOC;
};

SensorStatusType AirSensor::GetStatusCO2() {

  if (this->valCO2 < this->minValCO2) return TOO_LOW;
  if (this->valCO2 > this->maxValCO2) return TOO_HIGH;
  return OK;
};

SensorStatusType AirSensor::GetStatustVOC() {

  if (this->valtVOC < this->minValtVOC) return TOO_LOW;
  if (this->valtVOC > this->maxValtVOC) return TOO_HIGH;
  return OK;
};



//
// CLASS humidity and temperature sensor
//

TemperatureSensor::TemperatureSensor(long minValTemp, long maxValTemp, long minValHum, long maxValHum) {

  this->dht = new DHT(DHT22_PIN, DHT22);
  this->minValTemp = minValTemp;
  this->maxValTemp = maxValTemp;
  this->minValHum = minValHum;
  this->maxValHum = maxValHum;
};

TemperatureSensor::~TemperatureSensor() {
  delete this->dht;
  this->dht = NULL;
};

bool TemperatureSensor::Start() {

  this->dht->begin();
  return true;
};

bool TemperatureSensor::Read() {
  this->valTemp = round(this->dht->readTemperature());
  this->valHum = round(this->dht->readHumidity());
  return true;
};


void TemperatureSensor::PrintSerial() {
  Serial.print(String(this->GetValTemp()) + ", " + String(this->GetValHum()));
};

long TemperatureSensor::GetValTemp() {
  return this->valTemp;
};

long TemperatureSensor::GetValHum() {
  return this->valHum;
};

SensorStatusType TemperatureSensor::GetStatusTemp() {

  if (this->valTemp < this->minValTemp) return TOO_LOW;
  if (this->valTemp > this->maxValTemp) return TOO_HIGH;
  return OK;
};

SensorStatusType TemperatureSensor::GetStatusHum() {

  if (this->valHum < this->minValHum) return TOO_LOW;
  if (this->valHum > this->maxValHum) return TOO_HIGH;
  return OK;
};
