#include <Arduino.h>

#define POTENTIOMETER_PIN 33

#define DAC_CH1 25

#define pwm_pin1 32
#define pwm_pin2 26
#define pwm_pin3 27

int dutyCycle;
/* Setting PWM Properties */
const int PWMFreq = 2;  // 5 KHz
const int PWMChannel = 0;
const int PWMResolution = 15;  // might be max 15!
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);

void setup() {
    pinMode(pwm_pin1, OUTPUT);
    ledcSetup(PWMChannel, PWMFreq, PWMResolution);
    /* Attach the LED PWM Channel to the GPIO Pin */
    ledcAttachPin(pwm_pin1, PWMChannel);
}

void loop() {
    /* Increasing the LED brightness with PWM */
    for (dutyCycle = 0; dutyCycle <= MAX_DUTY_CYCLE; dutyCycle++) {
        ledcWrite(pwm_pin1, dutyCycle);
        delay(3);
        // delayMicroseconds(100);
    }
    /* Decreasing the LED brightness with PWM */
    for (dutyCycle = MAX_DUTY_CYCLE; dutyCycle >= 0; dutyCycle--) {
        ledcWrite(pwm_pin1, dutyCycle);
        delay(3);
        // delayMicroseconds(100);
    }
}
