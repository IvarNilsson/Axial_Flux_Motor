#define HALL_D_PIN 19
#define HALL_A_PIN 18
void setup() {
    pinMode(HALL_D_PIN, INPUT);
    Serial.begin(115200);
}
void loop() {
    Serial.print("Digital: ");
    Serial.print(digitalRead(HALL_D_PIN));
    Serial.print(" - Analog: ");
    Serial.println(analogRead(HALL_A_PIN));
    delay(1000);
}