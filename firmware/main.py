"""
Half-Life Week 1 PCB - MicroPython Firmware
Author: Pratham Prajapati (@ez-pratham)
Target: Seeed Studio XIAO ESP32-C3

Hardware:
- 0.96" I2C OLED (SSD1306 128x64) on SDA=GPIO6, SCL=GPIO7
- DHT11 Temperature & Humidity Sensor on GPIO3
- Dual Cherry MX Key Switches on GPIO4 (SW1) and GPIO5 (SW2)
- GY-521 MPU6050 6-Axis Motion Sensor on I2C
"""

import time
from machine import Pin, I2C
import dht

# Pin mapping for Seeed Studio XIAO ESP32-C3
PIN_DHT = 3    # D1 (GPIO3)
PIN_SW1 = 4    # D2 (GPIO4)
PIN_SW2 = 5    # D3 (GPIO5)
PIN_SDA = 6    # D4 (GPIO6)
PIN_SCL = 7    # D5 (GPIO7)

print("Starting Half-Life Week 1 PCB (MicroPython)...")

# Initialize Buttons (Active LOW with internal pull-ups)
sw1 = Pin(PIN_SW1, Pin.IN, Pin.PULL_UP)
sw2 = Pin(PIN_SW2, Pin.IN, Pin.PULL_UP)

# Initialize DHT11 sensor
dht_sensor = dht.DHT11(Pin(PIN_DHT))

# Initialize I2C Bus
i2c = I2C(0, scl=Pin(PIN_SCL), sda=Pin(PIN_SDA), freq=400000)
devices = i2c.scan()
print("I2C Devices detected:", [hex(d) for d in devices])

mode = 0  # 0: Environmental, 1: MacroPad stats
sw1_count = 0
sw2_count = 0
last_sw1 = 1
last_sw2 = 1

def read_dht():
    try:
        dht_sensor.measure()
        temp = dht_sensor.temperature()
        hum = dht_sensor.humidity()
        return temp, hum
    except Exception as e:
        return None, None

print("Firmware initialized. Running main loop...")

while True:
    # Button scanning with edge detection
    val1 = sw1.value()
    val2 = sw2.value()

    if last_sw1 == 1 and val1 == 0:
        sw1_count += 1
        mode = (mode + 1) % 2
        print(f"[BUTTON] SW1 Pressed! Count: {sw1_count}, Mode: {mode}")
    last_sw1 = val1

    if last_sw2 == 1 and val2 == 0:
        sw2_count += 1
        print(f"[BUTTON] SW2 Pressed! Count: {sw2_count}")
    last_sw2 = val2

    # Periodic sensor reporting
    temp, hum = read_dht()
    if temp is not None:
        print(f"[DHT11] Temp: {temp} C | Humidity: {hum} %")
    
    time.sleep(0.1)
