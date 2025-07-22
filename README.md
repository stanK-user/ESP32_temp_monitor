# ESP32 Temperature & Humidity Monitor with Discord Alerts

This project uses an ESP32 microcontroller and a DHT11 sensor to monitor temperature and humidity in near real-time. It features a web application for viewing live sensor data and allows you to set a temperature threshold that triggers Discord notifications when exceeded.

## Features

- Real-time temperature and humidity monitoring via web interface  
- Configurable temperature threshold with Discord alert notifications  
- Optional OLED display integration to show sensor data locally on the device  
- Built using Arduino IDE with commonly used libraries  
- Designed for easy expansion, including plans for a 3D-printed enclosure and dedicated power supply  

## Hardware Required

- ESP32 microcontroller board  
- DHT11 temperature and humidity sensor  
- Optional: small OLED display (e.g., SSD1306)  

## Libraries Used

- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)  
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) (for OLED display)  
- Other Arduino libraries as specified in the sketch  

## Installation & Usage

1. Clone or download this repository.  
2. Open the `.ino` sketch file in the Arduino IDE.  
3. Install the required libraries via the Arduino Library Manager.  
4. Configure your Wi-Fi credentials and Discord webhook URL in the sketch.  
5. Upload the sketch to your ESP32 board.  
6. Connect the DHT11 sensor (and optional OLED display) to the ESP32 as per wiring instructions.  
7. Access the web interface via the ESP32’s IP address to view live sensor data and adjust the temperature threshold.

## Future Plans

- Design and 3D-print a custom enclosure for a polished look  
- Develop a dedicated power supply for standalone operation  

## License

This project is open source under the MIT License. See the [LICENSE](LICENSE) file for details.
