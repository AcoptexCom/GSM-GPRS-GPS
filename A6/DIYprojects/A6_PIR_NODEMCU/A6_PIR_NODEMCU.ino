/*
  Project: ESP8266 ESP-12E module, Ai Thinker A6 GSM GPRS module, PIR sensor - motion detection system
  Function: Send SMS when motion detected.
  ACOPTEX.COM
*/
const int pirPin = 14;           //PIR sensor attached to ESP8266 ESP-12E GPIO 14(D5)
const int delayTime = 15000;      //you can set time delay (in ms) between detection here

void setup()
{
  pinMode(pirPin, INPUT);        //sets pirPin as INPUT
  digitalWrite(pirPin, LOW);     //by default, no motion detected
  Serial.begin(9600);    // Initialise serial communicstion with GPS module at 9600 bps
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
}

// Function that sends SMS
void sendSMS(String message) {
  // REPLACE THE xxxxxxxxxx WITH THE RECIPIENT'S NUMBER
  // REPLACE ZZZ WITH RECIPIENT'S COUNTRY CODE

  Serial.print("AT+CMGS = \"+ZZZxxxxxxxxxx\"\r");
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
