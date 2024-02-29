#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define ledPin 2


const char* ssid = "Your Mobile Hotspot Name";                            //Change this 
const char* password = "Your Mobile Hotspot Password";                    //Change this 
const char* mqtt_server = "Your Mobile Hotspot IP";  //Change this. you can find it out by typing ipconfig in the cmd.  


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


const char* topic = "button_1";


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


void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}


void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP8266Client")) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish(topic, "1");
            // ... and resubscribe
            //client.subscribe("inTopic");
        } else {
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


    long now = millis();
    if (now - lastMsg > 10000) {
        lastMsg = now;
        ++value;
        snprintf (msg, 75, "still alive", value);
        Serial.print("Publish message: ");
        Serial.println(msg);
        client.publish(topic, msg);
    }


    //Implementation of Reset
    String data = ESP.getResetInfo();
    Serial.println("Reset was pressed");
    if(data == "Deep-Sleep Wake") {
    //we have been reset, not just plugged in for the first time
      if(!runEffect) { //if run effect is true, then we've already done this - don't do it again!
        client.publish(topic, "Button 1 was pressed");
        done = false;
        runEffect = true;
        effectState = 0;
      }
    } else if(data == "Power On") {
      //checkForUpdates(); //check for firmware updates when the device fully reboots. Powercycling everything will cause this to happen
    }


    if(runEffect) {
      unsigned long currentMillis = millis();
      if(effectState < (sizeof(effect) / sizeof(effect[0]))) {
        if(currentMillis - previousMillis >= effectNext) {
          if(lastState == LOW) {
            lastState = HIGH;
          } else {
            lastState = LOW;
          }
  
          digitalWrite(ledPin, lastState);
          effectNext = effect[effectState];
          effectState++;
          previousMillis = currentMillis;
          Serial.println(effectState);
        }
      } else {
      //the effect is done, mark everything as completed
        done = true;
      }
    }


    if(done) {
      //we are done, sleep now until the button is pushed
      ESP.deepSleep(0);
    }
}

