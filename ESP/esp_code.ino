#include <ESP8266WiFi.h>
#include "ThingSpeak.h"


const char* ssid = "realme C3";   // your network SSID (name) 
const char* password = "newyork1269";   // your network password

WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "RQ0ROLXSPDG24T1L";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

char *strings[5]; // an array of pointers
char *ptr = NULL;
// Data variables
int CO;
int PM25;
int O3;
int dB;

String rray[];


void setup() {
  Serial.begin(115200);  //Initialize serial

  while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
  
  WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  if (Serial.available()) {
   // Serial.println((Serial.read(string)))
     rray=Serial.readString();
     Serial.println(rray);
  
//   byte index = 0;
//   ptr = strtok(array, ",");  // delimiter
//   while (ptr != NULL)
//   {
//      strings[index] = ptr;
//      index++;
//      ptr = strtok(NULL, ",");
//   }
//  
//   Serial.println("The Pieces separated by strtok()");
//   for (int i = 0; i < index; i++)
//   {
//      Serial.print(i);
//      Serial.print("  ");
//      Serial.println(strings[i]);
//   }
//   
//  // use the atoi() and atof() functions to convert ASCII strings to numbers.
//   CO = atoi(strings[0]); //http://www.cplusplus.com/reference/cstdlib/atoi/?kw=atoi
//   PM25 = atoi(strings[1]);
//   O3 = atoi(strings[2]);
//   dB = atoi(strings[3]); 
//   
//
//   Serial.print("\nCO conc = ");
//   Serial.println(CO);
//   Serial.print("PM2.5 = ");
//   Serial.println(PM25);
//   Serial.print("O3 conc = ");
//   Serial.println(O3);
//   Serial.print("Sound level = ");
//   Serial.println(dB);
//   Serial.println("hello");
//   
//
//  if ((millis() - lastTime) > timerDelay) {
//    
//    // Connect or reconnect to WiFi
//    if(WiFi.status() != WL_CONNECTED){
//      Serial.print("Attempting to connect");
//      while(WiFi.status() != WL_CONNECTED){
//        WiFi.begin(ssid, password); 
//        delay(5000);     
//      } 
//      Serial.println("\nConnected.");
//    }
//    
//    ThingSpeak.setField(1, CO);
//    ThingSpeak.setField(2, PM25);
//    ThingSpeak.setField(3, O3);
//    ThingSpeak.setField(4, dB);
//
//     int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
//
//    if(x == 200){
//      Serial.println("Channel update successful.");
//    }
//    else{
//      Serial.println("Problem updating channel. HTTP error code " + String(x));
//    }
//    lastTime = millis();
//  }
  }
}
