# Robot Control Core

This is the real transport path for the robot platform:

Browser UI → localhost:8080 C++ bridge → TCP/Wi-Fi → ESP32 → motors/sensors

## 1. Build the laptop bridge

Linux:
```bash
g++ -std=c++17 -O2 -pthread server.cpp -o robot-control-server
./robot-control-server
```

Keep the server running while the dashboard is open.

## 2. Flash the ESP32 body

Open `esp32/robot_control.ino` in Arduino IDE / PlatformIO.

Set:
- `WIFI_SSID`
- `WIFI_PASSWORD`
- motor and sensor GPIOs if your body is wired differently

The firmware exposes TCP port 5000 and reports its hardware description to the laptop. The ESP32 also has a 1.5 second command watchdog, so motors stop when the laptop stops sending commands.

Arduino-ESP32 provides station-mode Wi-Fi and TCP server/client networking through its Wi-Fi/network APIs. citeturn0search0turn0search1

## 3. Start the dashboard

Serve `index.html` over HTTP instead of opening it with `file://`:

```bash
python3 -m http.server 5500
```

Then open:
`http://localhost:5500`

Click **+ Add Robot**, give it a name, enter the ESP32 IP and port `5000`, then **CONNECT**.

## Protocol

The ESP32 accepts one command per TCP connection:
- `PING`
- `GET_HARDWARE`
- `GET_TELEMETRY`
- `COMMAND FORWARD 70`
- `COMMAND BACKWARD 70`
- `COMMAND LEFT 50`
- `COMMAND RIGHT 50`
- `COMMAND STOP 0`

The browser never talks directly to the ESP32. The laptop bridge does it, which avoids browser TCP restrictions and keeps the laptop as the robot brain.
