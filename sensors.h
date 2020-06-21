//Sensor value cache variables
float SensorTemperatureC = 0;
float SensorTemperatureF = 0;
float SensorHumidity;
float SensorSmoke;
float SensorLP;
float SensorCO;
int SensorMQ2;
float SensorPM;
int SensorsLastUpdated;
float Ro;

#include "pm.h" 
#include "mq2.h" 
#include "dht22.h" 

void updateSensorData(){
  Serial.println("");
  Serial.println("Updating sensor data...");
  updatePM();
  updateMQ2();
  updateDHT22();
  SensorsLastUpdated = millis();
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
