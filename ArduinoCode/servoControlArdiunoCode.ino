/*
 * This is used to control the servo motor by esp32 via mqtt
 * It is a open-sourced and modified by
 * Raymond,ZhiWei and Yiming
 * Feel free to take it as a reference
 */
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Servo.h>

#define LED_PIN 12
//declare varibables here
Servo myservo;
int pos = 0; 
int door =0;
/* Change the following configuration options */
const char* ssid = "Zhiwei";
const char* password = "zhiwei123";
const char* aws_iot_hostname = "a14to2g827y2g3-ats.iot.us-east-1.amazonaws.com";
const char* aws_iot_sub_topic = "topic/servo";
const char* aws_iot_pub_topic = "another/topic/echo";
const char* aws_iot_pub_message = "Acknowledged.";
const char* client_id = "MyIoT";

const char* ca_certificate = "-----BEGIN CERTIFICATE-----\nMIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\nADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\nb24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\nMAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\nb3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\nca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\nIFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\nVOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\njgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\nAYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\nA4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\nU5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\nN+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\no/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\nrqXRfboQnoZsG4q5WTP468SQvvG5\n-----END CERTIFICATE-----"; //CA certificate
const char* iot_certificate = "-----BEGIN CERTIFICATE-----\nMIIDWTCCAkGgAwIBAgIUTPFUVsaGgC1uLsNTXj0oANv+cokwDQYJKoZIhvcNAQEL\nBQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\nSW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMDIxMDA0MjUz\nOVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\nZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALDNhFm70CN24avXs/Ir\nnwwe4R6ad0dnNWLzk5Djx8UokOX9knzLVHqkFrVQxhj9bbJoQX6DDlQ4uce0Zp2r\ngeWWMm8WxgkgYIfbq9drIEMDNM+lb1vEbwiz0tzMgqlqZrNJ4n83Wo8BlXO0azQR\ncJBdPj4wIj6HkegnKaVlnDPyPIN99Va5J8wBqZx9wa4gW8+JmlYJk3/3+13pkB7A\nItiTx0As/J6InvyoB+eshU4p4UTnK9VPpbo+Y84vHHDZLNPAeyHDX8ho0+GjGThI\nAX5TH/vVp8x7CWHsrugjYhn1CrpX2pVwfNkj9Xe0qiKGHZBTlZC8zp753mLyRmbM\njpcCAwEAAaNgMF4wHwYDVR0jBBgwFoAUxX0HdYT4ePPhMWfkAGz7bhGA1P0wHQYD\nVR0OBBYEFOMKcrqHcxsSm1C9lZ+HhaIyylHpMAwGA1UdEwEB/wQCMAAwDgYDVR0P\nAQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCDjd0xDH4hwwoPi5+RcnHOW6Hg\nsVeb8TPu4eIh0+bv78OXGWZ1z0yFWpyGtgWYDoEnl6tf+u29hku3dfmPWlkB6zTn\nIsJULDMgd5IZrG+F06iq5Qo5LpkQk6fgTZJog4Q/rkyTtnDXM97svvHmtZDQMfv2\nKb5tPNnWmo15BcAqdab5tfRVYm93g1jxD67BySsFq/VX5rYA3WIA8MnXGs0Azczh\nvRuRBM9vsY9lkMYo+LHauwVLOdwxfg/ZM8lUAwq69FOY5uAJCo0xo4E9C3t5+YCb\nIXsgPBCHWtH5RTNDeAXcg1AjjSwu0xBBje0CqRwNO9aQfyOBgDHTvQTICzVN\n-----END CERTIFICATE-----\n"; //Own certificate
const char* iot_privatekey = "-----BEGIN RSA PRIVATE KEY-----\nMIIEowIBAAKCAQEAsM2EWbvQI3bhq9ez8iufDB7hHpp3R2c1YvOTkOPHxSiQ5f2S\nfMtUeqQWtVDGGP1tsmhBfoMOVDi5x7RmnauB5ZYybxbGCSBgh9ur12sgQwM0z6Vv\nW8RvCLPS3MyCqWpms0nifzdajwGVc7RrNBFwkF0+PjAiPoeR6CcppWWcM/I8g331\nVrknzAGpnH3BriBbz4maVgmTf/f7XemQHsAi2JPHQCz8noie/KgH56yFTinhROcr\n1U+luj5jzi8ccNks08B7IcNfyGjT4aMZOEgBflMf+9WnzHsJYeyu6CNiGfUKulfa\nlXB82SP1d7SqIoYdkFOVkLzOnvneYvJGZsyOlwIDAQABAoIBAHhrcjKgDhKpnOkr\nIH/YIBrkSVZLxOQYMq9z0VVFH0J078QGIqjm4GIrst/J5TMOIZH7GmitoKvSew6/\nG4lWUEHsO+uX6kg7CiRehTtS1oVuAKFRP3iUYT/fIsaKcJAtTUfeEeut6vsuC/nj\nr4SmQDHDoGpA5IIMWl9xlRb3Dx39Qx2wXAH2xYQztkDKFWsCx2OOv2bPbdMmrDhJ\nC/qoSudfHKVmDwfVV58lfE96MChyh6VgPSuSNZFQvUi9FbLAls75EZd8IWZW/Rfz\nXN0ZxhLPLOfwjuhEHDpGkhKCow+RqVJtTRSr/ld0UgWi3WEeaZYBPCN8Gh2eqByU\nbRS+1QkCgYEA6SnBl4YOFu3yxUr56L6tkXO0jmdkUuGZC6R3AweDjJGxVmnOJccr\n5P6xYJG6RzI0KVEYlAzzS6Brxfr1uzPaoUROzK98Ieyin7+KmK+v0wj7jldF0e9G\nOMlvTsW7otPIFbWzFKtxiy3VsljZlDuVUEvZqrOeS1fK27bkIPIloGsCgYEAwh6a\niVrCr6yHMJfTeRssDQFHc6BVPk9NnTf/cHrTdtU+DdKiAcIN0dd8WVIW4YAEYIgw\nAc88Kl07YIlY92/l25u9rjDdXxLkfoYxxyjI+xuzQLDDeziUIs6OYk1EBn34AUvt\nknyx5d0YMbV3oF8pftktpRMES0I5Wjqz3whiZYUCgYEAz5RXP/kfDhblrfQkLauk\nhvwF4Un3LTqfsJfJRK8p1LnYseAcodGCKKr/a8QRnVT6z1XdleE5sTiu+mGE6Bcw\nK6bOqLgKwYEU/zfdwjWXSUCUBJN6G370e5YKPI0ZkcsONRXAy0QfIBO8Tr1/Z9Ls\nCcKdlLW41cbhWV6oFxS9nuUCgYBI0sZSxRPIZ8cHuc2B32m1L0eo3vtbttB9LKbN\ngaaXLARtlxlj43LfyCFEOQGQc0AxHqYz55TAp91AqAYSPY5TxqdwE7DYm/PQa3N9\nLZXIbmne2PFc5qrcK/VKd4YPkW5avitQCyuJ+lm7PEK2jTy586umA+5WdW+io+ca\nwsB3KQKBgFVLRsI3TKRRZPra5agWSRqgDQ8tk/ih6jJFLz1WvBU0xFJh0p0UU73X\nYaL+np2VCdNBEgZwQdqKjr252Cv1TSxYRsUGfRqQyyR/P3+srh+byR4t9OMbVDWb\n4vtMUM84inlWma0SH99gWDn8kCVh8FpmACqZ+TxnKjpSTwNgohxY\n-----END RSA PRIVATE KEY-----\n"; //Own private key

