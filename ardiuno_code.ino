
/*
MQTT Simple Subscribe (GIGA R1 WiFi / Portenta H7)
This sketch demonstrates how to subscribe to a topic
from an MQTT broker (test.mosquitto.org).
Data is NOT encrypted and this sketch is for
prototyping purposes only.
created 23 March 2023
by Karl Söderby
*/

#include <ArduinoMqttClient.h>
#include <WiFi.h>
//#include "arduino_secrets.h"
#include <Stepper.h>

//Steps definieren für Motorenansteuerung
#define STEPS 2038 // the number of steps in one revolution of your motor (28BYJ-48)

// Erzeugung Objekt der Klasse Stepper & Konstruktroraufruf (Übergabe Konstante STEPS und Anschlusspins)
Stepper stepper_fahren(STEPS, 22, 23, 24, 25); 
Stepper stepper_druecken(STEPS, 28, 29, 30 , 31);

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "Your Mobile Network Name";        // your network SSID
char pass[] = "Your Mobile Network Password";    // your network password

//create the objects
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

//define broker, port and topic
const char broker[] = "Your mobile Network IP";    // Don't forget to change this.
int        port     = 1883;
const char topic[]  = "kaffeesorten";
const char topic1[]  = "variablen";
//char kaffee_msg;

void setup() {

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {

    ; // wait for serial port to connect. Needed for native USB port only

  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);

  }

  Serial.println("You're connected to the network");
  Serial.println();
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);
  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.print("Subscribing to topic: "); //Vorrübergehend
  Serial.println(topic1); //Vorrübergehend
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic);
  mqttClient.subscribe(topic1); //Vorrübergehend

  // topics can be unsubscribed using:

  // mqttClient.unsubscribe(topic);
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Topic: "); //Vorübergehend
  Serial.println(topic1); //Vorübergehend

  Serial.println();

}

void loop() {
  // call poll() regularly to allow the library to receive MQTT messages and

  // send MQTT keep alives which avoids being disconnected by the broker

  mqttClient.poll();

}

char kaffee_msg[50];
int variable_msg;

void onMqttMessage(int messageSize) {
  int index = 0; //EINGEFÜGT

  // we received a message, print out the topic and contents
  Serial.println("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    kaffee_msg[index] = (char)mqttClient.read();
    Serial.print(kaffee_msg[index]);
    index++;
    //variable_msg = (int)mqttClient.read(); //Vorübergehend

  }

  kaffee_msg[index] = '\0'; // Null-terminate the string
  //Serial.println();
  Serial.print("Received message: ");
  Serial.println(kaffee_msg);
  
  // Compare the received string with other strings
  if (strcmp(kaffee_msg, "Kleiner Kaffee") == 0) {
    // ...
    stepper_fahren.setSpeed(6); // 6 rpm
    //stepper.step(2038); // do 2038 steps -- corresponds to one revolution in one minute
    stepper_fahren.step(-variable_msg); // do 2038 steps -- corresponds to one revolution in one minute
    delay(1000); // wait for one second
    stepper_fahren.setSpeed(6); // 6 rpm
    //stepper.step(-2038); // do 2038 steps in the other direction with faster speed -- corresponds to one revolution in 10 seconds
    stepper_fahren.step(variable_msg); // do 2038 steps -- corresponds to one revolution in one minute
  } 
  else if (strcmp(kaffee_msg, "Grosser Kaffee") == 0) {
    stepper_druecken.setSpeed(3); // 3 rpm
    //stepper.step(2038); // do 2038 steps -- corresponds to one revolution in one minute
    stepper_druecken.step(-1019); // do 2038 steps -- corresponds to one revolution in one minute
    delay(1000); // wait for one second
    stepper_druecken.setSpeed(3);
    stepper_druecken.step(1019);
    delay(1000);
  }
  else if (strcmp(kaffee_msg, "Cappuccino") == 0) {
    // ...
  }
  else if (strcmp(kaffee_msg, "Latte Macchiato") == 0) {
    // ...
  }
  else if (strcmp(kaffee_msg, "Milchschaum") == 0) {
    // Handle 'Grosser Kaffee'
  }
  else if (strcmp(kaffee_msg, "Heisses Wasser") == 0) {
    // Handle 'Grosser Kaffee'
  }
  // ... (similar comparisons for other options)
  else {
    //Serial.println("Wrong Name");

    //Vorübergehend
    variable_msg = atoi(kaffee_msg);
  }
  //Serial.println(variable_msg); //Vorübergehend


}
