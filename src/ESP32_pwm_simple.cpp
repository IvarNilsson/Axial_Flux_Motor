#include <Arduino.h>
#include <esp_task_wdt.h>  //remove or change watchdog

#define pi 3.14159265358979323846

#define pwm_pin1 25
#define pwm_pin2 26
#define pwm_pin3 27

#define btn 32
#define POTENTIOMETER_PIN 33

#define hall_0 15
#define hall_1 16
#define hall_2 17
#define hall_3 18
#define hall_4 19
#define hall_5 23

#define SDA 21
#define SCL 22

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

TaskHandle_t Task1;
TaskHandle_t Task2;

unsigned long t;

double sin1;
double sin2;
double sin3;

double tri;

double amplitude_sin = 1;
double amplitude_tri = 0.7;

double phase1 = 0;
double phase2 = -2 * pi / 3;
double phase3 = -4 * pi / 3;
double freq_sin = 0.0005;
double freq_tri = 0.1;  // minst 10-20kHz (trevlig 20-40kHz) 10=10k

double ma = amplitude_sin / amplitude_tri;  // Amplitude modulation ratio!
// Vab = Va - Vb
// Vbc = Vb - Vc
// Vca = Vc - Va

int analogValue = 0;
int old_analogValue = 0;

int test_length = 1000000;
unsigned long t_test1 = 0;  // for tests
unsigned long t_test2 = 0;  // for tests
int count1 = 0;             // for tests
int count2 = 0;             // for tests
boolean live1 = true;       // for tests
boolean live2 = true;       // for tests

int t = 0;

void setup() {
    Serial.begin(115200);
    pinMode(pwm_pin1, OUTPUT);
    pinMode(pwm_pin2, OUTPUT);
    pinMode(pwm_pin3, OUTPUT);

    Serial.println("________________________");
    Serial.print("CPU freq: ");
    Serial.print(getCpuFrequencyMhz());
    Serial.println(" MHz");
    Serial.print("Abp freq: ");
    Serial.print(getApbFrequency() / 1000000);
    Serial.println(" MHz");
    Serial.println("________________________");

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
    Serial.println("________________________");

    esp_task_wdt_init(512, false);

    for (;;) {
        t = millis();
        testcase_frequency_milion_cycles();  // test case for 1 000 000 cycles

        sin1 = sample_sin(t, amplitude_sin, freq_sin, phase1);
        sin2 = sample_sin(t, amplitude_sin, freq_sin, phase2);
        sin3 = sample_sin(t, amplitude_sin, freq_sin, phase3);

        tri = sample_tri(t, amplitude_tri, freq_tri);

        if (sin1 > tri) {
            digitalWrite(pwm_pin1, 1);
        } else {
            digitalWrite(pwm_pin1, 0);
        }

        if (sin2 > tri) {
            digitalWrite(pwm_pin2, 1);
        } else {
            digitalWrite(pwm_pin2, 0);
        }

        if (sin3 > tri) {
            digitalWrite(pwm_pin3, 1);
        } else {
            digitalWrite(pwm_pin3, 0);
        }
    }
}

void Task2code(void* pvParameters) {
    Serial.print("Task2 running on core ");
    Serial.println(xPortGetCoreID());
    Serial.print("Task2: ");
    Serial.println("AnalogRead");
    Serial.println("________________________");
    for (;;) {
        vTaskDelay(10);
        analogValue = analogRead(POTENTIOMETER_PIN);

        if (analogValue < 200) {
            freq_sin = 0.0003;
            old_analogValue = analogValue;
        } else if (analogValue < 0.9 * old_analogValue ||
                   analogValue > 1.1 * old_analogValue) {
            freq_sin = 0.000002 * analogValue;
            old_analogValue = analogValue;
        }

        // Serial.print("sin freq: ");
        // Serial.println(freq_sin * 1000);
        vTaskDelay(1000);
    }
}

double sample_sin(double t, double amplitude, double freq, double phase) {
    return amplitude * sin(2 * pi * freq * t + phase);
}

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
}

void testcase_frequency_milion_cycles() {
    t++;
    if (t == test_length) {
        t_test1 = millis();
    } else if (t == test_length * 2) {
        t_test1 = millis() - t_test1;
        live1 = false;
    }

    if (!live1) {
        Serial.println("________________________");
        Serial.print("delta t (ms) per ");
        Serial.print(test_length);
        Serial.print(" cykler = ");
        Serial.println(t_test1);
        Serial.print("t per cycle (ms) = ");
        double cycle = ((double)t_test1) / ((double)test_length);
        Serial.println(cycle, 8);
        Serial.print("Periodtid T (ms) = ");
        double T = cycle / 10; // hur många cycler per T?
        Serial.println(T, 8);
        Serial.print("frequency triangle wave (kHz)");
        double f = 1 / T;
        Serial.println(f);
        Serial.println("________________________");
        live1 = true;
    }
}

void loop() { vTaskDelay(portMAX_DELAY); }