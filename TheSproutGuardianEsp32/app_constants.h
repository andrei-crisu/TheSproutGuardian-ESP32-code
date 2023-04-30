#ifndef APP_CONSTANTS_H
#define APP_CONSTANTS_H

//other libraries
#include <DHT.h>


// PC hotspot
//const char* PC_SSID = "-----------";
//const char* PC_PASSWORD = "**********";
//ESP32 wifi
const char* ESP32_SSID="Sprout";
const char* ESP32_PASSWORD="TSG2023RKO";

const unsigned long UPDATE_PERIOD = 5000; // 5000 ms (after each 5 seconds)
//define pump status
#define PUMP_OFF 0
#define PUMP_ON 255 // a value diffrent from 0

#define LIGHT_OFF 0
#define LIGHT_ON 255 // a value diffrent from 0

#define DHTPIN 26
#define DHTTYPE DHT11

#define MOISTUREPIN 39
#define LUMINOSITYPIN 36

#define READING_INTERVAL 2000

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



#endif