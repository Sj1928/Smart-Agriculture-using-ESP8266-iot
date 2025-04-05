# 🌱 Smart Agriculture Using ESP8266 and IoT

## 📌 Project Overview
This project enables **precision farming** with **ESP8266**, integrating:
- **Capacitive Soil Moisture Sensor** for soil analysis
- **DHT11 Temperature & Humidity Sensor** for environmental monitoring
- **OLED Display** for local data visualization
- **Blynk IoT** platform for real-time mobile alerts
- **Automated Irrigation Control** based on sensor readings
![IMG_20241121_125145330](https://github.com/user-attachments/assets/15872994-86a2-4cd9-a57e-761a6d910bf3)
![IMG_20241121_125113243](https://github.com/user-attachments/assets/ef274ec3-57b1-4bfd-9d78-4d77dd50fbaf)
![IMG_20241121_024014393](https://github.com/user-attachments/assets/a6374778-82f0-4e47-bec3-bc161cd0e2d8)
Soil Moisture 

https://github.com/user-attachments/assets/51970c74-5513-432e-9184-5879e3ce3f79






## 🔧 Hardware Components
- ESP8266 Wi-Fi Module
- Capacitive Soil Moisture Sensor
- DHT11 Temperature & Humidity Sensor
- OLED Display (128x64)
- Relay Module (for irrigation control)
- Water Pump (optional)
- Jumper wires & power source

## 🛠️ Software & Setup
1. **Install Arduino IDE** & add ESP8266 board support.
2. **Required Libraries**:
   - `ESP8266WiFi.h`
   - `BlynkSimpleEsp8266.h`
   - `Wire.h` for OLED display
   - `Adafruit_GFX.h` & `Adafruit_SSD1306.h`
3. **Blynk Setup**:
   - Create a new Blynk project.
   - Add widgets for sensor data.
   - Obtain **Auth Token** for ESP8266.

## 🚀 Features
- 📡 **IoT Connectivity:** ESP8266 sends sensor data to the Blynk app.
- 📊 **Live Monitoring:** Visualize soil moisture & environmental conditions.
- 🔔 **Alerts & Automation:** Trigger mobile alerts for low soil moisture.
- 💧 **Smart Irrigation:** Control water pump based on sensor thresholds.

## 🖥️ Code Structure

## 📖 Usage Guide
1. Upload the code to ESP8266.
2. Connect sensors as per wiring diagram.
3. Open **Blynk app** to monitor live data.
4. Adjust **moisture thresholds** for irrigation.

## 📜 License
This project is open-source under the **MIT License**. Feel free to modify and enhance!

---

**🔗 Want to contribute?** Fork the repo, make improvements, and submit a PR! Happy coding! 🚜💡
