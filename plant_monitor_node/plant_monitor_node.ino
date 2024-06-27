#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 16     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT22


DHT_Unified dht(DHTPIN, DHTTYPE);


// wifi connection
const char* ssid     = "Sam"; // Change this to your WiFi SSID
const char* password = "plth0000"; // Change this to your WiFi password


// MQTT Broker
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

const size_t bufferSize = JSON_OBJECT_SIZE(4);
StaticJsonDocument<bufferSize> doc;  // JSON document for MQTT payload


void setup()
{
    Serial.begin(115200);
    while(!Serial){delay(100);}

    dht.begin();
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println(F("Temperature Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  
    dht.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);

    // connect to WiFi network

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
      // client.subscribe("esp32/output");
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

  doc.clear();  // Clear previous data
  doc["timestamp"] = "2024-04-02 15.33";

  sensors_event_t event;

  // Get temperature
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    doc["temperature"] = event.temperature;
  }
  
  // Get humidity
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    doc["humidity"] = event.relative_humidity;
  }

  // Serialize JSON to a char buffer
  char jsonBuffer[256];  // Adjust buffer size as needed
  serializeJson(doc, jsonBuffer);

  Serial.println(jsonBuffer);
  Serial.println(client.publish("plant_monitor/data", jsonBuffer));

  client.loop();
  delay(10000);
}
