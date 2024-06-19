#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define ledPin 2

const char* ssid = "Host_PC";
const char* password = "abcdefg";
const char* mqtt_server = "192.168.137.1";
const char* espclientID = "ESP02"; 

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const char* topic = "button_blau";

bool done = true;
bool runEffect = false;
bool lastState = LOW;

int effectNext = 0;
int effectState = 0;
int effect[] = {
  500,
  500,
  500,
  500
};

unsigned long previousMillis = 0;

//Deep Sleep
const unsigned long inactivityThreshold = 1800000UL; // 30 minutes in milliseconds
unsigned long lastActivityTime = 0;

void setup_wifi() {

    delay(1000);
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

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(espclientID)) {
            Serial.println("connected");
            client.publish(topic, "1");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 0.5 seconds");
            delay(500);
        }
    }
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    //client.setCallback(callback);

    //Deep Sleep
    lastActivityTime = millis(); // Initialize the last activity time

    // Check if waking up from deep sleep
    if (ESP.getResetReason() == "Deep-Sleep Wake") {
      Serial.println("Woke up from deep sleep!");
      lastActivityTime = millis(); // Reset the last activity time
    }
}

void loop() {

    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    long now = millis();

  // Check if the inactivity period exceeds 30 minutes
  if (millis() - lastActivityTime >= inactivityThreshold) {
    Serial.println("No activity for 30 minutes. Going to sleep...");
    ESP.deepSleep(0);
  }
}
