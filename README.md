# Random UART Stream

Firmware for an ESP32 development board that generates a continuous stream of random numeric data over the USB serial port. The stream is controlled by simple start and stop command bytes, making it useful for testing serial readers, data loggers, parsers, dashboards, plotting tools, or any host application that needs a predictable UART data source.

## What It Does

The firmware waits for commands on the serial port at `115200` baud:

| Command byte | ASCII character | Action |
| --- | --- | --- |
| `0x73` | `s` | Start streaming random values |
| `0x66` | `f` | Stop streaming |

When streaming is enabled, the board sends one random integer from `0` to `100` every `10 ms`.

Each sample is sent as ASCII text followed by carriage return and line feed:

```text
42\r\n
7\r\n
100\r\n
```

The built-in LED on GPIO `2` blinks every `100 ms` while the stream is active, giving a visual indication that data generation is running.

## Hardware

- ESP32 development board compatible with the PlatformIO `esp32dev` board target
- USB cable for programming and serial communication

The project uses the Arduino framework through PlatformIO.

## Project Configuration

The current PlatformIO environment is defined in `platformio.ini`:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
```

## How to Build

Install PlatformIO, then build the firmware from the project root:

```sh
pio run
```

## How to Upload

Connect the ESP32 over USB and upload the firmware:

```sh
pio run --target upload
```

If PlatformIO does not detect the port automatically, specify it explicitly:

```sh
pio run --target upload --upload-port COM3
```

Replace `COM3` with the serial port used by your board.

## How to Use

Open a serial monitor at `115200` baud:

```sh
pio device monitor --baud 115200
```

Send the character `s` to start the stream. The board will begin printing random values:

```text
15
83
4
61
```

Send the character `f` to stop the stream.

Most serial terminal programs can be used as long as they allow sending raw characters or text over the serial port. Examples include PlatformIO Serial Monitor, Arduino Serial Monitor, PuTTY, Tera Term, RealTerm, or a custom Python/Node/C# application.

## Serial Protocol

- Baud rate: `115200`
- Data format: ASCII decimal integer
- Value range: `0` to `100`
- Line ending: `\r\n`
- Sample interval: `10 ms`
- Start command: `s`
- Stop command: `f`

The firmware checks for incoming serial bytes continuously. Any received `s` byte enables streaming, and any received `f` byte disables it. Other bytes are ignored.

## Example Host-Side Test

Using Python and `pyserial`, you can start the stream, read a few lines, and stop it again:

```python
import serial
import time

port = "COM3"  # Change this to your ESP32 serial port

with serial.Serial(port, 115200, timeout=1) as ser:
    ser.write(b"s")

    start = time.time()
    while time.time() - start < 2:
        line = ser.readline().decode("ascii", errors="replace").strip()
        if line:
            print(line)

    ser.write(b"f")
```

## Customization

The main behavior is controlled by constants in `src/main.cpp`:

```cpp
#define START_STREAM_COMMAND 's'
#define STOP_STREAM_COMMAND 'f'
#define STREAM_INTERVAL_MS 10
#define LED_BLINK_INTERVAL_MS 100
```

You can change these values to use different command bytes, adjust the sample rate, or modify the LED blink speed.

The generated random value range is defined here:

```cpp
Serial.print(random(0, 101));
```

For example, change it to `random(0, 1024)` to generate values from `0` to `1023`.

## Repository Structure

```text
.
├── platformio.ini
├── src/
│   └── main.cpp
├── include/
├── lib/
└── test/
```

## Notes

- The stream is not started automatically after reset. A start command must be sent first.
- The random generator is seeded with `micros()` during startup.
- `Serial.flush()` is called after each sample to wait for outgoing serial data to be transmitted.
