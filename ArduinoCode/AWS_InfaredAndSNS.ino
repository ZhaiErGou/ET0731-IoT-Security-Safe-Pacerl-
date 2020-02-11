/*
This is used for simple notification vis AWS SNS ,esp32 and mqtt
*/
// This include is for the AWS IOT library that we installed
#include <AWS_IOT.h>
// This include is for Wifi functionality
#include <WiFi.h>

// Declare an instance of the AWS IOT library
AWS_IOT hornbill;

// Wifi credentials
char WIFI_SSID[]="XXXXXXXX"; 
char WIFI_PASSWORD[]="XXXXXXX";

// AWS Thing details
char HOST_ADDRESS[]="XXXXXXXXXXXXXXXX.amazonaws.com"; 
char CLIENT_ID[]= "ESP32";
char TOPIC_NAME[]= "$aws/things/XXXXXX/shadow/update"; 

// Connection status
int status = WL_IDLE_STATUS;
// Payload array to store thing shadow JSON document
char payload[512];

// Bit for sending the MQTT message
int sendMessageBit = 0;

// Pin for the button
//const int buttonPin = 13;

//Pin for reciever 
const int reciever = 13;




void setup() {
  WiFi.disconnect(true);
  Serial.begin(115200);
  
// initialize the pushbutton pin as an input:
  //pinMode(buttonPin, INPUT);
  
  // initialise AWS connection
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Wifi network: ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
  }
  Serial.println("Connected to the Wifi!");
  if(hornbill.connect(HOST_ADDRESS,CLIENT_ID)== 0) {
    Serial.println("Connected to AWS, bru");
    delay(1000);
  }
  else {
    Serial.println("AWS connection failed, Check the HOST Address");
    while(1);
  }
}
void loop(){
    int irState = digitalRead(13);
     if (irState == 0) {
       sendMessageBit=1;
       sendMessage();
       sendMessageBit = 0;
       Serial.println("BOX OPEN");
       delay(100000);
  }
    Serial.println("BOX ClOSE");
}
void sendMessage() {
   int Open = 1;
    sprintf(payload,"{\"state\":{\"reported\":{\"Open\":\" %d \"}}}",Open);
    Serial.println(payload);
    if(hornbill.publish(TOPIC_NAME,payload) == 0) {
    Serial.println("Message was published successfully");}
        else {
    Serial.println("Message was not published");}
}
