//Sensor value cache variables
float SensorTemperatureC = 0;
float SensorTemperatureF = 0;
float SensorHumidity;
float SensorSmoke;
float SensorLP;
float SensorCO;
int SensorMQ2;
float SensorPMl10;
float SensorPMl25;
float SensorPMl100;
float SensorPMi;
float SensorPMia;
float SensorPMib;
float SensorPMic;
float SensorPMid;
int SensorsLastUpdated;
float Ro;

#include "pm.h" 
#include "mq2.h" 
#include "dht22.h" 

void updateSensorData(){
  Serial.println("");
  Serial.println("Updating sensor data...");
  updatePMi();
  updatePMl();
  //updateMQ2();
  updateDHT22();
  updateGeiger();
  SensorsLastUpdated = millis();
  updateBattery();
  Serial.println("All sensors updated. Waiting thirty seconds...");
  Serial.println("");
}

void loopSensors(){
  //update the values for all the sensors if it has been more than thirty seconds
  int DataAge = (millis() - SensorsLastUpdated);
  if(DataAge > (30 * 1000)){
    SensorsLastUpdated = millis();
    updateSensorData();
  }
}
