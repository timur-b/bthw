#include <Arduino.h>

constexpr uint8_t RELAY_IN_PIN = 14;
constexpr uint8_t RELAY_OUT_PIN = 13;

constexpr uint8_t ITERATIONS_COUNT = 10;

volatile uint32_t lastChange = 0;

uint32_t loopCounter = 0;
uint32_t sumOn = 0;
uint32_t sumOff = 0;

void ARDUINO_ISR_ATTR gpioIsrHandler(void* arg) {
    lastChange = millis();
}

void setup() {
    Serial.begin(115200);

    pinMode(RELAY_OUT_PIN, INPUT_PULLUP);

    attachInterruptArg(
        RELAY_OUT_PIN,
        &gpioIsrHandler,
        NULL,
        CHANGE
    );

    digitalWrite(RELAY_IN_PIN, LOW);
    pinMode(RELAY_IN_PIN, OUTPUT);
}

void loop() {
    if (loopCounter < ITERATIONS_COUNT) {
        Serial.printf("\nIteration %d:\n\r", loopCounter+1);
    }

    digitalWrite(RELAY_IN_PIN, HIGH);  // Relay ON
    uint32_t now = millis();
    delay(1000);
    if (loopCounter < ITERATIONS_COUNT) {        
        sumOn += lastChange - now;
        Serial.printf("delta (On) : %d\n\r", lastChange - now);
    }

    digitalWrite(RELAY_IN_PIN, LOW);   // Relay OFF
    now = millis();
    delay(1000);
    if (loopCounter < ITERATIONS_COUNT) {        
        sumOff += lastChange - now;
        Serial.printf("delta (Off): %d\n\r", lastChange - now);
    }

    if (loopCounter == ITERATIONS_COUNT - 1) {
        Serial.println("-----------------");
        Serial.printf("Avg time On:  %0.2f ms\n\r", (float)sumOn / ITERATIONS_COUNT);
        Serial.printf("Avg time Off: %0.2f ms\n\r", (float)sumOff / ITERATIONS_COUNT);
    }

    loopCounter++;
}