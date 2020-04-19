/*
  Project: ESP8266 ESP-12E module, Ai Thinker A6 GSM GPRS module, PIR sensor, REED switch - intruder detection system
  Function: Send SMS when door opened or motion detected.
  ACOPTEX.COM
*/
const int pirPin = 14;           //PIR sensor attached to ESP8266 ESP-12E GPIO 14(D5)
const int reedPin = 12;          //Door sensor attached to ESP8266 ESP-12E GPIO 12(D6)
const int delayTime = 5000;      //you can set time delay (in ms) between detection here

void setup()
{
  pinMode(pirPin, INPUT);        //sets pirPin as INPUT
  // Since the other end of the reed switch is connected to ground, we need
  // to pull-up the reed switch pin internally.
  pinMode(reedPin, INPUT);
  digitalWrite(pirPin, LOW);     //by default, no motion detected
  Serial.begin(9600);    // Initialise serial communication with GPS module at 9600 bps
  delay(4000);              // Give time to your GSM module logon to GSM network
  Serial.println("AT\r"); // AT command to set module to SMS mode
  delay(2000); // Set delay for 2 seconds
  Serial.println("AT+CMGF=1\r"); // AT command to set module to SMS mode
  delay(2000); // Set delay for 2 seconds
  Serial.println("AT+CNMI=2,2,0,0,0\r");// Set module to send SMS data to serial out upon receipt
  delay(2000);//sets delay for 2 seconds
  Serial.println("GSMmodule ready...");  //Print in Serial Monitor
  Serial.println("");
}

void loop()
{
  while (Serial.available() > 0)
    Serial.write(Serial.read());

  int sensorValue = digitalRead(pirPin);//reads the status of PIR sensor
  if (sensorValue == HIGH) {
    String message = "Motion detected";
    sendSMS(message);    
    delay(delayTime);
  }
 int proximity = digitalRead(reedPin); // Read the state of the switch
  if (proximity == LOW) // If the pin reads low, the switch is opened.
  {
   String message = "Door opened";
   sendSMS(message);
    //callMe();
    delay(delayTime);
  }  
}
// Function that calls your number
//void callMe() {
  // REPLACE THE xxxxxxxxxx WITH THE RECIPIENT'S NUMBER
  // REPLACE ZZZ WITH RECIPIENT'S COUNTRY CODE
//  Serial.println("ATD+ZZZxxxxxxxxxx"); //  change ZZZ with country code and xxxxxxxxxxx with phone number to dial  
//  delay(5000); // wait for 5 seconds...
//  Serial.println("ATH"); //hang up
 
//}
// Function that sends SMS
void sendSMS(String message) {
  // REPLACE THE xxxxxxxxxx WITH THE RECIPIENT'S NUMBER
  // REPLACE ZZZ WITH RECIPIENT'S COUNTRY CODE

  Serial.print("AT+CMGS = \"+ZZZxxxxxxxxxxx\"\r");
  delay(100);
  // Send the SMS
  Serial.println(message);
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  Serial.println((char)26);
  delay(100);
  Serial.println();
  // Give module time to send SMS
  delay(2000);
}
