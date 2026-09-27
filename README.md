# Industrial Controller

A small C++ project simulating a simplified industrial monitoring system: a temperature sensor feeds readings into a controller that evaluates the system state (`NORMAL` / `WARNING` / `ERROR`) using hysteresis, and logs every state transition. The controller and its state machine now also run unmodified on real Arduino hardware — see [Hardware port](#hardware-port-phase-1) below.

This is a learning/portfolio project built to practice C++ fundamentals and patterns relevant to embedded software development — interfaces over concrete hardware, pluggable logging, state machines, fault handling — as a step toward embedded/automotive software development.

## Features

- **Sensor abstraction** — `ISensor` interface (`readSensor()`) with a `TemperatureSensor` implementation, allowing a simulated sensor on PC and a real hardware-backed sensor (see hardware port) without changing the rest of the code.
- **Pluggable logging** — `ILogger` interface decouples the controller from *how* transitions are recorded. On PC, an unset logger keeps the original behavior (log file + console); on constrained targets (e.g. Arduino), a lightweight `ILogger` implementation can log without `std::string`/`std::ostringstream`.
- **Stateful controller** — `Controller` evaluates temperature readings and keeps track of the current `SystemState`.
- **Hysteresis** — state transitions use separate thresholds for escalation and recovery, preventing rapid flickering between states when the temperature hovers near a boundary.
- **Change-based logging** — a log entry is written only when the state actually changes (not on every reading), including old state, new state, temperature, and a timestamp.
- **Rolling log file** — the log file keeps only the most recent 10 entries; older entries are automatically dropped.
- **Fault handling** — `Controller::forceFaultState()` forces a safe `ERROR` state when a sensor reports a fault (e.g. a disconnected/shorted thermistor) instead of silently evaluating a nonsensical reading. Logs the forced transition with a sentinel value so it's distinguishable from a real threshold crossing.

## Architecture

```
ISensor (readSensor())
        |
        | temperature reading
        v
    Controller ----> ILogger (log transition)
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
| `ISensor.h` | Sensor interface |
| `TemperatureSensor.h/.cpp` | Simulated temperature sensor (PC) |
| `ILogger.h` | Logging interface |
| `Controller.h/.cpp` | State machine, hysteresis logic, fault handling, logging |
| `TimeUtils.h/.cpp` | Timestamp formatting helper (PC only) |
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

Open `IndustrialController.slnx` in Visual Studio and build/run with `Ctrl+F5`. No external dependencies — standard library only (`<chrono>`, `<fstream>`, `<deque>`, `<sstream>`). `Controller.h` includes `ILogger.h`, so make sure `ILogger.h` is added to the project (Add Existing Item) if Visual Studio doesn't pick it up automatically.

## Hardware port (phase 1)

The same `Controller` (state machine, hysteresis, fault handling) now also runs unmodified on a real Arduino Uno, reading an actual NTC thermistor instead of a simulated sensor — see [`faze1-arduino-port/`](./faze1-arduino-port). Highlights:

- Real NTC thermistor on an analog pin, converted to °C via the Steinhart-Hart equation
- Sensor-fault detection (disconnected/shorted thermistor) forces a safe `ERROR` state instead of evaluating a bogus reading — tested and confirmed on hardware, including recovery back through the hysteresis (`ERROR → WARNING → NORMAL`)
- An active buzzer acoustically signals the current state (silent in `NORMAL`, slow beeping in `WARNING`, fast in `ERROR`)
- A relay-driven actuator was evaluated and deliberately dropped for this phase (see the port's own README for why)
- CAN bus reporting (MCP2515) is the current work in progress

## Roadmap

- [x] Pluggable `ILogger` for constrained targets, decoupled from `std::string`
- [x] Fault handling (`forceFaultState`) for a failed/disconnected sensor
- [x] Port to real hardware (Arduino Uno) behind the existing `ISensor` interface — see `faze1-arduino-port/`
- [ ] CAN bus status reporting from the Arduino port (MCP2515 + TJA1050)
- [ ] Second sensor (pressure) fully combined into a single evaluation — state machine and thresholds already exist (`PressureState`/`evaluatePressure`), wiring into logging/hardware is pending a real pressure sensor
- [ ] Unit tests for `Controller::evaluateTemperature` (hysteresis edge cases), enabled by the `ILogger`/`ISensor` abstractions (mockable)
- [ ] Real-time timestamps on the hardware port (DS3231 RTC) instead of `millis()`-since-boot
- [ ] Simple serial/text communication protocol
- [ ] Multithreaded control loop
