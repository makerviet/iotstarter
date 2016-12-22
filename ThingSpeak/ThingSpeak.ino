#include <ESP8266WiFi.h>

// API key thingspeak
String apiKey = "4UM3AXNO6QUYAHQO"; //api thingspeak
const char* ssid = "Maker Ha Noi"; 
const char* password = "makerhanoi123";
const char* server = "api.thingspeak.com";
int OutPin = A0; // chân kết nối cảm biến độ ẩm 

WiFiClient client;
  
void setup() {                
  Serial.begin(115200);
   
  WiFi.begin(ssid, password);
 
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
}

void loop() {
  int h1 = analogRead(OutPin);
  //Đẩy dữ lên thingspeak
  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(h1);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
     Serial.print(" Analog value:  ");
     Serial.print(h1);
     Serial.println("% send to Thingspeak");    
  }

  client.stop();
   
  Serial.println("Waiting...");    
  
  delay(5000);
}



