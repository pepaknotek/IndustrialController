# Industrial Controller

A small C++ project simulating a simplified industrial monitoring system: a temperature sensor feeds readings into a controller that evaluates the system state (`NORMAL` / `WARNING` / `ERROR`) using hysteresis, and logs every state transition to both the console and a rolling log file.

This is a learning/portfolio project built to practice C++ fundamentals and patterns relevant to embedded software development — interfaces over concrete hardware, state machines, and structured logging — before moving on to real microcontroller targets (ESP32 / STM32).

## Features

- **Sensor abstraction** — `ITemperatureSensor` interface with a `TemperatureSensor` implementation, allowing a simulated sensor today and a real hardware-backed sensor later without changing the rest of the code.
- **Stateful controller** — `Controller` evaluates temperature readings and keeps track of the current `SystemState`, only recomputing on demand via `update()`.
- **Hysteresis** — state transitions use separate thresholds for escalation and recovery, preventing rapid flickering between states when the temperature hovers near a boundary.
- **Change-based logging** — a log entry is written only when the state actually changes (not on every reading), including old state, new state, temperature, and a timestamp.
- **Rolling log file** — the log file keeps only the most recent 10 entries; older entries are automatically dropped.

## Architecture

```
TemperatureSensor (implements ITemperatureSensor)
        |
        | temperature reading
        v
    Controller
        |
        | evaluate(temperature, currentState)
        v
    SystemState
        |
        +-- NORMAL
        +-- WARNING
        +-- ERROR
```

## State machine & hysteresis

| Transition          | Threshold |
|----------------------|-----------|
| NORMAL → WARNING     | temperature ≥ 71 °C |
| WARNING → NORMAL     | temperature < 69 °C |
| WARNING → ERROR      | temperature ≥ 90 °C |
| ERROR → WARNING      | temperature < 88 °C |
| NORMAL → ERROR       | direct escalation possible if temperature ≥ 90 °C |

The gap between the "up" and "down" thresholds prevents the controller from oscillating between two states when the reading sits close to a single boundary.

## Project structure

| File | Responsibility |
|---|---|
| `ITemperatureSensor.h` | Sensor interface |
| `TemperatureSensor.h/.cpp` | Simulated temperature sensor |
| `Controller.h/.cpp` | State machine, hysteresis logic, logging |
| `TimeUtils.h/.cpp` | Timestamp formatting helper |
| `IndustrialController.cpp` | Entry point, wires sensor and controller together |

## Example output

```
Industrial Controller starting...
Temperature: 65
System state: NORMAL
temperature sensor, temperature: 75 ,old state: NORMAL ,new state: WARNING ,date and time: 09.09.2026 15:04:27
Temperature: 75
System state: WARNING
Temperature: 70
System state: WARNING
temperature sensor, temperature: 95 ,old state: WARNING ,new state: ERROR ,date and time: 09.09.2026 15:04:27
Temperature: 95
System state: ERROR
```

## Building

Open `IndustrialController.slnx` in Visual Studio and build/run with `Ctrl+F5`. No external dependencies — standard library only (`<chrono>`, `<fstream>`, `<deque>`, `<sstream>`).

## Roadmap

- [ ] Second simulated sensor (e.g. pressure) combined into a single evaluation
- [ ] Unit tests for `Controller::evaluate` (hysteresis edge cases)
- [ ] Simple serial/text communication protocol
- [ ] Multithreaded control loop
- [ ] Port to real hardware (ESP32 / STM32) behind the existing `ITemperatureSensor` interface
