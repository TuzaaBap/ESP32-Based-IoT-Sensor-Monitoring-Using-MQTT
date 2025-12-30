#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

/* ------------ WiFi ------------ */
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

/* ------------ MQTT ------------ */
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* topic = "esp32/sensors/data";

/* ------------ Sensors ------------ */
#define DHTPIN 4
#define DHTTYPE DHT11
#define ONE_WIRE_BUS 27
#define TDS_PIN 35
#define PH_PIN 34

#define VREF 3.3
#define ADC_RESOLUTION 4095.0

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature waterTemp(&oneWire);

/* ------------ WiFi Connect ------------ */
void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

/* ------------ MQTT Reconnect ------------ */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32_SENSOR_CLIENT")) {
      Serial.println("connected");
    } else {
      Serial.println("failed, retrying...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  dht.begin();
  waterTemp.begin();

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  /* ---- Read Sensors ---- */
  float airTemp = dht.readTemperature();
  float humidity = dht.readHumidity();

  waterTemp.requestTemperatures();
  float waterTemperature = waterTemp.getTempCByIndex(0);

  int tdsADC = analogRead(TDS_PIN);
  float tdsVoltage = tdsADC * (VREF / ADC_RESOLUTION);
  float tdsValue = (tdsVoltage * 1000) / 2;

  int phADC = analogRead(PH_PIN);
  float phVoltage = phADC * (VREF / ADC_RESOLUTION);
  float V7 = 1.55;
  float phValue = 7 + (V7 - phVoltage) * 3.0;

  if (phValue < 0) phValue = 0;
  if (phValue > 14) phValue = 14;

  /* ---- JSON Payload ---- */
  String payload = "{";
  payload += "\"air_temp\":" + String(airTemp, 2) + ",";
  payload += "\"humidity\":" + String(humidity, 2) + ",";
  payload += "\"water_temp\":" + String(waterTemperature, 2) + ",";
  payload += "\"tds\":" + String(tdsValue, 2) + ",";
  payload += "\"ph\":" + String(phValue, 2);
  payload += "}";

  client.publish(topic, payload.c_str());

  Serial.println("Published: " + payload);
  delay(2000);
}