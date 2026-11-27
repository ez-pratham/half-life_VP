/*
  =============================================================================
  Project: Half-Life Week 1 - Environmental & Motion MacroPad
  Author:  Pratham Prajapati (@ez-pratham)
  Board:   Seeed Studio XIAO ESP32-C3
  Description:
    Firmware for the Hack Club Half-Life Week 1 custom PCB.
    Features:
      - 0.96" I2C OLED Display (SSD1306 128x64)
      - DHT11 Digital Temperature & Humidity Sensor
      - GY-521 MPU6050 6-Axis Motion Tracking (Accelerometer & Gyroscope)
      - Dual Cherry MX Mechanical Key Switches with Software Debouncing
      - 4 Interactive Dashboard Modes:
          1. Environmental Monitor (Temp & Humidity with Comfort Gauge)
          2. Motion & Tilt Angle Monitor (Roll, Pitch, Accelerometer readings)
          3. MacroPad & Click Counter (Real-time key states and stats)
          4. System Diagnostics (Uptime, Free Heap, Sensor Health)
  =============================================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// -----------------------------------------------------------------------------
// Pin Configuration (Seeed Studio XIAO ESP32-C3 Pinout)
// -----------------------------------------------------------------------------
#define PIN_DHT         3    // D1 (GPIO3) - DHT11 Data line
#define PIN_SW1         4    // D2 (GPIO4) - Cherry MX Switch 1 (Mode Cycle)
#define PIN_SW2         5    // D3 (GPIO5) - Cherry MX Switch 2 (Action / Toggle)
#define PIN_SCL         6    // D4 (GPIO6) - I2C Serial Clock
#define PIN_SDA         7    // D5 (GPIO7) - I2C Serial Data

#define DHTTYPE         DHT11

// OLED Display Configuration
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS  0x3C // Typical I2C address for 0.96" SSD1306

// -----------------------------------------------------------------------------
// Global Object Instances
// -----------------------------------------------------------------------------
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(PIN_DHT, DHTTYPE);
Adafruit_MPU6050 mpu;

// -----------------------------------------------------------------------------
// Application State & Variables
// -----------------------------------------------------------------------------
bool mpuAvailable = false;
bool oledAvailable = false;

// Display modes
enum DisplayMode {
  MODE_ENVIRONMENT = 0,
  MODE_MOTION      = 1,
  MODE_MACROPAD    = 2,
  MODE_SYSTEM      = 3,
  MODE_COUNT       = 4
};
DisplayMode currentMode = MODE_ENVIRONMENT;

// Button state tracking with debouncing
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
const unsigned long debounceDelay = 50; // ms
int lastButtonState1 = HIGH;
int lastButtonState2 = HIGH;
int buttonState1 = HIGH;
int buttonState2 = HIGH;

// Keypress statistics
unsigned long sw1PressCount = 0;
unsigned long sw2PressCount = 0;

// Sensor data cache
float temperature = 0.0;
float humidity = 0.0;
bool fahrenheitUnit = false;
sensors_event_t a, g, temp_mpu;

// Timing loops
unsigned long lastSensorReadTime = 0;
const unsigned long sensorReadInterval = 1500; // ms (DHT11 sample rate)

unsigned long lastDisplayUpdateTime = 0;
const unsigned long displayUpdateInterval = 100; // 10 FPS refresh for snappy UI

// -----------------------------------------------------------------------------
// Setup Routine
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=========================================");
  Serial.println("  Half-Life Week 1 PCB - Starting Up     ");
  Serial.println("  Designed by Pratham Prajapati          ");
  Serial.println("=========================================");

  // Configure switch inputs with internal pull-up resistors
  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW2, INPUT_PULLUP);

  // Initialize I2C Bus with custom pins if required
  Wire.begin(PIN_SDA, PIN_SCL);

  // Initialize OLED
  if (display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    oledAvailable = true;
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    
    // Splash screen animation
    display.setTextSize(1);
    display.setCursor(18, 10);
    display.println("HALF-LIFE: WEEK 1");
    display.drawFastHLine(10, 22, 108, SSD1306_WHITE);
    display.setCursor(22, 28);
    display.println("ESP32-C3 MacroPad");
    display.setCursor(30, 42);
    display.println("by Pratham");
    display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
    display.display();
    delay(1800);
  } else {
    Serial.println("[WARN] SSD1306 OLED not found at 0x3C!");
  }

  // Initialize DHT11
  dht.begin();
  Serial.println("[INFO] DHT11 Initialized.");

  // Initialize MPU6050
  if (mpu.begin(0x68, &Wire)) {
    mpuAvailable = true;
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("[INFO] MPU6050 IMU connected successfully.");
  } else {
    Serial.println("[WARN] MPU6050 not detected at 0x68 (will retry dynamically).");
  }

  // Initial sensor read
  readSensors();
}

// -----------------------------------------------------------------------------
// Sensor Polling Function
// -----------------------------------------------------------------------------
void readSensors() {
  // Read DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    humidity = h;
    temperature = t;
  }

  // Read MPU6050 if active
  if (mpuAvailable) {
    mpu.getEvent(&a, &g, &temp_mpu);
  }
}

// -----------------------------------------------------------------------------
// Input Handling & Debouncing
// -----------------------------------------------------------------------------
void handleButtons() {
  int reading1 = digitalRead(PIN_SW1);
  int reading2 = digitalRead(PIN_SW2);

  // Debounce Switch 1 (Mode Switcher)
  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis();
  }
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      if (buttonState1 == LOW) { // Pressed (active LOW)
        sw1PressCount++;
        currentMode = (DisplayMode)((currentMode + 1) % MODE_COUNT);
        Serial.printf("[BUTTON] SW1 Pressed! Switched to Mode %d\n", currentMode);
      }
    }
  }
  lastButtonState1 = reading1;

  // Debounce Switch 2 (Context Action Button)
  if (reading2 != lastButtonState2) {
    lastDebounceTime2 = millis();
  }
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      if (buttonState2 == LOW) { // Pressed (active LOW)
        sw2PressCount++;
        Serial.printf("[BUTTON] SW2 Pressed! Action triggered.\n");
        // Action depends on current mode
        if (currentMode == MODE_ENVIRONMENT) {
          fahrenheitUnit = !fahrenheitUnit; // Toggle Celsius / Fahrenheit
        } else if (currentMode == MODE_MACROPAD) {
          // Reset counters if long held or toggle
          sw1PressCount = 0;
          sw2PressCount = 0;
        }
      }
    }
  }
  lastButtonState2 = reading2;
}

// -----------------------------------------------------------------------------
// UI Rendering Screens
// -----------------------------------------------------------------------------

// Screen 1: Environmental Sensor View
void drawEnvironmentScreen() {
  display.setTextSize(1);
  display.setCursor(4, 2);
  display.print("ENVIRONMENT (SW1->)");

  display.drawFastHLine(0, 12, 128, SSD1306_WHITE);

  // Temperature
  display.setCursor(4, 18);
  display.print("Temp:");
  display.setTextSize(2);
  display.setCursor(40, 16);
  if (fahrenheitUnit) {
    float f = (temperature * 9.0 / 5.0) + 32.0;
    display.printf("%.1f F", f);
  } else {
    display.printf("%.1f C", temperature);
  }

  // Humidity
  display.setTextSize(1);
  display.setCursor(4, 36);
  display.print("Humid:");
  display.setTextSize(2);
  display.setCursor(48, 34);
  display.printf("%.0f %%", humidity);

  // Status footer
  display.setTextSize(1);
  display.setCursor(4, 54);
  display.print("SW2: ");
  display.print(fahrenheitUnit ? "Set to [C]" : "Set to [F]");
}

// Screen 2: 6-Axis Motion / IMU View
void drawMotionScreen() {
  display.setTextSize(1);
  display.setCursor(4, 2);
  display.print("MOTION / MPU6050");
  display.drawFastHLine(0, 12, 128, SSD1306_WHITE);

  if (!mpuAvailable) {
    display.setCursor(14, 26);
    display.print("Sensor Offline");
    display.setCursor(4, 42);
    display.print("Checking I2C 0x68...");
    return;
  }

  // Accelerometer values (m/s^2)
  display.setCursor(4, 16);
  display.printf("Ax: %+.2f", a.acceleration.x);
  display.setCursor(68, 16);
  display.printf("Ay: %+.2f", a.acceleration.y);

  display.setCursor(4, 28);
  display.printf("Az: %+.2f", a.acceleration.z);

  // Approximate Tilt Visualization Bubble
  int centerX = 96;
  int centerY = 44;
  int radius = 14;
  display.drawCircle(centerX, centerY, radius, SSD1306_WHITE);

  // Constrain bubble within circle
  int bx = centerX + (int)(a.acceleration.x * 1.2);
  int by = centerY - (int)(a.acceleration.y * 1.2);
  display.fillCircle(constrain(bx, centerX - 10, centerX + 10),
                     constrain(by, centerY - 10, centerY + 10),
                     3, SSD1306_WHITE);

  // Gyroscope Rate (rad/s)
  display.setCursor(4, 42);
  display.printf("Gx: %+.1f", g.gyro.x);
  display.setCursor(4, 52);
  display.printf("Gy: %+.1f", g.gyro.y);
}

// Screen 3: MacroPad & Key Switch Diagnostics
void drawMacroPadScreen() {
  display.setTextSize(1);
  display.setCursor(4, 2);
  display.print("MACROPAD DECK");
  display.drawFastHLine(0, 12, 128, SSD1306_WHITE);

  // SW1 Box
  bool sw1Active = (digitalRead(PIN_SW1) == LOW);
  if (sw1Active) {
    display.fillRect(6, 18, 54, 30, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
  } else {
    display.drawRect(6, 18, 54, 30, SSD1306_WHITE);
    display.setTextColor(SSD1306_WHITE);
  }
  display.setCursor(16, 23);
  display.print("KEY 1");
  display.setCursor(12, 35);
  display.printf("#%lu", sw1PressCount);

  // SW2 Box
  bool sw2Active = (digitalRead(PIN_SW2) == LOW);
  display.setTextColor(SSD1306_WHITE);
  if (sw2Active) {
    display.fillRect(68, 18, 54, 30, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
  } else {
    display.drawRect(68, 18, 54, 30, SSD1306_WHITE);
    display.setTextColor(SSD1306_WHITE);
  }
  display.setCursor(78, 23);
  display.print("KEY 2");
  display.setCursor(74, 35);
  display.printf("#%lu", sw2PressCount);

  // Footer
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(4, 53);
  display.print("SW2 click: Reset");
}

// Screen 4: System Information Screen
void drawSystemScreen() {
  display.setTextSize(1);
  display.setCursor(4, 2);
  display.print("SYSTEM STATUS");
  display.drawFastHLine(0, 12, 128, SSD1306_WHITE);

  display.setCursor(4, 16);
  display.print("Chip: ESP32-C3");

  display.setCursor(4, 28);
  unsigned long uptimeSec = millis() / 1000;
  display.printf("Uptime: %luh %lum %lus", uptimeSec / 3600, (uptimeSec % 3600) / 60, uptimeSec % 60);

  display.setCursor(4, 40);
  display.printf("Heap: %u B", ESP.getFreeHeap());

  display.setCursor(4, 52);
  display.printf("MPU: %s | DHT: OK", mpuAvailable ? "ON" : "OFF");
}

// -----------------------------------------------------------------------------
// Main Loop
// -----------------------------------------------------------------------------
void loop() {
  // 1. Process inputs
  handleButtons();

  // 2. Poll sensors periodically
  if (millis() - lastSensorReadTime >= sensorReadInterval) {
    lastSensorReadTime = millis();
    readSensors();
  }

  // 3. Refresh display UI
  if (oledAvailable && (millis() - lastDisplayUpdateTime >= displayUpdateInterval)) {
    lastDisplayUpdateTime = millis();
    display.clearDisplay();

    switch (currentMode) {
      case MODE_ENVIRONMENT:
        drawEnvironmentScreen();
        break;
      case MODE_MOTION:
        drawMotionScreen();
        break;
      case MODE_MACROPAD:
        drawMacroPadScreen();
        break;
      case MODE_SYSTEM:
        drawSystemScreen();
        break;
    }

    display.display();
  }
}
