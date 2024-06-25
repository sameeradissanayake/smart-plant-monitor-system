#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "Virus"; // Change this to your WiFi SSID
const char* password = "bedok@98"; // Change this to your WiFi password

// MQTT Broker
// const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// The default example accepts one data filed named "field1"
// For your own server you can ofcourse create more of them.
int field1 = 0;

int numberOfResults = 3; // Number of results to be read
int fieldNumber = 1; // Field number which will be read out


void setup()
{
    Serial.begin(115200);
    while(!Serial){delay(100);}

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println("******************************************************");
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

    client.setServer(mqtt_server, 1883);
    // client.setCallback(callback);

    
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop(){
  // WiFiClient client;

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // // Convert the value to a char array
  // char tempString[8];
  // dtostrf(temperature, 1, 2, tempString);
  // Serial.print("Temperature: ");
  // Serial.println(tempString);
  // client.publish("esp32/temperature", tempString);
  Serial.println("in loop");

  // Convert the value to a char array
  char tempString[6] = "89892";
  // dtostrf(temperature, 1, 2, tempString);
  // tempString = "89892";
  Serial.print("Temperature: ");
  Serial.println(tempString);
  Serial.println(client.publish("esp332/temperature", tempString));

  client.loop();
  delay(10000);
}
