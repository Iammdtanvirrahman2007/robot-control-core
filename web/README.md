# Web Control Panel

The root index.html is currently a browser-only UI prototype.

Intended architecture:

    Browser
       |
       | HTTP / WebSocket
       v
    Host Control API
       |
       v
    RobotManagerBridge
       |
       | TCP
       v
    ESP32 robots

The browser should not connect directly to the ESP32 network. The host API will validate commands and forward them through the Robot Manager.
