#include <Bounce2.h>

//Team count setup
const int teams = 8; 
// Count of buttons for each team setup 
const int teamButtons = 3;

const int buttons = teams*teamButtons;

// Instantiate a Bounce objects array
Bounce bouncedPins[buttons]=Bounce();

void setup() 
{
  // Print initial info to serial 
  Serial.begin(57600);
  Serial.print("Input initialization for ");
  Serial.print(teams);
  Serial.println(" teams");

  // Initialize all pins
  for(int i=2; i<=buttons+1; i++)
    {
      pinMode(i, INPUT_PULLUP);
      bouncedPins[i-2].attach(i);
      bouncedPins[i-2].interval(10);
    }
  
  // Print post info about initialization
  Serial.print("Inputs 2-");
  Serial.print(buttons+1);
  Serial.println(" are initialized");
  Serial.println();
}

void loop() 
{
   for(int i=0; i<buttons; i++)
    {
      // Update bouncer  
      bouncedPins[i].update();
      
      // Check if state is changed  
      if ( bouncedPins[i].fell() )  
        {
            // Print info about team and pressed button to serial
            Serial.print("Team ");
            Serial.print(i/teamButtons+1);
            Serial.print(" Button ");
            Serial.println(i%teamButtons+1);
        }
    } 
}
