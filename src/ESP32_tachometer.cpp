#include <Adafruit_GFX.h>      //display
#include <Adafruit_SSD1306.h>  //display
#include <Arduino.h>           //standarad
#include <Wire.h>              //display

#define HALL_D_PIN 33

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const byte PulsesPerRevolution = 2;  // antal magneter

const unsigned long ZeroTimeout = 10000000;  

const byte numReadings = 1;  // 1 for no avrage 2 for smooth rpm

volatile unsigned long LastTimeWeMeasured;
volatile unsigned long PeriodBetweenPulses = ZeroTimeout + 1000;
volatile unsigned long PeriodAverage = ZeroTimeout + 1000;
unsigned long FrequencyRaw;
unsigned long FrequencyReal;
unsigned long RPM;
unsigned int PulseCounter = 1;

unsigned long PeriodSum;

unsigned long LastTimeCycleMeasure = LastTimeWeMeasured;
unsigned long CurrentMicros = micros();

unsigned int AmountOfReadings = 1;

unsigned int ZeroDebouncingExtra;

// Variables for smoothing tachometer:
unsigned long readings[numReadings];  // The input.
unsigned long readIndex;              // The index of the current reading.
unsigned long total;                  // The running total.
unsigned long average;  // The RPM value after applying the smoothing.

void setup()  // Start of setup:
{
    Serial.begin(115200);
    attachInterrupt(digitalPinToInterrupt(HALL_D_PIN), Pulse_Event, FALLING);

    delay(1000);

    // OLED 0.96" Display:
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setRotation(0);
    display.setTextWrap(false);
    display.dim(0);
}

void loop() {
    LastTimeCycleMeasure = LastTimeWeMeasured;
    CurrentMicros = micros();

    if (CurrentMicros < LastTimeCycleMeasure) {
        LastTimeCycleMeasure = CurrentMicros;
    }

    // Calculate the frequency:
    FrequencyRaw = 10000000000 / PeriodAverage;
    if (PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra ||
        CurrentMicros - LastTimeCycleMeasure >
            ZeroTimeout - ZeroDebouncingExtra) {
        FrequencyRaw = 0;
        ZeroDebouncingExtra = 2000;
    } else {
        ZeroDebouncingExtra = 0;
    }

    FrequencyReal = FrequencyRaw / 10000;

    RPM = FrequencyRaw / PulsesPerRevolution * 60;
    RPM = RPM / 10000;

    // Smoothing RPM:
    total = total - readings[readIndex];
    readings[readIndex] = RPM;
    total = total + readings[readIndex];
    readIndex = readIndex + 1;

    if (readIndex >= numReadings) {
        readIndex = 0;
    }

    average = total / numReadings;

    
      // Print information on the serial monitor:
      Serial.print("Period: ");
      Serial.print(PeriodBetweenPulses);
      Serial.print("\tReadings: ");
      Serial.print(AmountOfReadings);
      Serial.print("\tFrequency: ");
      Serial.print(FrequencyReal);
      Serial.print("\tRPM: ");
      Serial.print(RPM);
      Serial.print("\tTachometer: ");
      Serial.println(average);
    

    char string[10];  // Create a character array of 10 characters

    dtostrf(average, 6, 0, string);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("RPM:");

    // Print variable with right alignment:
    display.setCursor(0, 20);
    display.println(string);

    display.display();
}

void Pulse_Event() {
    PeriodBetweenPulses = micros() - LastTimeWeMeasured;

    LastTimeWeMeasured = micros();

    if (PulseCounter >= AmountOfReadings) {
        PeriodAverage = PeriodSum / AmountOfReadings;
        PulseCounter = 1;
        PeriodSum = PeriodBetweenPulses;

        int RemapedAmountOfReadings =
            map(PeriodBetweenPulses, 40000, 5000, 1, 10);

        RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);
        AmountOfReadings = RemapedAmountOfReadings;
    } else {
        PulseCounter++;
        PeriodSum = PeriodSum + PeriodBetweenPulses;
    }
}
