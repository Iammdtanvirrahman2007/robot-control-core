# Command Message Format

Phase 1 uses a simple line-based protocol over TCP.

Format:

    COMMAND SPEED

Examples:

    FORWARD 50
    BACKWARD 40
    LEFT 30
    RIGHT 30
    STOP 0

Valid commands are intentionally small and deterministic. The ESP32 firmware will validate every message before acting.

## Safety

- Invalid commands must be rejected.
- Speed is limited to 0-100.
- STOP must always be available.
- A communication timeout will later trigger an automatic stop.
