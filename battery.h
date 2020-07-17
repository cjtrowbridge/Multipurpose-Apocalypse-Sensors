
float sensorAnalogReference;
float sensorBatteryVoltage;

void setupBattery(){
  //Serial.print("Analog Reference Voltage: ");
  //Serial.print(AR_DEFAULT);
  //Serial.println("v");
  
  //analogReference(AR_DEFAULT);
  //sensorAnalogReference = AR_DEFAULT;
  //Serial.print("Analog Reference Voltage: ");
  //Serial.print(sensorAnalogReference);
  //Serial.println("v");
}

void updateBattery(){
  int sensorValue = analogRead(ADC_BATTERY);
  float voltage = sensorValue * (3.3 / 1023.0);
  sensorBatteryVoltage = voltage;
  Serial.print("Battery Voltage: ");
  Serial.print(voltage);
  Serial.println("v");
  
}
