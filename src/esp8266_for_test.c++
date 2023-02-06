#include <Arduino.h>

int AV = 5;
int BV = 4;
int CV = 0;

int AG = 14;
int BG = 12;
int CG = 13;

void setup() {
    pinMode(AV, OUTPUT);
    pinMode(BV, OUTPUT);
    pinMode(CV, OUTPUT);

    pinMode(AG, OUTPUT);
    pinMode(BG, OUTPUT);
    pinMode(CG, OUTPUT);

    digitalWrite(AV, HIGH);
    digitalWrite(BV, HIGH);
    digitalWrite(CV, HIGH);

    digitalWrite(AG, LOW);
    digitalWrite(BG, LOW);
    digitalWrite(CG, LOW);

    delay(1000);
    digitalWrite(BG, HIGH);
    delay(100);
    digitalWrite(AV, LOW);
    delay(2000);
}

void loop() {
    digitalWrite(CV, HIGH);
    delay(100);
    digitalWrite(AV, LOW);
    delay(2000);

    digitalWrite(BG, LOW);
    delay(100);
    digitalWrite(CG, HIGH);
    delay(2000);

    digitalWrite(AV, HIGH);
    delay(100);
    digitalWrite(BV, LOW);
    delay(2000);

    digitalWrite(CG, LOW);
    delay(100);
    digitalWrite(AG, HIGH);
    delay(2000);

    digitalWrite(BV, HIGH);
    delay(100);
    digitalWrite(CV, LOW);
    delay(2000);

    digitalWrite(AG, LOW);
    delay(100);
    digitalWrite(BG, HIGH);
    delay(2000);
}