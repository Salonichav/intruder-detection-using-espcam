# Intruder Detection System

This project is a Smart Surveillance and Intruder Detection System built using an **ESP32** microcontroller and an **ESP32-CAM** module. It provides a real-time web-based dashboard that monitors motion, counts intrusions, triggers a local LED alert, and displays a live video feed from the camera.

## Project Overview

The system consists of two main components:
1. **Dashboard Node (ESP32):** Acts as the main controller. It reads data from an Infrared (IR) motion sensor, toggles an LED when motion is detected, keeps track of the number of intrusions, and hosts a web-based dashboard. The dashboard automatically fetches real-time status updates every second and embeds the live video feed.
2. **Camera Node (ESP32-CAM):** Captures real-time video using the OV2640 camera module and streams it over the local network via MJPEG.

## Hardware Requirements

- 1x **ESP32 Development Board**
- 1x **ESP32-CAM Module** (AI-Thinker model) with OV2640 camera
- 1x **IR Motion Sensor** (e.g., HC-SR501 PIR or similar IR obstacle avoidance module)
- 1x **LED** (with an appropriate current-limiting resistor)
- Jumper wires and breadboards
- 5V power supplies for the ESP boards

## Project Structure

```text
Intruder detection/
│
├── dashboard/
│   └── dashboard.ino    # Code for the main ESP32 (Sensor monitoring + Web Dashboard)
│
└── espcam/
    └── espcam.ino       # Code for the ESP32-CAM (Video Streaming)
```

## Pin Configuration and Wiring

### ESP32 Dashboard Node (`dashboard.ino`)
| Component      | ESP32 Pin |
|----------------|-----------|
| IR Sensor (OUT)| GPIO 27   |
| LED (Anode)    | GPIO 26   |

*Note: Connect the IR Sensor VCC to 3.3V/5V and GND to GND. Connect the LED Cathode to GND via a resistor.*

### ESP32-CAM Node (`espcam.ino`)
The camera uses the standard AI-Thinker pin mapping. No external sensors need to be wired to the ESP32-CAM. You will need an FTDI programmer to upload the code to the ESP32-CAM module.

## Setup and Installation

### 1. Network Configuration
Both devices need to connect to the same Wi-Fi network. Open both `.ino` files and update the Wi-Fi credentials to match your network:
```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

### 2. Configure the IP Address for the Camera
The dashboard attempts to load the live camera feed from a specific IP address.
1. Upload the `espcam.ino` code to your ESP32-CAM.
2. Open the Serial Monitor (baud rate `115200`).
3. Note the IP Address assigned to the ESP32-CAM.
4. Open `dashboard.ino` and locate the `index_html` variable.
5. Update the `<img>` tag with the IP address of your ESP32-CAM:
   ```html
   <img src="http://YOUR_ESPCAM_IP/stream">
   ```
   *(Currently, it is hardcoded to `http://172.25.75.208/stream` in the source code.)*

### 3. Uploading Code
- Use the **Arduino IDE** to compile and upload the sketches.
- Ensure you have the ESP32 board package installed in your Arduino IDE.
- For the `dashboard.ino`, select **DOIT ESP32 DEVKIT V1** (or your specific ESP32 board).
- For the `espcam.ino`, select **AI Thinker ESP32-CAM**.

## Usage

1. Power on both the ESP32 and ESP32-CAM modules.
2. Open the Serial Monitor for the **Dashboard Node** to find its IP address.
3. Open a web browser on a device connected to the same Wi-Fi network and navigate to the Dashboard IP address (e.g., `http://192.168.x.x`).
4. The dashboard will display:
   - **Motion Status:** `DETECTED` or `NO MOTION`
   - **LED Status:** `ON` or `OFF`
   - **Security Status:** `ALERT` or `NORMAL`
   - **Intrusion Count:** Number of times motion was detected
   - **Live Camera Feed:** Real-time video stream from the ESP32-CAM.

*When motion is detected by the IR sensor (pin goes LOW), the ESP32 will turn on the LED, register an intrusion, and update the dashboard in real-time.*
