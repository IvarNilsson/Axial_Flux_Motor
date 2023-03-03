#include <Arduino.h>

#define pi 3.14159265358979323846

void setup() {
  Serial.begin(115200);
}

int pwm_pin1 = 25; // check refrence for good pins
int pwm_pin2 = 25;
int pwm_pin3 = 25;



void loop() {
  
}

double sample_sin(double t, double amplitude, double freq, double phase) {
    return amplitude * sin(2 * pi * freq * t + phase);
};

double sample_tri(double t, double amplitude, double freq) {
    double res = 0.0;
    double fullPeriodTime = 1.0 / freq;
    double localTime = fmod(t, fullPeriodTime);

    double value = localTime / fullPeriodTime;

    if (value < 0.25) {
        res = value * 4;
    } else if (value < 0.75) {
        res = 2.0 - (value * 4.0);
    } else {
        res = value * 4 - 4.0;
    }

    return amplitude * res;
};

// ESP32 has two 8-bit DAC (digital to analog converter) channels, connected to GPIO25 (Channel 1) and GPIO26 (Channel 2)
// Square wave   = amplitude . sin(x) + sin(3.x) / 3 +  sin (5.x) / 5 + sin (7.x) / 7  + sin (9.x) / 9  + sin (11.x) / 11  Odd harmonics
// Triangle wave = amplitude . sin(x) - 1/3^2.sin(3.x) +  1/5^2.sin(5.x) - 1/7^2.sin (7.x) + 1/9^2.sin(9.x) - 1/11^2.sin (11.x) Odd harmonics
// dacWrite(pin, int(128 + 80 * (sin(deg*PI/180)))); // GPIO Pin mode (OUTPUT) is set by the dacWrite function
// dacWrite(pin, int(128 + 80 * (sin(deg*PI/180)+sin(3*deg*PI/180)/3+sin(5*deg*PI/180)/5+sin(7*deg*PI/180)/7+sin(9*deg*PI/180)/9+sin(11*deg*PI/180)/11))); // Square
// dacWrite(pin, int(128 + 80 * (sin(deg*PI/180)+1/pow(3,2)*sin(3*deg*PI/180)+1/pow(5,2)*sin(5*deg*PI/180)+1/pow(7,2)*sin(7*deg*PI/180)+1/pow(9,2)*sin(9*deg*PI/180)))); // Triangle
