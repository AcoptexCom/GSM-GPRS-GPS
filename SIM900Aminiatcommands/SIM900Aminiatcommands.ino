/* 
 Project: SIM900A GSM GPRS MINI Board 
 Function: Serial communication
 
 RX is digital pin 7 of Arduino board (connect to TXD of SIM900A GSM GPRS MINI Board )
 TX is digital pin 8 of Arduino board (connect to RXD of SIM900A GSM GPRS MINI Board )
 */
//********************************************
#include <SoftwareSerial.h>//include library code
//********************************************
SoftwareSerial SIM900Amodule(7, 8);//RX, TX
//**********************************************
void setup()
{
SIM900Amodule.begin(19200);   
Serial.begin(19200);  
Serial.println("Please enter AT command:");  
delay(100);
}
void loop()
{
 if (Serial.available()>0)
 SIM900Amodule.write(Serial.read());
 if (SIM900Amodule.available()>0)
 Serial.write(SIM900Amodule.read()); 
}
