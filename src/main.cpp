#include <Arduino.h>

#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "DHT_U.h"

#include <Adafruit_MCP3008.h>

// wifi info:
const char* ssid = "abdo1987";
const char* password = "abdo1987";
const char* serverUrl = "http://localhost:7144/api/sensordata"; // server address

// Sensor info:
DHT dht[8]; // Create an array of 8 DHT sensors
const int numLDR = 8; // The number of LDR sensors
const int numSoilMoisture = 8; // Number of soil moisture sensors

int ldrValues[numLDR];
int soilMoistureValues[numSoilMoisture];

float temperature[8];
float humidity[8];


void setup() {

}

void loop() {

}
