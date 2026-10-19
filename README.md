# Starbie

Starbie is a digital pet designed in the shape of a star with a microcontroller, switches, and sensors.

![Starbie 3D Render](render_pcb_3d.png)

---

## What is it?

It is designed to be your digital pet which shows temperature and humidity, and reacts to moving and tilting.

The board contains a microcontroller, sensors for humidity and temperature, motion sensing, and a few tactile buttons.

---

## Why?

It is my **Hack Club's Week 1** project.

---

## Project Status

Its PCB design, firmware, and required files are included. It is completed with all functions working and verified:
- **0 DRC Violations**: Clean KiCad 10 design rule check.
- **Components 100% Inside Board**: MPU-6050, OLED, Cherry MX switches, and DHT11 fit within the star outline.
- **Tested Firmware**: Supports environmental sensing, tilt reactions, and interactive display modes.

---

## Software

The firmware runs on the **Seeed Studio XIAO ESP32-C3** microcontroller and can be compiled using the Arduino IDE:

- **Firmware Location**: [`firmware/firmware.ino`](firmware/firmware.ino)
- **Supported Platforms**: Arduino IDE (ESP32 core) & MicroPython ([`firmware/main.py`](firmware/main.py))
- **Required Arduino Libraries**:
  - `Adafruit SSD1306` & `Adafruit GFX Library` (for OLED display)
  - `Adafruit MPU6050` (for accelerometer and gyroscope motion tracking)
  - `DHT sensor library` (for DHT11 temperature & humidity readings)

---

## Repository Content

| Folder / File | Description |
| :--- | :--- |
| **`pcb2/`** | KiCad PCB design files (`pcb2.kicad_pcb`), schematic, and project settings |
| **`week1 pratham/`** | KiCad project workspace and schematic files |
| **`firmware/`** | Complete Arduino C++ and MicroPython firmware source code |
| **`fabrication/`** | Production-ready Gerber and NC drill files |
| **`gerbers.zip`** | Pre-packaged Gerber archive ready to order from JLCPCB / PCBWay |
| **`3D Models/`** | 3D STEP models for all components (MPU-6050, OLED, Cherry MX, XIAO, DHT11) |
| **`images/`** | High-resolution top, bottom, and 3D isometric board renders |
| **`bom.csv`** | Complete Bill of Materials |
