#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);// TX, RX

void updateSerial()
{
  delay(2000);
  while (Serial.available()) {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop()
{
  mySerial.println("AT");           //We test AT communication
  updateSerial();
  mySerial.println("AT+CGATT=1");   //We attach the GPRS Mobile Termination (MT) to the Packet Domain service
  updateSerial();
  mySerial.println("AT+CGDCONT=1,\"IP\",\"wap\"");
  updateSerial();
  //We use this command to define PDP context.1 - Minimal value of (PDP Context Identifier), a numeric parameter
  //which specifies a particular PDP context definition, IP - Internet Protocol (IETF STD 5),wap - Access Point Name
  mySerial.println("AT+CIPSTATUS"); //We check the current connection status
  updateSerial();
  mySerial.println("AT+CGACT=1,1"); //Activate PDP; Internet connection is available after successful PDP activation
  updateSerial();
  mySerial.println("AT+CIFSR");     //We get the local IP address
  updateSerial();
  mySerial.println("AT+CIPSTART=TCP,acoptex.com,80"); //We connect to the server then the server will send back former data
  updateSerial();
  updateSerial();
  delay(2000);
  updateSerial();
  mySerial.println("AT+CIPSEND");                    //We send data request to the server
  updateSerial();
  mySerial.print("TEST");                            //We send data to the server
  updateSerial();
  mySerial.write(26);                                //Termination symbol
  updateSerial();
  mySerial.println("AT+CDNSGIP=acoptex.com");       //We query the IP address of given domain name
  updateSerial();
  mySerial.println("AT+CGACT=0");                    //We deactivate PDP
  updateSerial();
  mySerial.println("AT+CGATT=0");                   //We detach the GPRS Mobile Termination (MT) from the Packet Domain service
  updateSerial();
  while (1)
  {
    if (mySerial.available())
    {
      Serial.write(mySerial.read());//Data received by mySerial will be outputted by Serial
    }
    if (Serial.available())
    {
      mySerial.write(Serial.read());//Data received by Serial will be outputted by mySerial
    }
  }

}
