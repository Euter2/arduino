#include <ESP8266WiFi.h>

const int buttonPin = 5;     
boolean button = false;
unsigned long timeDebounce = 0;
int sleepTime = 60*1000000; // 60 seconds 

void setup() 
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Wake up!");
    
    pinMode(buttonPin, INPUT_PULLUP);
    attachInterrupt(buttonPin, interruptAction, FALLING);
}

void loop() 
{
    if (button)
      {
        button = false;
        Serial.println("Button pressed! ");
        Serial.println("Time to sleep!");
        ESP.deepSleep(sleepTime);  
        delay(1000); 
      }
}

void interruptAction() 
{
    if(millis() > timeDebounce+200)
      {
        button = true;
        timeDebounce=millis();
      }  
}
