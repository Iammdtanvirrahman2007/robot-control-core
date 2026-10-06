#include <WiFi.h>

#include "robot_config.h"
#include "../../protocol/parser.h"

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

WiFiServer server(5000);
WiFiClient client;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("Robot ID: ");
    Serial.println(robot_config::ROBOT_ID);
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void handle_line(const String& raw) {
    const std::string line(raw.c_str());

    auto command = robot_control::parse_command(line);
    if (!command) {
        client.println("ERR INVALID_COMMAND");
        Serial.println("Rejected: " + raw);
        return;
    }

    Serial.print("Accepted: ");
    Serial.println(robot_control::format_command(*command).c_str());

    // Phase 1: protocol validation + acknowledgement only.
    // Motor-driver actuation will be connected here next.
    client.print("OK ");
    client.println(robot_control::format_command(*command).c_str());
}

void loop() {
    if (!client || !client.connected()) {
        client = server.available();
        if (client) {
            client.setTimeout(100);
            Serial.println("Controller connected.");
        }
        return;
    }

    if (client.available()) {
        String line = client.readStringUntil('\n');
        line.trim();
        if (!line.isEmpty()) {
            handle_line(line);
        }
    }
}
