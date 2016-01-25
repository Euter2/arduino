#include "Timer.h"                 
#include <TM1637Display.h>

//pin definition
const int ssrPin = 13;
const int CLK = 2;
const int DIO = 3;

//time constants 
const int timePeriod = 60*60;
const int pumpTime = 15;
const int waitTime=timePeriod-pumpTime;

//PUMP title for display
uint8_t pumpTitle[]{0b01110011,0b00111110,0b00110111,0b01110011};


//variables initialization
int secondsCounter = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;
int hourMin = 0;
boolean pumping = false;
boolean colon = true;

Timer t;                               
TM1637Display display(CLK, DIO);

void setup(void)
{
    
    //Serial.begin(9600); //uncoment for debug
    pinMode(ssrPin, OUTPUT);
    display.setBrightness(9);
    t.every(1000,updateTime);
}

void updateTime()
{
  secondsCounter++;
  
  if(pumping && secondsCounter>=pumpTime)
  {
    digitalWrite(ssrPin, LOW);
    secondsCounter = 0;
    pumping = false; 
  } 
  else if (!pumping && secondsCounter>=waitTime)
  {
    secondsCounter=0;
    digitalWrite(ssrPin, HIGH);
    pumping = true;  
  }
  
  //time calculation
  hours=(waitTime-secondsCounter)/3600;
  minutes=(waitTime-secondsCounter)%3601/60;
  seconds=(waitTime-secondsCounter)%3600%60;

  //display
  if (pumping){
    display.setColon(false);
    if(secondsCounter==0)display.setSegments(pumpTitle); //show PUMP instead 0 second
    else display.showNumberDec(secondsCounter,false);
  }
  else
  {
    //Serial.print(hours); //uncoment for debug
    //Serial.print(":");
    //Serial.print(minutes);
    //Serial.print(":");
    //Serial.println(seconds);
    
    hourMin=hours*100+minutes; //combine hours and minutes into one number
    if (colon) colon = false; //colon blink once per 2 sec
    else colon = true;
    display.setColon(colon);
    display.showNumberDec(hourMin,true);
    }
}

void loop(void)
{
    t.update();  //update timer   
}
