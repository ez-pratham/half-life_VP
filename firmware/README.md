# Half-Life Week 1 PCB - Firmware Guide

This directory contains the firmware for the **Half-Life Week 1: Environmental & Motion MacroPad**, powered by the **Seeed Studio XIAO ESP32-C3**.

---

## 🛠️ Hardware Pinout Mapping

| Component | Function | XIAO Pin | ESP32-C3 GPIO | Description |
| :--- | :--- | :--- | :--- | :--- |
| **DHT11** | Data | `D1` | `GPIO3` | Digital 1-wire bus with 10k pull-up |
| **Cherry MX 1** | Key Switch 1 | `D2` | `GPIO4` | Active LOW (`INPUT_PULLUP`), Mode cycle |
| **Cherry MX 2** | Key Switch 2 | `D3` | `GPIO5` | Active LOW (`INPUT_PULLUP`), Action / Reset |
| **OLED (SSD1306)** | SDA | `D4` | `GPIO6` | I2C Serial Data (0x3C) |
| **OLED (SSD1306)** | SCL | `D5` | `GPIO7` | I2C Serial Clock (0x3C) |
| **MPU6050 IMU** | SDA | `D4` | `GPIO6` | I2C Serial Data (0x68) |
| **MPU6050 IMU** | SCL | `D5` | `GPIO7` | I2C Serial Clock (0x68) |

---

## 🚀 How to Flash via Arduino IDE

### 1. Board Support Setup
1. Open **Arduino IDE** (v2.x recommended).
2. Go to **File > Preferences** and add the following URL to **Additional Boards Manager URLs**:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Open **Tools > Board > Boards Manager**, search for `esp32` by Espressif Systems, and click **Install**.
4. In **Tools > Board > ESP32 Arduino**, select **XIAO_ESP32C3** (or `Seeed Studio XIAO ESP32C3`).

### 2. Install Required Libraries
Open **Tools > Manage Libraries...** and search for & install:
- **Adafruit SSD1306** (by Adafruit)
- **Adafruit GFX Library** (by Adafruit)
- **Adafruit MPU6050** (by Adafruit)
- **Adafruit Unified Sensor** (by Adafruit)
- **DHT sensor library** (by Adafruit)

### 3. Uploading
1. Connect your Seeed Studio XIAO ESP32-C3 to your computer via USB-C.
2. Select the appropriate serial port under **Tools > Port**.
3. Open `firmware.ino` (or `half_life_firmware/half_life_firmware.ino`).
4. Click **Upload** (`Ctrl + U`).
5. Open the **Serial Monitor** at **115200 baud** to view real-time debug telemetry.

---

## 🎮 Interactive Firmware Modes

Press **Switch 1 (SW1)** to cycle through the interactive display views:

1. **Environmental Dashboard**: Displays live ambient temperature (°C or °F) and relative humidity (%) measured by the DHT11 sensor. Press **Switch 2 (SW2)** to toggle between Celsius and Fahrenheit.
2. **Motion & Level Tracker**: Reads 3-axis accelerometer and gyroscope data from the MPU6050 with a live on-screen bubble level indicator.
3. **MacroPad & Key Switch Diagnostics**: Visualizes real-time switch press animations and tracks total click counts for each Cherry MX key. Press **SW2** to reset counters.
4. **System Diagnostics**: Displays chip info, uptime counter, free heap memory, and bus health status.

---

## 🐍 MicroPython Option
If you prefer MicroPython:
1. Flash the ESP32-C3 MicroPython firmware bin.
2. Upload `main.py` using Thonny or `mpremote`.
