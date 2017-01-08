/*
 * @MQTT_FreeBoard.ino
 * @author: doanthevinh1991@gmail.com
 * @Organization: Maker Hanoi
 * 
 * This program make ESP8266 used as an MQTT Client
 * connect to mqtt broker and send data to specific topic
 * and display data on the server by using FreeBoard
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SimpleTimer.h>
#include <DHT.h>
#define DHTPIN 02 //pin gpio 02 - D4 in NodeMCU 12E in sensor
#define DHTTYPE DHT22   // DHT 22 Change this if you have a DHT11
DHT dht(DHTPIN, DHTTYPE);
// Thông tin Wifi
const char* ssid = "Maker Ha Noi";
const char* password = "makerhanoi123";

const char* mqtt_server = "dev.iotstarter.vn"; // Thông tin MQTT Broker Maker Hanoi đang deploy

// Thông tin tin clientID, publishing, subcribing topic. ClientID nên là duy nhất trong tất cả các ứng dụng
// Thay đổi thông tin ở đây tương ứng với thông tin các bạn muốn
const char* clientID = "b3098a8f-ce26-40a6-8ea5-761f30531c1e";

const char* outTopic = "public/esp/b3098a8f-ce26-40a6-8ea5-761f30531c1e/device_out";
const char* inTopic = "public/esp/b3098a8f-ce26-40a6-8ea5-761f30531c1e/device_in";

// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
int count = 0;
String send_str;

// Hàm kết nối Wifi
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
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
}

// Hàm call back được gọi khi nhận được thông tin từ subcribing topic
// Trong ứng dụng demo này thì chưa xử lý gì thông tin nhận được
void callback(char* topic, byte* payload, unsigned int length) {
  // do nothing here
  
}

// Reconnect đến MQTT Broker
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, clientID);
      // ... and resubscribe
      client.subscribe(inTopic);
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publishValue() {
//  count++;
//  dtostrf(count, 2, 2, msg);
  float h = dht.readHumidity();
//  Serial.println(h);
  float t = dht.readTemperature();
  dtostrf(t, 2, 2, msg);
//   tmp_str = msg;
  // Format du lieu: "D0;D1;..;D10;A0"
  send_str = "";
  for (int i = 0; i <= 10; i++) {
    send_str+="0;";
    // send_str+= digitalReaad(0).toChar();
  }
  send_str += msg;
  Serial.println(msg);
  send_str.toCharArray(msg, 50);
  client.publish(outTopic, msg);
  delay(2000);
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  publishValue();
  client.loop();
}

