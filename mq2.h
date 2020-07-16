
//#define MQPower (4)
#define         MQ_PIND                      (3)
#define         MQ_PIN                       (A2)     //define which analog input channel you are going to use
#define         RL_VALUE                     (5)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (9.83)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
#define         GAS_LPG                      (0)
#define         GAS_CO                       (1)
#define         GAS_SMOKE                    (2)
float           LPGCurve[3]  =  {2.3,0.21,-0.47};   
float           COCurve[3]  =  {2.3,0.72,-0.34};    
float           SmokeCurve[3] ={2.3,0.53,-0.44};    

/***************************  MQGetPercentage ********************************
Input:   rs_ro_ratio - Rs divided by Ro
         pcurve      - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm) 
         of the line could be derived if y(rs_ro_ratio) is provided. As it is a 
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic 
         value.
**********************************************************************************/ 
int  MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
  return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}

/**************** MQResistanceCalculation **************************************
Input:   raw_adc - raw value read from adc, which represents the voltage
Output:  the calculated sensor resistance
Remarks: The sensor and the load resistor forms a voltage divider. Given the voltage
         across the load resistor and its resistance, the resistance of the sensor
         could be derived.
**********************************************************************************/ 
float MQResistanceCalculation(int raw_adc){
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}
 

/***************************  MQRead *******************************************
Input:   mq_pin - analog channel
Output:  Rs of the sensor
Remarks: This function use MQResistanceCalculation to caculate the sensor resistenc (Rs).
         The Rs changes as the sensor is in the different consentration of the target
         gas. The sample times and the time interval between samples could be configured
         by changing the definition of the macros.
**********************************************************************************/ 
float MQRead(int mq_pin){
  int i;
  float rs=0;
 
  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }
 
  rs = rs/READ_SAMPLE_TIMES;
 
  return rs;  
}
 
/***************************  MQGetGasPercentage ********************************
Input:   rs_ro_ratio - Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the target gas
Remarks: This function passes different curves to the MQGetPercentage function which 
         calculates the ppm (parts per million) of the target gas.
**********************************************************************************/ 
int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
  if ( gas_id == GAS_LPG ) {
     return MQGetPercentage(rs_ro_ratio,LPGCurve);
  } else if ( gas_id == GAS_CO ) {
     return MQGetPercentage(rs_ro_ratio,COCurve);
  } else if ( gas_id == GAS_SMOKE ) {
     return MQGetPercentage(rs_ro_ratio,SmokeCurve);
  }    
 
  return 0;
}
 






void updateMQ2(){
  //SensorSmoke;
  //SensorLP;
  //SensorCO;
  //SensorMQ2;
  
  SensorMQ2 = digitalRead(MQ_PIND);
  Serial.print("MQ2(Bool): ");
  Serial.println(SensorMQ2);

  SensorCO = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_CO);
  SensorLP = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_LPG);
  SensorSmoke = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_SMOKE);
  
  Serial.print("Smoke: ");
  Serial.println(SensorSmoke);
  Serial.print("CO: ");
  Serial.println(SensorCO);
  Serial.print("LP: ");
  Serial.println(SensorLP);
  
}
