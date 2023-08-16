#include <Arduino.h>

#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "DHT_U.h"
#include <DHTesp.h>
#include <Adafruit_MCP3008.h>

// wifi info:
const char* ssid = "abdo1987";
const char* password = "abdo1987";
const char* serverUrl = "http://localhost:7144/api/sensordata"; // server address

// Sensor info:
DHTesp dht[8]; // Create an array of 8 DHT sensors
const int numLDR = 8; // The number of LDR sensors
const int numSoilMoisture = 8; // Number of soil moisture sensors

int ldrValues[numLDR];
int soilMoistureValues[numSoilMoisture];

float temperature[8];
float humidity[8];

Adafruit_MCP3008 adc; // Create an instance of MCP3008

//previous Time for sensors
unsigned long previousDHTTime = 0;
unsigned long previousLDRTime = 0;
unsigned long previousSoilMoistureTime = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }

  // DHT22 sensors initialization
  for (int i = 0; i < 8; i++) {
    dht[i].setup(4 + i, DHTesp::DHT22);
  }
  
  adc.begin(); // MCP3008 initialization

  Serial.println("Wi-Fi is connected");
}

void loop() {
  unsigned long currentMillis = millis();

// Define the intervals for each type of sensor
  const unsigned long dhtInterval = 900000; // 15 minutes
  const unsigned long ldrInterval = 3600000; // hour
  const unsigned long soilMoistureInterval = 1800000; // half an hour

   // Read and send to the DHT sensor every specified period of time
  if (currentMillis - previousDHTTime >= dhtInterval) {
    previousDHTTime = currentMillis;
    for (int i = 0; i < 8; i++) {
      temperature[i] = dht[i].getTemperature();
      humidity[i] = dht[i].getHumidity();
    }

    // Prepare data to be sent to the server
    String jsonDataDHT = "[";
    for (int i = 0; i < 8; i++) {
      jsonDataDHT += "{\"sensor\": " + String(i) + ", \"temperature\": " + String(temperature[i]) + ", \"humidity\": " + String(humidity[i]) + "}";
      if (i < 7) {
        jsonDataDHT += ",";
      }
    }
    jsonDataDHT += "]";

    // Send DHT data to the server
    HTTPClient httpDHT;
    httpDHT.begin(serverUrl);
    httpDHT.addHeader("Content-Type", "application/json");
    int httpResponseCodeDHT = httpDHT.POST(jsonDataDHT);
    httpDHT.end();

    Serial.print("DHT response code: ");
    Serial.println(httpResponseCodeDHT);
  }
   // Read and send to the LDR sensor every specified period of time
  if (currentMillis - previousLDRTime >= ldrInterval) {
    previousLDRTime = currentMillis;
    for (int i = 0; i < numLDR; i++) {
      ldrValues[i] = adc.readADC(i);
    }
    // Prepare data to be sent to the server
    String jsonDataLDR = "[";
    for (int i = 0; i < numLDR; i++) {
      jsonDataLDR += "{\"sensor\": " + String(i) + ", \"ldr\": " + String(ldrValues[i]) + "}";
      if (i < numLDR - 1) {
        jsonDataLDR += ",";
      }
    }
    jsonDataLDR += "]";

      // Send LDR data to the server
    HTTPClient httpLDR;
    httpLDR.begin(serverUrl);
    httpLDR.addHeader("Content-Type", "application/json");
    int httpResponseCodeLDR = httpLDR.POST(jsonDataLDR);
    httpLDR.end();

    Serial.print("كود الاستجابة للـ LDR: ");
    Serial.println(httpResponseCodeLDR);
    
  }

   // Read and send to the SoilMoisture sensor every specified period of time
  if (currentMillis - previousSoilMoistureTime >= soilMoistureInterval) {
    previousSoilMoistureTime = currentMillis;
    for (int i = 0; i < numSoilMoisture; i++) {
      soilMoistureValues[i] = adc.readADC(8 + i);
    }
    
    // Prepare data to be sent to the server
    String jsonDataSoilMoisture = "[";
    for (int i = 0; i < numSoilMoisture; i++) {
      jsonDataSoilMoisture += "{\"sensor\": " + String(i) + ", \"soilMoisture\": " + String(soilMoistureValues[i]) + "}";
      if (i < numSoilMoisture - 1) {
        jsonDataSoilMoisture += ",";
      }
    }
    jsonDataSoilMoisture += "]";

    // Send SoilMoisture data to the server
    HTTPClient httpSoilMoisture;
    httpSoilMoisture.begin(serverUrl);
    httpSoilMoisture.addHeader("Content-Type", "application/json");
    int httpResponseCodeSoilMoisture = httpSoilMoisture.POST(jsonDataSoilMoisture);
    httpSoilMoisture.end();

    Serial.print("كود الاستجابة لرطوبة التربة: ");
    Serial.println(httpResponseCodeSoilMoisture);
  }
}
