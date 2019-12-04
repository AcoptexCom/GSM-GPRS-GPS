#include <SoftwareSerial.h> // Library for using software serial communication
SoftwareSerial SIM900(7, 8); // Pins 7, 8 are used as used as software serial pins

String incomingData;   // for storing incoming serial data
String message = "";   // A String for storing the message
int relay_pin = 2;    // Initialized a pin for relay module

void setup()
{
  Serial.begin(115200); // baudrate for serial monitor
  SIM900.begin(19200); // baudrate for GSM shield

  pinMode(relay_pin, OUTPUT);   // Setting erlay pin as output pin
  digitalWrite(relay_pin, LOW);  // Making relay pin initailly low

  // set SMS mode to text mode
  SIM900.print("AT+CMGF=1\r");  
  delay(100);
  
  // set gsm module to tp show the output on serial out
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  delay(100);
}

void loop()
{
  //Function for receiving sms
  receive_message();

  // if received command is to turn on relay
  if(incomingData.indexOf("A.light_On")>=0)
  {
    digitalWrite(relay_pin, HIGH);
    message = "Light is ON";
    // Send a sms back to confirm that the relay is turned on
    send_message(message);
  }
  
  // if received command is to turn off relay
  if(incomingData.indexOf("A.light_Off")>=0)
  {
    digitalWrite(relay_pin, LOW);
    message = "Light is OFF";
    // Send a sms back to confirm that the relay is turned off
    send_message(message);
  }        
}

void receive_message()
{
  if (SIM900.available() > 0)
  {
    incomingData = SIM900.readString(); // Get the data from the serial port.
    Serial.print(incomingData); 
    delay(10); 
  }
}

void send_message(String message)
{
  SIM900.println("AT+CMGF=1");    //Set the GSM Module in Text Mode
  delay(100);  
  SIM900.println("AT+CMGS=\"+37165738579\""); // Replace it with your mobile number
  delay(100);
  SIM900.println(message);   // The SMS text you want to send
  delay(100);
  SIM900.println((char)26);  // ASCII code of CTRL+Z
  delay(100);
  SIM900.println();
  delay(1000);  
}