#define SSID_HAS_PASSWORD //comment this line if your SSID does not have a password

/* Global Variables */
WiFiClientSecure client;
PubSubClient mqtt(client);

void openDoor(){
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  door = 1;                         //change the status to 1  as lock opened
}

void closeDoor(){
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  door = 0;                          // change the status to 0 as lock opened
}

/* Functions */
void sub_callback(const char* topic, byte* payload, unsigned int length) {
  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  for (int i = 0; i < length; i++)
    Serial.print((char) payload[i]);
  Serial.println();

  if ((char) payload[0] == '1'){
    if (door ==0){                  // if lock is closed then unlock ,otherwise no movement
      openDoor();
    }
  }
  else if ((char) payload[0] == '0'){
    if (door ==1){                  //// if lock is opened then lock ,otherwise no movement
      closeDoor();
    }
  }
  mqtt.publish(aws_iot_pub_topic, aws_iot_pub_message);
}

void setup() {
  //Initializations
  Serial.begin(115200);
  Serial.print("Attempting WiFi connection on SSID: ");
  Serial.print(ssid);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  myservo.attach(13);             //setup the Input pin for servo here
  // WiFi
  #ifdef SSID_HAS_PASSWORD
  WiFi.begin(ssid, password);
  #else
  WiFi.begin(ssid);
  #endif

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.print("\nWiFi connection succeeded.\n");

  client.setCACert(ca_certificate);
  client.setCertificate(iot_certificate);
  client.setPrivateKey(iot_privatekey);

  // AWS IoT MQTT uses port 8883
  mqtt.setServer(aws_iot_hostname, 8883);
  mqtt.setCallback(sub_callback);
}

void loop() {
  // reconnect on disconnect
  while (!mqtt.connected()) {
    Serial.print("Now connecting to AWS IoT: ");
    if (mqtt.connect(client_id)) {
      Serial.println("connected!");
      mqtt.subscribe(aws_iot_sub_topic); //subscribe to the topic
    } else {
      Serial.print("failed with status code ");
      Serial.print(mqtt.state());
      Serial.println(" trying again in 5 seconds...");
      delay(5000);
    }
  }

  mqtt.loop();
}
