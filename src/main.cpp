#include <Arduino.h>

#define LED_BUILTIN 2

#define START_STREAM_COMMAND 's'
#define STOP_STREAM_COMMAND 'f'
#define STREAM_INTERVAL_MS 10
#define LED_BLINK_INTERVAL_MS 100

void setup() {

    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    randomSeed(micros());
}

void loop() {

    static bool isStreaming = false;
    static bool ledState = LOW;
    static uint32_t lastSampleAt = 0;
    static uint32_t lastBlinkAt = 0;

    while (Serial.available() > 0) {

        uint8_t incomingByte = Serial.read();

        if (incomingByte == START_STREAM_COMMAND) {
            isStreaming = true;
            lastSampleAt = 0;
            lastBlinkAt = millis();
            ledState = HIGH;
            digitalWrite(LED_BUILTIN, ledState);
        }
        else if (incomingByte == STOP_STREAM_COMMAND) {
            isStreaming = false;
            ledState = LOW;
            digitalWrite(LED_BUILTIN, ledState);
        }
    }

    if (isStreaming && millis() - lastSampleAt >= STREAM_INTERVAL_MS) {
        lastSampleAt = millis();
        Serial.print(random(0, 101));
        Serial.print("\r\n");
				Serial.flush();
    }

    if (isStreaming && millis() - lastBlinkAt >= LED_BLINK_INTERVAL_MS) {
        lastBlinkAt = millis();
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
    }
}
