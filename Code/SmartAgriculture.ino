#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <BlynkSimpleEsp8266.h>  // Include the Blynk library
#include <ESP8266WebServer.h>    // Web server library
#include <ArduinoJson.h>         // For handling JSON in communication

// Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL3ZJfxVt6q"
#define BLYNK_TEMPLATE_NAME "Smart farm"
#define BLYNK_AUTH_TOKEN "L-suSTMDZLolWsz1nF8tCvSHwxZbadq7"  // Replace with your token

// Pin definitions
#define DHTPIN 14               // DHT sensor pin (GPIO14)
#define MOISTURE_SENSOR_PIN A0   // Soil moisture sensor pin (A0)
#define PUMP_PIN 2             // Pin controlling the relay for the pump
#define BUZZER_PIN 12           // Pin controlling the buzzer (GPIO12)

// Wi-Fi credentials
const char* ssid = "your_SSID";     // Replace with your Wi-Fi SSID
const char* password = "your_PASSWORD"; // Replace with your Wi-Fi password

// Sensor and display variables
unsigned long previousMillis = 0;  // Stores the last time we updated the sensor readings
const long interval = 10000; // Interval for reading sensors and updating display (10 seconds)
DHT dht(DHTPIN, DHT11);       // DHT sensor
Adafruit_SSD1306 display(128, 64, &Wire, -1);  // OLED display

// Sensor data
float humidity = 0;
float temperature = 0;
int moistureLevel = 0;
int moisturePercentage = 0;

// Web server
ESP8266WebServer server(80);

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // Print IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  // Start DHT sensor
  dht.begin();

  // Initialize OLED display
  if (!display.begin(SSD1306_PAGEADDR, 0x3C)) {
    Serial.println(F("SSD1306 OLED allocation failed"));
    for (;;);  // Stay here if display fails to initialize
  }
  display.clearDisplay();

  // Set pin modes for pump and buzzer
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, HIGH);  // Relay off (active-low)
  digitalWrite(BUZZER_PIN, LOW);

  // Setup Web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/pump/on", HTTP_GET, handlePumpOn);
  server.on("/pump/off", HTTP_GET, handlePumpOff);
  server.on("/buzzer/on", HTTP_GET, handleBuzzerOn);
  server.on("/buzzer/off", HTTP_GET, handleBuzzerOff);
  server.on("/getdata", HTTP_GET, handleGetData);  // Endpoint for fetching data in JSON format

  // Start the web server
  server.begin();
}

void loop() {
  // Handle Blynk events
  Blynk.run();

  // Handle web server requests
  server.handleClient();

  // Check if it's time to update the sensor readings (using millis)
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Save the last time we updated the sensor readings
    previousMillis = currentMillis;

    // Read values from DHT sensor
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    // Read moisture sensor value (0-1023 range)
    moistureLevel = analogRead(MOISTURE_SENSOR_PIN);

    // Map the moisture value to a percentage (0 = dry, 100 = wet)
    moisturePercentage = map(moistureLevel, 400, 1023, 100, 0); // Wet soil = 100%, Dry soil = 0%

    // Constrain the moisture percentage to the range 0-100
    moisturePercentage = constrain(moisturePercentage, 0, 100);

    // Send sensor data to Blynk app
    Blynk.virtualWrite(V1, humidity);      // Send humidity value to Virtual Pin V1
    Blynk.virtualWrite(V0, temperature);   // Send temperature value to Virtual Pin V0
    Blynk.virtualWrite(V2, moisturePercentage); // Send moisture level to Virtual Pin V2
  }
}

// Handle the root page (dashboard)
void handleRoot() {
  String html = "<html>";
  html += "<head><title>Smart Farm Dashboard</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 0; padding: 0; background: #f4f4f9; color: #333; text-align: center;}";
  html += "h1 { color: #4CAF50; font-size: 24px;}";
  html += ".container { max-width: 100%; margin: 0 auto; padding: 10px; background: #fff; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);}";
  html += ".card { background: #f9f9f9; padding: 10px; margin: 10px 0; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);}";
  html += ".button { padding: 12px 24px; border: none; border-radius: 5px; background: #4CAF50; color: white; cursor: pointer; font-size: 18px; transition: background 0.3s ease; width: 90%; margin: 5px 0;}";
  html += ".button:hover { background: #45a049; }";
  html += "@media only screen and (max-width: 600px) {";
  html += "h1 { font-size: 20px; }";
  html += ".container { padding: 20px; }";
  html += ".card { padding: 15px; }";
  html += ".button { font-size: 16px; }";
  html += "}";
  html += "</style></head>";
  html += "<body><div class='container'><h1>Smart Farm Dashboard</h1>";
  html += "<div class='card'><p>Humidity: <span id='humidity'>" + String(humidity) + " %</span></p>";
  html += "<p>Temperature: <span id='temperature'>" + String(temperature) + " &#8451;</span></p>";
  html += "<p>Soil Moisture: <span id='moisture'>" + String(moisturePercentage) + " %</span></p>";
  html += "</div>";

  html += "<div class='card'>";
  html += "<button class='button' onclick='controlPump(\"on\")'>Turn Pump ON</button>";
  html += "<button class='button' onclick='controlPump(\"off\")'>Turn Pump OFF</button>";
  html += "</div>";

  html += "<div class='card'>";
  html += "<button class='button' onclick='controlBuzzer(\"on\")'>Turn Buzzer ON</button>";
  html += "<button class='button' onclick='controlBuzzer(\"off\")'>Turn Buzzer OFF</button>";
  html += "</div>";

  html += "<div class='card'>";
  html += "<button class='button' onclick='updateData()'>Refresh Data</button>";
  html += "</div>";
  
  html += "</div><script>";
  html += "function controlPump(state) { fetch('/pump/' + state); }";
  html += "function controlBuzzer(state) { fetch('/buzzer/' + state); }";
  html += "function updateData() { fetch('/getdata').then(response => response.json()).then(data => {";
  html += "document.getElementById('humidity').innerText = data.humidity + ' %';";
  html += "document.getElementById('temperature').innerText = data.temperature + ' °C';";
  html += "document.getElementById('moisture').innerText = data.moisture + ' %';";
  html += "}); }";
  html += "</script></body></html>";
  
  server.send(200, "text/html", html);
}

// Handle Pump ON request
void handlePumpOn() {
  digitalWrite(PUMP_PIN, LOW); // Turn on the pump (LOW for active-low relay)
  Serial.println("Pump is ON");
  server.sendHeader("Location", "/");
  server.send(303);
}

// Handle Pump OFF request
void handlePumpOff() {
  digitalWrite(PUMP_PIN, HIGH); // Turn off the pump (HIGH for active-low relay)
  Serial.println("Pump is OFF");
  server.sendHeader("Location", "/");
  server.send(303);
}

// Handle Buzzer ON request
void handleBuzzerOn() {
  digitalWrite(BUZZER_PIN, HIGH);  // Turn on the buzzer
  Serial.println("Buzzer is ON");
  server.sendHeader("Location", "/");
  server.send(303);
}

// Handle Buzzer OFF request
void handleBuzzerOff() {
  digitalWrite(BUZZER_PIN, LOW);  // Turn off the buzzer
  Serial.println("Buzzer is OFF");
  server.sendHeader("Location", "/");
  server.send(303);
}

// Handle get data in JSON format
void handleGetData() {
  String json = "{";
  json += "\"humidity\": " + String(humidity) + ",";
  json += "\"temperature\": " + String(temperature) + ",";
  json += "\"moisture\": " + String(moisturePercentage);
  json += "}";
  server.send(200, "application/json", json);
}
