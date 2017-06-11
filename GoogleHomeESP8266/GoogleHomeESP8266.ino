/*
 * @GoogleHomeESP8266.ino
 * @author: doanthevinh1991@gmail.com
 * @Organization: Maker Hanoi
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define CONTROL_PIN 10

const char* ssid = "MakerHanoi-2-1";
const char* password = "makerhanoi@12345621";
const char* mqtt_server = "test.mosquitto.org"; // Thông tin MQTT Broker Maker Hanoi đang deploy
const char* clientID = "test-sub";
const char* outTopic = "putsomethinghere";
const char* inTopic = "diygooglehomeesp8266";

// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor
const int threshold = 600;

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
int count = 0;

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
  pinMode(CONTROL_PIN, OUTPUT);
}

void callback(char* topic, byte* payload, unsigned int length) {
  char* message = (char*) payload;
//  int value = atoi(message);
//  Serial.println(value);
  if (message[0] == 'o' && message[1] == 'n') {
      digitalWrite(CONTROL_PIN, HIGH);
      Serial.println("HIGH");
  } else if (message[0] == 'o' && message[1] == 'f' && message[2] == 'f') {
    digitalWrite(CONTROL_PIN, LOW);
    Serial.println("LOW");
  }
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
  client.loop();
}

