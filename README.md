# Robot Control Core

Centralized multi-robot control platform.

## Architecture

Laptop/PC → Robot Manager → Network → ESP32 → Motor/Sensor hardware

The first milestone is reliable command and status communication with one ESP32 robot. Multi-robot control, sensors, vision, AI planning, localization, and safe-return will be added incrementally.

## Project status

Phase 1: Control protocol and software architecture.
