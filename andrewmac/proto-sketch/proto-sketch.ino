/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include "Timer.h"

const int RECV_PIN = 13;
const int TX_PIN = 5;
IRrecv irrecv(RECV_PIN);
IRsend irsend(TX_PIN); //an IR led is connected to GPIO pin 0

decode_results results;

const char* ssid     = "mbd-west-wing";
const char* password = "monkeyfarm";

const char* host = "10.85.0.200";

int badgeList[10];
int numBadges = 0;

int thisBadgeInt = 0xb001a1;

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 14;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 16;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 12;

Timer t;                               //instantiate the timer object

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
  irsend.begin();
  delay(10);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  int tickEvent = t.every(5000, updateOverWifi);
  int tickEvent2 = t.every(2000, transmitBadge);
    
}

int value = 0;


void transmitBadge()
{
  Serial.println("IR TX");
  irsend.sendSony(thisBadgeInt, 32);
}

void setBadgeLED(int LEDint)
{
  //int levelInt = pow(2,level) - 1;
  //levelInt = pow(2,team+4) + levelInt;
  
  
  digitalWrite(latchPin, LOW);
  // shift out the bits:
  shiftOut(dataPin, clockPin, MSBFIRST, LEDint);  

  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
}

void updateOverWifi()
{
  
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 8080;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/badge/checkin.php?badges=";
  int i;
    for (i = 0; i <= numBadges - 1; i++){
    url += String(badgeList[i], HEX);
    url +=  "|";
  }
  
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    char c = client.read();
    int x = c - '0';
    setBadgeLED(x);
}
}


void dump(decode_results *results) {
  int newBadge = results->value;
  Serial.print("IR RX: ");Serial.println(newBadge,HEX);
  // Dumps out the decode_results structure.
  // Call this after IRrecv::decode()
  int count = results->rawlen;
  if (results->decode_type == SONY) 
  {
    
    if(results->bits == 32)
    {
      int newBadge = results->value;
     // char charBuf[50];
      //String(newBadge).toCharArray(charBuf, 50);
      
      Serial.print("Decoded Badge: ");Serial.println(newBadge,HEX);
      bool seenBadge = false;
      int i = 0;
      for (i = 0; i <= numBadges - 1; i++)
      {
        if(badgeList[i] == newBadge)
        {
          seenBadge = true;
        }
      }

      if(seenBadge == false)
      {
        
        int thisBadgeSpot = numBadges + 1;
        if(thisBadgeSpot == 6)
        {
          thisBadgeSpot = 0;
        }
        badgeList[thisBadgeSpot] = newBadge;
        numBadges++;
        Serial.println("- Adding as new Badge.");
        //Serial.print("List count at:");Serial.println(numBadges);
        
      }
      else
      {
        Serial.println("- Already seen this badge.");
      }
      
      
    }
    
   
    
  }

}


void loop() {
  //delay(5000);

  t.update();
  
  if (irrecv.decode(&results)) {
    dump(&results);
    irrecv.resume(); // Receive the next value
  }
  
  
  
  
}
