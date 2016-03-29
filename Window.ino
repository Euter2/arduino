#include <ESP8266WiFi.h>
#include <MQTTClient.h>

//wifi setup
const char *ssid = "test";
const char *pass = "1514020215";

//MQTT setup
const char * broker="176.74.137.214";

const char * pubTopic="/window/out";
const char * subTopic="/window/in";

WiFiClient net;
MQTTClient client;
void connect(); // <- predefine connect() for setup()

//HW setup
const int motorTime = 30*1000; // <- time to open and close the window in seconds
const int motorUpPin= 4;
const int motorDownPin = 5;
boolean moveUp = false;
boolean moveDown = false;
long movementTimer = 0;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  client.begin(broker, net);
  connect();
  
  pinMode(motorUpPin, OUTPUT);
  pinMode(motorDownPin, OUTPUT);  
}

void connect() {
  Serial.print("\nchecking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("myESP8266")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");

  client.subscribe(subTopic);
  // client.unsubscribe(subTopic);
}

void loop() {
  client.loop();
  delay(10); // <- fixes some issues with WiFi stability

  if(!client.connected()) {
    connect();
  }

  if(moveUp && millis() - movementTimer >= motorTime)
  {
    digitalWrite(motorUpPin, LOW);
    moveUp = false;
    Serial.println("Window was opened");
    client.publish(pubTopic, "Window was opened");  
  } 
  else if (moveDown && millis() - movementTimer >= motorTime)
  {
    digitalWrite(motorDownPin, LOW);
    moveDown = false;
    Serial.println("Window was closed");
    client.publish(pubTopic, "Window was closed");  
  } 
   
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
  if (payload == "open" && !moveUp && !moveDown) 
  {
    moveUp = true;
    movementTimer=millis();
    digitalWrite(motorUpPin, HIGH);
    Serial.println("Opening window in progress...");
    client.publish(pubTopic, "Opening window in progress...");
  } 
  else if (payload == "close" && !moveUp && !moveDown) 
  {
    moveDown = true;
    movementTimer=millis();
    digitalWrite(motorDownPin, HIGH);
    Serial.println("Closing window in progress...");
    client.publish(pubTopic, "Closing window in progress...");  
  } 
  else client.publish(pubTopic, "Command not found");
}
