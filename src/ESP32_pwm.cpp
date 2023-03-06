#include <Arduino.h>

// #include "driver/gpio.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

#define pi 3.14159265358979323846

#define POTENTIOMETER_PIN 33

#define pwm_pin1 25
#define pwm_pin2 26
#define pwm_pin3 27
#define pwm_pin4 12
#define pwm_pin5 13
#define pwm_pin6 14

TaskHandle_t Task1;
TaskHandle_t Task2;

unsigned long t;

double sin1;
double sin2;
double sin3;

double tri;

double amplitude_sin = 1;
double amplitude_tri = 1;

double phase1 = 0;
double phase2 = -2 * pi / 3;
double phase3 = -4 * pi / 3;
double freq_sin = 0.0005;
double freq_tri = 40;  // minst 10-20kHz (trevlig 20-40kHz) 2=40k?

double ma = amplitude_sin / amplitude_tri;  // Amplitude modulation ratio!
// Vab = Va - Vb
// Vbc = Vb - Vc
// Vca = Vc - Va

int analogValue = 0;

unsigned long t_test1 = 0;  // for tests
unsigned long t_test2 = 0;  // for tests
int count1 = 0;             // for tests
int count2 = 0;             // for tests
boolean live1 = true;       // for tests
boolean live2 = true;       // for tests

void setup() {
  Serial.begin(115200);
  pinMode(pwm_pin1, OUTPUT);
  pinMode(pwm_pin2, OUTPUT);
  pinMode(pwm_pin3, OUTPUT);
  pinMode(pwm_pin4, OUTPUT);
  pinMode(pwm_pin5, OUTPUT);
  pinMode(pwm_pin6, OUTPUT);
  // gpio_pad_select_gpio(pwm_pin1);
  // gpio_set_direction(pwm_pin1, GPIO_MODE_OUTPUT);

  // gpio_pad_select_gpio(pwm_pin2);
  // gpio_set_direction(pwm_pin2, GPIO_MODE_OUTPUT);

  // gpio_pad_select_gpio(pwm_pin2);
  // gpio_set_direction(pwm_pin2, GPIO_MODE_OUTPUT);

  xTaskCreatePinnedToCore(
    Task1code, /* Task function. */
    "Task1",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task1,    /* Task handle to keep track of created task */
    0);        /* pin task to core 0 */
  delay(500);

  xTaskCreatePinnedToCore(
    Task2code, /* Task function. */
    "Task2",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task2,    /* Task handle to keep track of created task */
    1);        /* pin task to core 1 */
  delay(500);
}

void Task1code(void* pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  Serial.print("Task1: ");
  Serial.println("Waves + DigitaWrite");
  for (;;) {
    vTaskDelay(1);
    t = millis();

    /*
      if (count1 == 10000 && live1) {  // for tests
        t_test1 = t;
      } else if (count1 == 20000 && live) {
        t_test1 = t - t_test1;
        live = false;
      }
      count++;

      if (!live1) {  // for tests
        Serial.println(t_test1);
        live1 = true;
      }
    */

    sin1 = sample_sin(t, amplitude_sin, freq_sin, phase1);
    sin2 = sample_sin(t, amplitude_sin, freq_sin, phase2);
    sin3 = sample_sin(t, amplitude_sin, freq_sin, phase3);

    tri = sample_tri(t, amplitude_tri, freq_tri);

    if (sin1 > tri) {
      digitalWrite(pwm_pin4, 1);
      digitalWrite(pwm_pin1, 1);
    } else {
      digitalWrite(pwm_pin1, 0);
      digitalWrite(pwm_pin4, 0);
    }

    if (sin2 > tri) {
      digitalWrite(pwm_pin5, 1);
      digitalWrite(pwm_pin2, 1);
    } else {
      digitalWrite(pwm_pin2, 0);
      digitalWrite(pwm_pin5, 0);
    }

    if (sin3 > tri) {
      digitalWrite(pwm_pin6, 1);
      digitalWrite(pwm_pin3, 1);
    } else {
      digitalWrite(pwm_pin3, 0);
      digitalWrite(pwm_pin6, 0);
    }
  }
}

void Task2code(void* pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  Serial.print("Task2: ");
  Serial.println("AnalogRead");
  Serial.print("sin freq: ");
  Serial.println(freq_sin * 1000);
  Serial.print("tri freq: ");
  Serial.println(freq_tri * 1000);
  for (;;) {
    vTaskDelay(10);
    analogValue = analogRead(POTENTIOMETER_PIN);
    freq_sin = 0.00001 * analogValue;
    Serial.print("sin freq: ");
    Serial.println(freq_sin * 1000);
    vTaskDelay(1000);
  }
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

void loop() {
  vTaskDelay(portMAX_DELAY);
}

