#include <Adafruit_GFX.h>      //display
#include <Adafruit_SSD1306.h>  //display
#include <Arduino.h>           //standarad
#include <Wire.h>              //display
#include <esp_task_wdt.h>      //remove or change watchdog

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

// generat table using
// https://daycounter.com/Calculators/Sine-Generator-Calculator.phtml
#define Num_Samples 240  //  number of dample of signal
#define MaxWaveTypes 2   // types of wave (sin=0, tri=1)

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

TaskHandle_t Task1;
TaskHandle_t Task2;

int t = 0;
boolean active = true;  // True -> Output = on
boolean clockwise = false;

double amplitude_sin = 1;
double amplitude_tri = 1;

double phase1 = 0;
double phase2 = Num_Samples / 3;
double phase3 = Num_Samples * 2 / 3;

double freq_sin = 0;  // between 0 & freq_tri
double freq_tri = 1;

unsigned long t_test1 = 0;  // for tests
boolean live1 = true;       // for tests

static byte WaveFormTable[MaxWaveTypes][Num_Samples] = {
    // Sin wave
    {0x3c, 0x3e, 0x3f, 0x41, 0x42, 0x44, 0x45, 0x47, 0x48, 0x4a, 0x4c, 0x4d,
     0x4f, 0x50, 0x52, 0x53, 0x54, 0x56, 0x57, 0x59, 0x5a, 0x5b, 0x5d, 0x5e,
     0x5f, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x68, 0x69, 0x6a, 0x6b, 0x6c,
     0x6d, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x71, 0x72, 0x73, 0x73, 0x74, 0x75,
     0x75, 0x76, 0x76, 0x76, 0x77, 0x77, 0x77, 0x77, 0x78, 0x78, 0x78, 0x78,
     0x78, 0x78, 0x78, 0x78, 0x78, 0x77, 0x77, 0x77, 0x77, 0x76, 0x76, 0x76,
     0x75, 0x75, 0x74, 0x73, 0x73, 0x72, 0x71, 0x71, 0x70, 0x6f, 0x6e, 0x6d,
     0x6d, 0x6c, 0x6b, 0x6a, 0x69, 0x68, 0x66, 0x65, 0x64, 0x63, 0x62, 0x61,
     0x5f, 0x5e, 0x5d, 0x5b, 0x5a, 0x59, 0x57, 0x56, 0x54, 0x53, 0x52, 0x50,
     0x4f, 0x4d, 0x4c, 0x4a, 0x48, 0x47, 0x45, 0x44, 0x42, 0x41, 0x3f, 0x3e,
     0x3c, 0x3a, 0x39, 0x37, 0x36, 0x34, 0x33, 0x31, 0x30, 0x2e, 0x2c, 0x2b,
     0x29, 0x28, 0x26, 0x25, 0x24, 0x22, 0x21, 0x1f, 0x1e, 0x1d, 0x1b, 0x1a,
     0x19, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x10, 0xf,  0xe,  0xd,  0xc,
     0xb,  0xb,  0xa,  0x9,  0x8,  0x7,  0x7,  0x6,  0x5,  0x5,  0x4,  0x3,
     0x3,  0x2,  0x2,  0x2,  0x1,  0x1,  0x1,  0x1,  0x0,  0x0,  0x0,  0x0,
     0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0x1,  0x1,  0x1,  0x2,  0x2,  0x2,
     0x3,  0x3,  0x4,  0x5,  0x5,  0x6,  0x7,  0x7,  0x8,  0x9,  0xa,  0xb,
     0xb,  0xc,  0xd,  0xe,  0xf,  0x10, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
     0x19, 0x1a, 0x1b, 0x1d, 0x1e, 0x1f, 0x21, 0x22, 0x24, 0x25, 0x26, 0x28,
     0x29, 0x2b, 0x2c, 0x2e, 0x30, 0x31, 0x33, 0x34, 0x36, 0x37, 0x39, 0x3a},
    // Triangular wave table
    {0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8,  0x9,  0xa,  0xb,  0xc,
     0xd,  0xe,  0xf,  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
     0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24,
     0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
     0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c,
     0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
     0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54,
     0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60,
     0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c,
     0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
     0x77, 0x76, 0x75, 0x74, 0x73, 0x72, 0x71, 0x70, 0x6f, 0x6e, 0x6d, 0x6c,
     0x6b, 0x6a, 0x69, 0x68, 0x67, 0x66, 0x65, 0x64, 0x63, 0x62, 0x61, 0x60,
     0x5f, 0x5e, 0x5d, 0x5c, 0x5b, 0x5a, 0x59, 0x58, 0x57, 0x56, 0x55, 0x54,
     0x53, 0x52, 0x51, 0x50, 0x4f, 0x4e, 0x4d, 0x4c, 0x4b, 0x4a, 0x49, 0x48,
     0x47, 0x46, 0x45, 0x44, 0x43, 0x42, 0x41, 0x40, 0x3f, 0x3e, 0x3d, 0x3c,
     0x3b, 0x3a, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
     0x2f, 0x2e, 0x2d, 0x2c, 0x2b, 0x2a, 0x29, 0x28, 0x27, 0x26, 0x25, 0x24,
     0x23, 0x22, 0x21, 0x20, 0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18,
     0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0xf,  0xe,  0xd,  0xc,
     0xb,  0xa,  0x9,  0x8,  0x7,  0x6,  0x5,  0x4,  0x3,  0x2,  0x1,  0x0}};

