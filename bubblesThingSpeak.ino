#include <ESP8266WiFi.h>
#include <Timer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// wifi connection related variables
const char* ssid     = "XXX"; //<-- setup required!!!
const char* password = "XXX"; //<-- setup required!!!
const char* host = "api.thingspeak.com";
const char* thingspeak_key = "XXX"; //<-- setup required!!!
const long updatePeriod = 1*60*1000; //<-- setup required!!!

// buubble detection related variables
const int bubblePin = 5;
int bubbleCounter = 0;   
const int bubbleDebounceInterval = 500;
unsigned long timeDebounce = 0;

//temperature detection related variables
const int tempPin = 4;
float temp = 0.0;
const float offset = -4.0; //<-- setup required!!!
OneWire oneWire(tempPin);
DallasTemperature sensor(&oneWire);

// timer
Timer t;

void setup() 
{
    //debug serial communication
    Serial.begin(115200);
    //buuble detection 
    pinMode(bubblePin, INPUT);
    attachInterrupt(bubblePin, bubble, FALLING);
    //temperature sensor 
    sensor.begin();
    //timer setup
    t.every(updatePeriod, sendBubbles);
    //wifi start
    WiFi.hostname("BeerSensor");
    wifiConnect();   
}

void loop() 
{
  t.update();
}

void bubble() 
{
    if(millis() - timeDebounce >= bubbleDebounceInterval)
      {
        timeDebounce=millis();
        bubbleCounter++; 
      }  
}

void sendBubbles()
{
  //suspend interupt 
  detachInterrupt(bubblePin);
  //check if wifi is running 
  if (WiFi.status() != WL_CONNECTED) wifiConnect();
  //serial bubble output
  Serial.print("Bubbles per minute: ");
  Serial.println(bubbleCounter);
  //read value from temp sensor with calibration offset
  sensor.requestTemperaturesByIndex(0);
  temp = sensor.getTempCByIndex(0) + offset;
  //serial temp output
  Serial.print("Temperature: ");
  Serial.println(temp);
  //send data to server
  sendToThingSpeak(bubbleCounter, temp);
  //reset counter
  bubbleCounter = 0;
  //activate interrupt again
  attachInterrupt(bubblePin, bubble, FALLING);
}

void wifiConnect()
{
  Serial.println();
    Serial.println(); 
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
     while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi connected");
    Serial.println();
}

void sendToThingSpeak(int bubbleValue, float tempValue)
{  
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  String strData1 = String(bubbleValue);
  String strData2 = String(tempValue);
  String url = "/update?key=";
  url += thingspeak_key;
  url += "&field1=";
  url += strData1;
  url += "&field2=";
  url += strData2;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  Serial.println();
  }
