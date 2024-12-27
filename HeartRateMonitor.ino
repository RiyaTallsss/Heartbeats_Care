#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <MAX30100_PulseOximeter.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

PulseOximeter pox;

void setup() {
  Serial.begin(9600);
  
  // Initialize OLED
  if(!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  // Initialize MAX30100
  if (!pox.begin()) {
    Serial.println("Could not find a valid MAX30100 sensor, check wiring!");
    while (1);
  }

  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();

  if (millis() - lastBeat > 2000) {
    // Display heart rate on OLED every 2 seconds
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Heart Rate: ");
    display.print(pox.getHeartRate());
    display.display();
    lastBeat = millis();
  }
}

void onBeatDetected() {
  Serial.println("Beat!");
}
