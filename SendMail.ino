#include <ESP8266WiFi.h>

// setup WiFi
const char* ssid = "test";
const char* password = "1514020215";
int temp = 28;

WiFiServer server(80);
WiFiClient client;

// MAC from ESP8266   
byte mac[] = { 0x5C, 0xCF, 0x7F, 0x0F ,0x73, 0x77 };

//Network setup
IPAddress ip( 192, 168, 2, 20 );   
IPAddress gateway( 192, 168, 2, 1 );
IPAddress subnet( 255, 255, 255, 0 );
char smtpcorp[] = "smtpcorp.com";
int port = 2525;



void setup()
{
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  server.begin();
  delay(10000);
  Serial.println(F("Ready. Press 'e' to send."));
}


byte sendEmail()
{ 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  byte thisByte = 0;
  byte respCode;

  if(client.connect( smtpcorp,port) == 1) {
    Serial.println(F("connected smtp2go"));
  } else {
    Serial.println(F("connection failed smtp2go"));
    return 0;
  }
  Serial.println(F("Sending ehlo"));
// replace 192.168.0.3 with your ESP8266's ip
  client.println("EHLO 192.168.2.20");
  Serial.println(F("Sending auth login"));
  client.println("auth login");
  Serial.println("Sending User");
// Change to your base64 encoded user
  client.println("ZmlsaXBtYXIubWZAZ21haWwuY29t");//
  Serial.println(F("Sending Password"));
// change to your base64 encoded password
  client.println("ZmlyZXdvcms=");//
// change to your email address (sender)
  Serial.println(F("Sending From"));
  client.println("MAIL From: beer@home.com");
// change to recipient address
  Serial.println(F("Sending To"));
  client.println("RCPT To: filipmar.mf@gmail.com");
  Serial.println(F("Sending DATA"));
  client.println("DATA");
  Serial.println(F("Sending email"));
// change to recipient address
  client.println("To: filipmar.mf@gmail.com");
// change to your address
  client.println("From: beer@home.com");
  client.println("Subject: Teplota piva\r\n");
  
  client.println("Zprava o prubehu kvaseni.");
  client.print("Teplota je: ");
  client.println(temp);

  client.println(".");
  Serial.println(F("Sending QUIT")); //Serial.println(F("Sending QUIT"));
  client.println("QUIT");
  client.stop();
  Serial.println(F("disconnected"));
  return 1;
}

void loop()
{
  byte inChar;

  inChar = Serial.read();

  if(inChar == 'e')
  
      if(sendEmail()) Serial.println(F("Email sent"));
      else Serial.println(F("Email failed"));
  } 
