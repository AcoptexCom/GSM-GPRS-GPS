/*
  Project: ESP32 development board, Ai Thinker A6 GSM GPRS module, DHT11
  Function: Temperature and humidity readings over SMS.
  Ai Thinker A6 GSM GPRS module powered by power supply 5V DC 2A

  Check for more DIY projects on acoptex.com

  Ai Thinker A6 GSM GPRS module -> ESP32 development board
  GND           GND
  U_RXD         TX2 (GPIO17)
  U_TXD         RX2 (GPIO16)

  There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.

  U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
  U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
  U2UXD is unused and can be used for your projects.

*/

#include "DHT.h"            //library for DHT sensor

// Uncomment one of the lines below for whatever DHT sensor type you're using
#define DHTTYPE DHT11      // DHT 11
//#define DHTTYPE DHT21    // DHT 21 (AM2301)
//#define DHTTYPE DHT22    // DHT 22  (AM2302), AM2321
const int DHTPin = 2;      // DHT11 connected to D4 (GPIO4) pin of ESP32 development board
DHT dht(DHTPin, DHTTYPE);  // create an instance of the dht class called DHT
float h, t, f;             // Variables for temperature and humidity
String textMessage;        // Variable to store text message

void setup()
{
  Serial.begin(9600);                      // Initialise serial commmunication at 9600 bps
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // Initialise serial commmunication at 9600 bps on U2UXD serial port
  dht.begin();                             // Initialize DHT sensor
  pinMode(DHTPin, INPUT);                  // Set DHTPin as INPUT
  delay(2000);              // Give time to your GSM module logon to GSM network
  Serial.println("GSMmodule ready...");  //Print test in Serial Monitor
  Serial2.println("AT\r"); // AT command to set module to SMS mode
  delay(100); // Set delay for 100 ms
    Serial2.println("AT+CMGF=1\r"); // AT command to set module to SMS mode
  delay(100); // Set delay for 100 ms
  Serial2.println("AT+CNMI=2,2,0,0,0\r");// Set module to send SMS data to serial out upon receipt
  delay(100); // Set delay for 100 ms
  Serial2.print("AT+CMGD=1,4\r");//delete all the message in the storage
   delay(2000); // Set delay for 2 seconds
}

void loop()
{
  if (Serial.available() > 0)
    Serial2.write(Serial.read());
  if (Serial2.available() > 0)
    Serial.write(Serial2.read());
  if (Serial2.available() > 0) {
    textMessage = Serial2.readString();
    Serial.print(textMessage);
    delay(10);

  }
   
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  t = dht.readTemperature(); //Temperature in Celsius
  h = dht.readHumidity(); //Humidity
  f = dht.readTemperature(true); //Temperature in Fahrenheit
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (textMessage.indexOf("Status") >= 0) {
    textMessage = "";
    String message1 = "Temperature (Celsius): " + String(t);
    //String message1 = "Temperature (Fahrenheit): " + String(f);
    String message2 = " Humidity: " + String(h);
    String message = message1 + message2;
    sendSMS(message);
  }
  
}

// Function that sends SMS
void sendSMS(String message) {
  // REPLACE xxxxxxxxxxxx WITH THE RECIPIENT'S NUMBER
  // REPLACE ZZZ WITH THE RECIPIENT'S COUNTRY CODE
  Serial2.print("AT+CMGS = \"+ZZZxxxxxxxxx\"\r");
  delay(100);
  // Send the SMS
  Serial2.print(message);
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  Serial2.println((char)26);
  delay(100);
  Serial2.println();
  // Give module time to send SMS
  delay(5000);
  
  
}
