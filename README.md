# Starbie ⭐
Starbie is a star-shaped digital. I designed for **Hack Club Half-Life (Week 1)**!
It sits on your desk, shows face on the screen, reads the room temperature and humidity, and reacts when you pick it up or tilt it. You can also use the two mechanical keyboard switches to interact with it or use them as shortcut keys.
---
## Photos & Screenshots
### 3D Render
![Starbie 3D Render](images/render_pcb_3d.png)
### Top View
![Starbie Top View](images/render_pcb_top.png)
### Schematic
![Starbie Schematic](images/schematic.svg)
### PCB Layout
![Starbie PCB Layout](images/pcb_layout.svg)
*(Photos of the real assembled board will be added here as soon as my PCBs arrive in the mail!)*
---
## How It Works
### How the parts fit on the star
I placed each part on a different point of the star to keep it balanced:
- **Center**: The 0.96" OLED display sits in the middle as Starbie's face.
- **Top point**: The DHT11 temperature & humidity sensor and its 10k resistor sit here, away from the heat of the microcontroller.
- **Right point**: The Seeed Studio XIAO ESP32-C3 microcontroller sits here with its USB-C port facing outwards so you can easily plug it in.
- **Bottom-right point**: The MPU6050 motion sensor sits here to track tilting and shaking.
- **Left and bottom-left points**: Two Cherry MX mechanical keyswitches sit here so your fingers can easily tap them.
---
## Bill of Materials (BOM)

| Item | Qty | Supplier | Cost / Item | Total Cost | Link |
| :--- | :---: | :--- | :---: | :---: | :--- |
| **Seeed Studio XIAO ESP32-C3** | 1 | UrbanProto | $7.77 | $7.77 | [UrbanProto](https://www.urbanproto.com/part/102010636) |
| **0.96" I2C OLED Display (128x64)** | 1 | Circuitkar | $1.47 | $1.47 | [Circuitkar](https://www.circuitkar.in/products/oled-096-i2c) |
| **GY-521 MPU-6050 Motion Sensor** | 1 | ElectroPi | $2.04 | $2.04 | [ElectroPi](https://www.electropi.in/mpu6050-triple-axis-gyro-accelerometer-module?search=mpu605) |
| **DHT11 Temp & Humidity Sensor** | 1 | Robu | $0.63 | $0.63 | [Robu.in](https://robu.in/product/dht11-digital-relative-humidity-temperature-sensor-module/) |
| **Cherry MX Switches (Board Mount)** | 2 | Mouser | $0.75 | $1.50 | [Mouser](https://www.mouser.in/en/ProductDetail/540-MX2A-E1NB) |
| **10kΩ Resistors (Pack of 10)** | 10 | Robu | $0.01 | $0.10 | [Robu.in](https://robu.in/product/mf25-10k-multicomp-pro-through-hole-resistor-10-kohm-250-mw-%c2%b1-1-axial-leaded-250-v/) |
| **Estimated Shipping** | 1 | — | $0.00 | $0.00 | — |
| **Total** | | | | **$13.51** | |
---
## How to Build
1. **Solder the 10k resistor (R1)** first since it sits flat on the board.
2. **Solder the female pin headers** for the OLED screen (4 pins), MPU6050 (8 pins), and XIAO ESP32-C3 (two 7-pin rows).
3. **Solder the DHT11 sensor**.
4. **Push the two Cherry MX switches** into place and solder their two pins on the back. Put your keycaps on!
5. **Plug the modules in**: Slide the OLED, MPU6050, and XIAO into their headers.
---
## Software
The code is in the [`firmware/`](firmware/) folder.
To upload it using the Arduino IDE:

- Install these libraries from Library Manager:
   - `Adafruit SSD1306`
   - `Adafruit GFX Library`
   - `Adafruit MPU6050`
   - `DHT sensor library`
- Open [`firmware/firmware.ino`](firmware/firmware.ino) and hit **Upload**!
---
## KiCad Files
The project files can be opened in KiCad 10:
- Project file: [`pcb2/pcb2.kicad_pro`](pcb2/pcb2.kicad_pro) (also at [`week1 pratham.kicad_pro`](week1%20pratham.kicad_pro))
- Schematic: [`pcb2/pcb2.kicad_sch`](pcb2/pcb2.kicad_sch)
- PCB layout: [`pcb2/pcb2.kicad_pcb`](pcb2/pcb2.kicad_pcb)
- Gerber files: [`gerbers.zip`](gerbers.zip)
