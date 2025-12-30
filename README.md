# ESP32-Based IoT Sensor Monitoring Using MQTT

This project implements a simple IoT-based sensor monitoring system using an ESP32 development board. The system collects environmental and water-quality parameters from multiple sensors and publishes the data in real time using the MQTT protocol. The published data can be visualized live using open-source MQTT visualization tools.

---

## Objectives
- Interface multiple sensors with ESP32  
- Acquire real-time environmental and water-quality data  
- Publish sensor data using MQTT  
- Visualize live data using MQTT clients  

---

## Hardware Components
- ESP32 Dev Module  
- DHT11 (Air Temperature & Humidity)  
- DS18B20 (Water Temperature Sensor)  
- TDS Sensor (Total Dissolved Solids)  
- pH Sensor  
- 4.7kΩ pull-up resistor  
- Breadboard and jumper wires  

---

## Required Libraries

Install the following libraries using **Arduino Library Manager**:

- **WiFi** (ESP32 Core)
- **PubSubClient** by Nick O’Leary
- **DHT sensor library** by Adafruit
- **OneWire** by Paul Stoffregen
- **DallasTemperature** by Miles Burton

### Library Purpose
- WiFi: Enables ESP32 internet connectivity  
- PubSubClient: MQTT publish/subscribe communication  
- DHT: Interface for DHT11 sensor  
- OneWire: Communication protocol for DS18B20  
- DallasTemperature: Temperature handling for DS18B20  

---

## Pin Configuration

### Power Pins
| ESP32 Pin | Usage |
|----------|-------|
| 3.3V | Sensor VCC |
| GND | Common Ground |

### Sensor Connections
| Sensor | Signal Pin | ESP32 Pin |
|-------|-----------|-----------|
| DHT11 | DATA | D4 |
| DS18B20 | DATA | D27 |
| TDS Sensor | Analog Output | D35 (ADC) |
| pH Sensor | Analog Output | D34 (ADC) |

> **Note:** DS18B20 requires a 4.7kΩ pull-up resistor between DATA (D27) and 3.3V.

---

## Sensor Description
- **DHT11:** Measures ambient temperature and humidity using a digital interface  
- **DS18B20:** Measures water temperature using One-Wire protocol  
- **TDS Sensor:** Estimates dissolved solids concentration in ppm  
- **pH Sensor:** Measures acidity/alkalinity of water and is calibrated using drinking water  

---

## MQTT Configuration
- **Broker:** `test.mosquitto.org`  
- **Port:** `1883`  
- **Topic:** `esp32/sensors/data`  
- **Data Format:** JSON  

Example payload:
```json
{
  "air_temp": 23.0,
  "humidity": 46.8,
  "water_temp": 23.9,
  "tds": 76.9,
  "ph": 6.23
}