void setup() {
    Serial.begin(115200);
    pinMode(pwm_pin1, OUTPUT);
    pinMode(pwm_pin2, OUTPUT);
    pinMode(pwm_pin3, OUTPUT);

    pinMode(hall_0, INPUT);
    pinMode(hall_1, INPUT);
    pinMode(hall_2, INPUT);
    pinMode(hall_3, INPUT);
    pinMode(hall_4, INPUT);
    pinMode(hall_5, INPUT);

    pinMode(btn, INPUT);  // btn for changing the direction of the motor

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

    // start the first core (Task1)
    xTaskCreatePinnedToCore(
        Task1code,                      /* Task function. */
        "Task1",                        /* name of task. */
        8192, /* Stack size of task */  // 10000 standard
        NULL,                           /* parameter of the task */
        1,                              /* priority of the task */
        &Task1, /* Task handle to keep track of created task */
        0);     /* pin task to core 0 */
    delay(500);

    // start the second core (Task2)
    xTaskCreatePinnedToCore(
        Task2code, /* Task function. */
        "Task2",   /* name of task. */
        8192,      /* Stack size of task */
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

    esp_task_wdt_init(512, false);  // turns off reset on watchdog activation

    int test_length = 1000000;  // for tests

    double sin1;
    double sin2;
    double sin3;

    double tri;

    for (;;) {
        t++;
        testcase_frequency_milion_cycles(
            test_length);  // test case for 1 000 000 cycles

        sin1 = sample_sin(t, amplitude_sin, freq_sin, phase1);
        sin2 = sample_sin(t, amplitude_sin, freq_sin, phase2);
        sin3 = sample_sin(t, amplitude_sin, freq_sin, phase3);

        tri = sample_tri(t, amplitude_tri, freq_tri);

        if (sin1 > tri && active) {
            digitalWrite(pwm_pin1, 1);
        } else {
            digitalWrite(pwm_pin1, 0);
        }

        if (sin2 > tri && active) {
            digitalWrite(pwm_pin2, 1);
        } else {
            digitalWrite(pwm_pin2, 0);
        }

        if (sin3 > tri && active) {
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

    int btn_press = 0;
    int analogValue = 0;
    int old_analogValue = 10000;

    for (;;) {
        if (digitalRead(btn) && btn_press == 0) {
            btn_press = 1;
            Serial.println("BTN (change diraction)");

            double last_freq = freq_sin;
            smooth_freq(0, freq_sin);
            double temp_phase = phase2;
            phase2 = phase3;
            phase3 = temp_phase;
            delay(50);
            Serial.println(" ");
            Serial.println("Changing");
            smooth_freq(last_freq, freq_sin);
            Serial.println(" ");
            if (phase2 == Num_Samples / 3) {
                Serial.println("Moturs");
                clockwise = false;
            } else {
                Serial.println("Medurs");
                clockwise = true;
            }
            delay(200);
        } else if (!digitalRead(btn)) {
            btn_press = 0;
            delay(200);
        }

        analogValue = analogRead(POTENTIOMETER_PIN);

        if (analogValue == 0 && active) {
            old_analogValue = analogValue;
            active = false;
            Serial.println("off");
            smooth_freq(0, freq_sin);
            Serial.print("sin freq: ");
            Serial.println(freq_sin * 660);
            update_display();

        } else if (analogValue == 4095 && old_analogValue != 4095) {
            old_analogValue = analogValue;
            active = true;
            Serial.print("go: ");
            Serial.println("49.5 Hz");
            smooth_freq(0.075, freq_sin);
            Serial.print("sin freq: ");
            Serial.println(freq_sin * 660);
            update_display();

        } else if (analogValue > old_analogValue + 100 ||
                   analogValue < old_analogValue - 100) {
            old_analogValue = analogValue;
            active = true;
            Serial.print("go: ");
            Serial.println(0.000003788 * analogValue - freq_sin, 10);

            Serial.println("0-10 Hz");
            smooth_freq(0.000003788 * analogValue, freq_sin);  // 0-10 Hz
            Serial.print("sin freq: ");
            Serial.println(freq_sin * 660);
            update_display();
        }
        vTaskDelay(20);
    }
}

void smooth_freq(double new_freq, double old_freq) {
    double last_freq;
    while (freq_sin < new_freq - 0.01 || freq_sin > new_freq + 0.01) {
        last_freq = freq_sin;
        if (freq_sin > new_freq) {
            freq_sin -= 0.005;  // deccelerate
        } else if (freq_sin < new_freq) {
            freq_sin += 0.005;  // accelerate
        }
        t = t * last_freq / freq_sin;
        Serial.print(".");
        update_display();
        delay(200);
    }

    int old_index = (int)((t * old_freq)) % Num_Samples;

    t = t * old_freq / new_freq;
    freq_sin = new_freq;

    int new_index = (int)((t * freq_sin)) % Num_Samples;

    if (new_index != old_index && new_index != 0) {
        Serial.print("WARNING!(1) Index_MisMatch!");
        Serial.print("Old_Index: ");
        Serial.print(old_index);
        Serial.print(", New_Index: ");
        Serial.println(new_index);
    }
}

void update_display() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("f=");
    if (!clockwise) {
        display.print("-");
    } else {
        display.print(" ");
    }
    display.print(freq_sin * 660);
    display.print("Hz");
    display.display();
}

double sample_sin(int t, double amplitude, double freq, double phase) {
    t = (int)((t * freq) + phase) % Num_Samples;
    return WaveFormTable[0][(int)t] * amplitude;
}

double sample_tri(int t, double amplitude, double freq) {
    t = (int)(t * freq) % Num_Samples;
    return WaveFormTable[1][(int)t] * amplitude;
}

void testcase_frequency_milion_cycles(int test_length) {
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
        double T = cycle * Num_Samples;
        Serial.println(T, 8);
        Serial.print("frequency triangle wave (kHz)");
        double f = 1 / T;
        Serial.println(f);
        Serial.println("________________________");
        live1 = true;
    }
}

void loop() { vTaskDelay(portMAX_DELAY); }
