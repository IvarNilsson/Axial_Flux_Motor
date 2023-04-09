#include <Adafruit_GFX.h>      //display
#include <Adafruit_SSD1306.h>  //display
#include <Arduino.h>           //standarad
#include <Wire.h>              //display
#include <esp_task_wdt.h>      //remove or change watchdog

#define pwm_pin1 25
#define pwm_pin2 26
#define pwm_pin3 27

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int t = 0;

void setup() {
    Serial.begin(115200);
    pinMode(pwm_pin1, OUTPUT);
    pinMode(pwm_pin2, OUTPUT);
    pinMode(pwm_pin3, OUTPUT);

    // Address 0x3D for 128x64
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("ERROR 1");
        Serial.println(F("SSD1306 allocation failed"));
        Serial.println("ERROR 1");
        for (;;) {
        }
    }
    delay(2000);
    display.clearDisplay();
    // can chage font here :)
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Hello, ");
    display.setCursor(0, 20);
    display.println("world!");
    display.display();

    // Print some nice data
    Serial.println("________________________");
    Serial.print("CPU freq: ");
    Serial.print(getCpuFrequencyMhz());
    Serial.println(" MHz");
    Serial.print("Abp freq: ");
    Serial.print(getApbFrequency() / 1000000);
    Serial.println(" MHz");
    Serial.println("________________________");
}

void loop() {
    t++;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(t);
    display.display();

    digitalWrite(pwm_pin1, 1);
    delay(1000);
    digitalWrite(pwm_pin1, 0);
    delay(100);

    digitalWrite(pwm_pin2, 1);
    delay(1000);
    digitalWrite(pwm_pin2, 0);
    delay(100);

    digitalWrite(pwm_pin3, 1);
    delay(1000);
    digitalWrite(pwm_pin3, 0);
    delay(100);
}