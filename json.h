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
  
  //PM2.5 laser sensor values
  ptr +="\"PM1.0 Laser\": \"";
  ptr +=(float)SensorPMl10;
  ptr +="\",\n";
  ptr +="\"PM2.5 Laser\": \"";
  ptr +=(float)SensorPMl25;
  ptr +="\",\n";
  ptr +="\"PM10 Laser\": \"";
  ptr +=(float)SensorPMl100;
  ptr +="\",\n";

  //PM2.5 infrared sensor values
  ptr +="\"PM2.5 Infrared (Method A)\": \"";
  ptr +=(float)SensorPMia;
  ptr +="\",\n";
  ptr +="\"PM2.5 Infrared (Method B)\": \"";
  ptr +=(float)SensorPMib;
  ptr +="\",\n";
  ptr +="\"PM2.5 Infrared (Method C)\": \"";
  ptr +=(float)SensorPMic;
  ptr +="\",\n";
  ptr +="\"PM2.5 Infrared (Method D)\": \"";
  ptr +=(float)SensorPMid;
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
