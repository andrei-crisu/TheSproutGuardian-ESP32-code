// Author: Crisu Radu Andrei 
// 

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include "app_constants.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//create a variable for the DHT sensor
DHT dht(DHTPIN, DHTTYPE);
// Json Variable to Hold Sensor Readings
JSONVar readings;
//

// Timer variables
static unsigned long lastUpdate = 0;

// variables used for monitoring the Sprout
float temperature = 0.0;
float humidity = 0.0;
float luminosity=0.0;
float moisture=0.0;
int pumpStatus=PUMP_OFF;
int lightStatus=LIGHT_ON;



// Get Sensor Readings and return JSON object
String getSensorReadings() {
  readings["temperature"] = temperature;
  readings["humidity"] = humidity;
  readings["luminosity"] = luminosity;
  readings["moisture"] = moisture;
  readings["pumpStatus"]=pumpStatus;
  readings["lightStatus"]=lightStatus;
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ESP32_SSID,ESP32_PASSWORD);
  //WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  //Serial.println(WiFi.localIP());
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();
  initWiFi();

  // Serve the HTML content
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String();
  });

  // Start server
  server.begin();
}

void loop() {

  if (millis() - lastUpdate > READING_INTERVAL) {
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    int moisture_analog_read=analogRead(MOISTUREPIN);
    int luminosity_analog_read=analogRead(LUMINOSITYPIN)
    Serial.println(luminosity_analog_read);
    moisture=100-((float)moisture_analog_read/4095.0)*100;
    lastUpdate = millis();
  }
}