#include <Arduino.h>

constexpr uint8_t LED_PIN = 2;

constexpr char START_STREAM_COMMAND = 's';
constexpr char STOP_STREAM_COMMAND = 'f';

constexpr uint32_t STREAM_INTERVAL_MS = 10;
constexpr uint32_t LED_BLINK_INTERVAL_MS = 100;

constexpr unsigned long SERIAL_BAUD_RATE = 115200;

constexpr long RANDOM_MIN_VALUE = 0;
constexpr long RANDOM_MAX_VALUE_EXCLUSIVE = 101;

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    randomSeed(static_cast<uint32_t>(micros()));
}

void loop()
{
    static bool isStreaming = false;
    static bool ledState = LOW;
    static uint32_t lastSampleAt = 0;
    static uint32_t lastBlinkAt = 0;

    while (Serial.available() > 0)
    {
        const char incomingByte = static_cast<char>(Serial.read());

        if (incomingByte == START_STREAM_COMMAND)
        {
            isStreaming = true;
            lastSampleAt = 0;
            lastBlinkAt = millis();

            ledState = HIGH;
            digitalWrite(LED_PIN, ledState);
        }
        else if (incomingByte == STOP_STREAM_COMMAND)
        {
            isStreaming = false;

            ledState = LOW;
            digitalWrite(LED_PIN, ledState);
        }
    }

    const uint32_t now = millis();

    if (isStreaming && (now - lastSampleAt >= STREAM_INTERVAL_MS))
    {
        lastSampleAt = now;

        Serial.print(random(RANDOM_MIN_VALUE, RANDOM_MAX_VALUE_EXCLUSIVE));
        Serial.print("\r\n");
    }

    if (isStreaming && (now - lastBlinkAt >= LED_BLINK_INTERVAL_MS))
    {
        lastBlinkAt = now;

        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }
}
