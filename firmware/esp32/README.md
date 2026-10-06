# ESP32 Robot Firmware

Phase 1 communication scaffold.

The ESP32 connects to Wi-Fi and listens for line-based TCP messages on port 5000.

Supported message vocabulary:

    STOP 0
    FORWARD 50
    BACKWARD 50
    LEFT 30
    RIGHT 30

At this stage the firmware only validates and acknowledges commands. Motor actuation will be added after the exact motor-driver and wiring are confirmed.

## Setup

1. Open the firmware in Arduino IDE or PlatformIO.
2. Set the Wi-Fi credentials.
3. Flash the ESP32.
4. Open Serial Monitor at 115200 baud.
5. Note the ESP32 IP address.
