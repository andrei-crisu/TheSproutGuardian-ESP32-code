// Author: Crisu Radu Andrei 
// 


#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

// PC hotspot
//const char* PC_SSID = "-----------";
//const char* PC_PASSWORD = "**********";
//ESP32 wifi
const char* ESP32_SSID="Sprout";
const char* ESP32_PASSWORD="TSG2023RKO";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Json Variable to Hold Sensor Readings
JSONVar readings;
//

// Timer variables
static unsigned long lastUpdate = 0;
unsigned long UPDATE_PERIOD = 5000; // 5000 ms (after each 5 seconds)
//define pump status
#define PUMP_OFF 0
#define PUMP_ON 255 // a value diffrent from 0

#define LIGHT_OFF 0
#define LIGHT_ON 255 // a value diffrent from 0

int temperature = 0;
int humidity = 0;
int luminosity=0;
int pumpStatus=PUMP_OFF;
int lightStatus=LIGHT_ON


// Get Sensor Readings and return JSON object
String getSensorReadings() {
  readings["temperature"] = temperature;
  readings["humidity"] = humidity;
  readings["humidity"] = humidity;
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
  Serial.println(WiFi.localIP());
}

// HTML content
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Sensor Data</title>
</head>
<body>
  <h1>ESP32 Sensor Data</h1>
  <p>Temperature: <span id="temperature">0</span></p>
  <p>Humidity: <span id="humidity">0</span></p>

  <script>
    function updateReadings() {
      fetch('/readings')
        .then(response => response.json())
        .then(data => {
          document.getElementById('temperature').innerText = data.temperature;
          document.getElementById('humidity').innerText = data.humidity;
        })
        .catch(error => {
          console.error('Error fetching readings:', error);
        });
    }

    setInterval(updateReadings, 2000);
  </script>
</body>
</html>
)rawliteral";

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
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

  if (millis() - lastUpdate > 2000) {
    temperature++;
    lastUpdate = millis();
  }
}