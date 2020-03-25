/*
  Project: Arduino Nano, Ai Thinker A6 GSM GPRS module, LED
  Function: LED control by SMS. You can also read the state of ledPin by SMS
  Ai Thinker A6 GSM GPRS module powered by power supply 5V DC 2A
  Ai Thinker A6 GSM GPRS module -> Arduino Uno
  GND           GND
  U_TXD         digital pin 3
  U_RXD         digital pin 2
                
*/
//**********************************************************************************************
#include <SoftwareSerial.h> // Include library code to allow serial communication on other digital pins of the Arduino board
//**********************************************************************************************
String textMessage;        // Variable to store text message
String ledState;           // Create a variable to store LED state
const int ledPin = 4;      // LED connected to digital pin 8 of Arduino Uno
SoftwareSerial GSMmodule(3, 2); // RX, TX
void setup()
{
  Serial.begin(9600);       // Initialise serial commmunication at 9600 bps
  GSMmodule.begin(9600);    // Initialise serial communicstion with GPS module at 9600 bps
  pinMode(ledPin, OUTPUT);  // Set ledPin as OUTPUT
  digitalWrite(ledPin, LOW);// By default the LED is off
  delay(2000);              // Give time to your GSM GPS module logon to GSM network
  Serial.println("GSMmodule ready...");  //Print test in Serial Monitor
  GSMmodule.print("AT\r"); // AT command to set module to SMS mode
  delay(100); // Set delay for 100 ms
  GSMmodule.print("AT+CMGF=1\r"); // AT command to set module to SMS mode
  delay(100); // Set delay for 100 ms
  GSMmodule.print("AT+CNMI=2,2,0,0,0\r");// Set module to send SMS data to serial out upon receipt
  delay(100); // Set delay for 100 ms
}

void loop()
{
  if (Serial.available() > 0)
    GSMmodule.write(Serial.read());
  if (GSMmodule.available() > 0)
    Serial.write(GSMmodule.read());
  if (GSMmodule.available() > 0) {
    textMessage = GSMmodule.readString();
    Serial.print(textMessage);
    delay(10);

  }
  if (textMessage.indexOf("On") >= 0) {
    // Turn on relay and save current state
    digitalWrite(ledPin, HIGH);
    ledState = "ON";
    textMessage = "";
    String message = "LED is " + ledState;
    sendSMS(message);
  }
  if (textMessage.indexOf("Off") >= 0) {
    // Turn off relay and save current state
    digitalWrite(ledPin, LOW);
    ledState = "OFF";
    textMessage = "";
    String message = "LED is " + ledState;
    sendSMS(message);
  }
  if (textMessage.indexOf("State") >= 0) {
    if (digitalRead(ledPin) == HIGH )
    {
      ledState = "ON";
    }
    else
    {
      ledState = "OFF";
    };
    String message = "LED is " + ledState;
    sendSMS(message);
    textMessage = "";
  }
}

// Function that sends SMS
void sendSMS(String message) {
  // REPLACE xxxxxxxxxxxx WITH THE RECIPIENT'S NUMBER
  // REPLACE ZZZ WITH THE RECIPIENT'S COUNTRY CODE
  GSMmodule.print("AT+CMGS = \"+ZZZxxxxxxxxxx\"\r");
  delay(100);
  // Send the SMS
  GSMmodule.print(message);
  delay(100);
  Serial.println(message);

  // End AT command with a ^Z, ASCII code 26
  GSMmodule.println((char)26);
  delay(100);
  GSMmodule.println();
  // Give module time to send SMS
  delay(5000);
}
