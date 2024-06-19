/*

MQTT Simple Subscribe (GIGA R1 WiFi / Portenta H7)


This sketch demonstrates how to subscribe to a topic

from an MQTT broker (test.mosquitto.org).

Data is NOT encrypted and this sketch is for

prototyping purposes only.


created 23 March 2023

by Karl Söderby

further applied to the coffee machine application by Anke Fischer-Janzen and Kim Zähringer

*/


#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include <Stepper.h>

#define STEPS 2038 // the number of steps in one revolution of your motor (28BYJ-48)
#define STEPS2 200 // numer of steps in one revolution of motor NEMA

// Erzeugung Objekt der Klasse Stepper & Konstruktroraufruf (Übergabe Konstante STEPS und Anschlusspins)
Stepper stepper_driver(STEPS2, 22, 23, 24, 25); 
Stepper stepper_push_button(STEPS, 28, 30, 29 , 31);


///////please enter your sensitive data in the Secret tab/arduino_secrets.h

char ssid[] = "Host_PC";        // your network SSID
char pass[] = "abcdefg";    // your network password


//create the objects

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);


//define broker, port and topic

const char broker[] = "192.168.137.1";

int        port     = 1883;
const char topic[]  = "kaffeesorten";
const char topic1[]  = "variablen";

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
    stepper_driver.setSpeed(60); // 60 rpm
    stepper_driver.step(884.8); // zu Kleiner Kaffee fahren
    delay(1000); // wait for one second
    stepper_push_button.setSpeed(6);
    stepper_push_button.step(815.2); // Taste drücken
    delay(1000); // wait for one second
    stepper_push_button.step(-815.2); // Drücker hochfahren
    delay(1000); // wait for one second
    stepper_driver.step(-884.8); // in Ausgangsposition fahren
  } 
  else if (strcmp(kaffee_msg, "Grosser Kaffee") == 0) {
   // ...
    stepper_driver.setSpeed(60); // 60 rpm
    stepper_driver.step(733.3); // zu Kleiner Kaffee fahren
    delay(1000); // wait for one second
    stepper_push_button.setSpeed(6);
    stepper_push_button.step(815.2); // Taste drücken
    delay(1000); // wait for one second
    stepper_push_button.step(-815.2); // Drücker hochfahren
    delay(1000); // wait for one second
    stepper_driver.step(-733.3); // in Ausgangsposition fahren
  }
  else if (strcmp(kaffee_msg, "Cappuccino") == 0) {
    // ...
    stepper_driver.setSpeed(60); // 60 rpm
    stepper_driver.step(587.85); // zu Kleiner Kaffee fahren
    delay(1000); // wait for one second
    stepper_push_button.setSpeed(6);
    stepper_push_button.step(815.2); // Taste drücken
    delay(1000); // wait for one second
    stepper_push_button.step(-815.2); // Drücker hochfahren
    delay(1000); // wait for one second
    stepper_driver.step(-587.85); // in Ausgangsposition fahren
  }
  else if (strcmp(kaffee_msg, "Latte Macchiato") == 0) {
    // ...
    stepper_driver.setSpeed(60); // 60 rpm
    stepper_driver.step(442.4); // zu Kleiner Kaffee fahren
    delay(1000); // wait for one second
    stepper_push_button.setSpeed(6);
    stepper_push_button.step(815.2); // Taste drücken
    delay(1000); // wait for one second
    stepper_push_button.step(-815.2); // Drücker hochfahren
    delay(1000); // wait for one second
    stepper_driver.step(-442.4); // in Ausgangsposition fahren
  }
  else if (strcmp(kaffee_msg, "Milchschaum") == 0) {
    // ...
    stepper_driver.setSpeed(60); // 60 rpm
    stepper_driver.step(190.9); // zu Kleiner Kaffee fahren
    delay(1000); // wait for one second
    stepper_push_button.setSpeed(6);
    stepper_push_button.step(815.2); // Taste drücken
    delay(1000); // wait for one second
    stepper_push_button.step(-815.2); // Drücker hochfahren
    delay(1000); // wait for one second
    stepper_driver.step(-190.9); // in Ausgangsposition fahren
  }
  else if (strcmp(kaffee_msg, "Heisses Wasser") == 0) {
    // ...
    stepper_driver.setSpeed(60); // 60 rpm
    stepper_driver.step(145.45); // zu Kleiner Kaffee fahren
    delay(1000); // wait for one second
    stepper_push_button.setSpeed(6);
    stepper_push_button.step(815.2); // Taste drücken
    delay(1000); // wait for one second
    stepper_push_button.step(-815.2); // Drücker hochfahren
    delay(1000); // wait for one second
    stepper_driver.step(-145.45); // in Ausgangsposition fahren
  }
  // ... (similar comparisons for other options)
  else {
    Serial.println("There's a problem!");

    //Vorübergehend
    //variable_msg = atoi(kaffee_msg);
  }
  
  //Serial.println(variable_msg); //Vorübergehend

}
