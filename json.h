String getJSON(){
  String ptr = "{\n";
  //DHT22 sensor values
  ptr +="\"Temperature(F)\": \"";
  ptr +=(float)SensorTemperatureF;
  ptr +="\",\n";
  ptr +="\"Temperature(C)\": \"";
  ptr +=(float)SensorTemperatureC;
  ptr +="\",\n";
  ptr +="\"Humidity\": \"";
  ptr +=(float)SensorHumidity;
  ptr +="\",\n";
  /*
  //MQ2 sensor values
  ptr +="\"MQ2\": \"";
  ptr +=(int)SensorMQ2;
  ptr +="\",\n";
  ptr +="\"CarboMonoxide(ppm)\": \"";
  ptr +=(float)SensorCO;
  ptr +="\",\n";
  ptr +="\"LiquidPropaneGas(ppm)\": \"";
  ptr +=(float)SensorLP;
  ptr +="\",\n";
  ptr +="\"Smoke(ppm)\": \"";
  ptr +=(int)SensorSmoke;
  ptr +="\",\n";
  */
  //Geiger sensor values
  ptr +="\"Geiger CPM\": \"";
  ptr +=(float)SensorGeigerCPM;
  ptr +="\",\n";
  //PM2.5 sensor values
  ptr +="\"PM2.5\": \"";
  ptr +=(float)SensorPM;
  ptr +="\",\n";
  //Meta values
  ptr +="\"DataAge(ms)\": \"";
  ptr +=(int)(millis() - SensorsLastUpdated);
  ptr +="\",\n";
  ptr +="\"Battery(v)\": \"";
  ptr +=sensorBatteryVoltage;
  ptr +="\"\n";
  ptr +="}\n";
  return ptr;
}
