#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);  

void setupDHT(){
  dht.begin();
}

void updateDHT22(){
  //SensorTemperatureC;
  //SensorTemperatureF;
  //SensorHumidity;
  //pinMode(DHTPIN, INPUT_PULLUP);
  delayMicroseconds(10);


  SensorHumidity = dht.readHumidity(); // Gets the values of the humidity 
  SensorTemperatureC = dht.readTemperature(); // Gets the values of the temperature
  SensorTemperatureF = (SensorTemperatureC * 9/5) + 32;
  
  Serial.print("Humidity(%): ");
  Serial.println(SensorHumidity);
  
  Serial.print("Temperature(C): ");
  Serial.println(SensorTemperatureC);
  
  Serial.print("Temperature(F): ");
  Serial.println(SensorTemperatureF);
}
