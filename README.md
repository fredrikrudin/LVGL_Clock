# ESP32-S3-Touch-LCD-4 (v4) - Modular LVGL Clock Application

This repository contains a clean, modular, and easily extensible **LVGL Clock Application** developed for the **Waveshare ESP32-S3-Touch-LCD-4** (V4.0) development board. 

The project features a decoupled architecture that completely separates hardware communication, user interface (UI), and time management into distinct modules for effortless extension.

## 🌟 Features
* **Modular Codebase:** Fully separated concerns (`ui_manager`, `time_manager`, `hardware_manager`, and core sketch logic) for clean development.
* **Dark UI Theme:** A clean, high-contrast black design featuring crisp white text and UI components.
* **Hardware RTC Integration:** Full read/write communication with the onboard **PCF85063A** real-time clock chip using BCD conversion, allowing the device to retain time through power cycles.
* **LiPo Power Monitoring:** Live background queries to the **SW6106** power manager chip, showing an active battery percentage indicator directly on the display.
* **CAN (TWAI) & I2C Subsystems:** Asynchronous background CAN bus reception via the internal ESP32-S3 TWAI driver (500 kbps), alongside automated I2C bus scanners.
* **Touch Controls:** Interactive time configuration using native LVGL Rollers and Buttons.

---

## 📂 Project Structure

```text
LVGL_Clock/
├── LVGL_Clock.ino         # Main sketch (Hardware execution loop & background intervals)
├── hardware_manager.h     # External transceiver & power bus interfaces
├── hardware_manager.cpp   # Implementation for I2C scans, CAN (TWAI), and SW6106 battery metrics
├── ui_manager.h           # UI Architecture & state declarations
├── ui_manager.cpp         # UI implementation, dark theme styling, layout, and screens
├── time_manager.h         # Timekeeping interface definitions
└── time_manager.cpp       # BCD-to-Integer drivers interfacing directly with the PCF85063A RTC
```

---

## ⚙️ Core Architecture & Components

### 1. Main Entrypoint (`LVGL_Clock.ino`)
Initializes the board's core peripheral systems using Waveshare's Board Support Package (BSP), initializes the secondary buses, and maintains the tick rates for the graphics layout engine while triggering secondary telemetry on safe intervals.

### 2. Hardware Manager (`hardware_manager.h` / `hardware_manager.cpp`)
Wraps low-level access pins for the auxiliary features of the version 4.0 layout:
* **Pin 8 & 9 (I2C):** Interfaces with peripheral registers.
* **Pin 19 & 20 (CAN / TWAI):** Connected to the TJA1051T transceiver for differential networking.
* **Address 0x3C (SW6106):** Tracks operational capacity and pack voltage of the connected LiPo element.

### 3. Time Manager (`time_manager.h` / `time_manager.cpp`)
Maintains precise, hardware-backed timekeeping by requesting the epoch stack directly from the **PCF85063A** (`0x51`) during each update loop.

### 4. UI Manager (`ui_manager.h` / `ui_manager.cpp`)
Handles layouts, dark themes, and navigation flow:
* **Main Screen:** Features a large, centralized Montserrat-48 time output, a localized battery level array (`BAT: XX%`), and a transition key to the configurations view.
* **Settings Screen:** Provides high-resolution single-minute stepping wheels to realign the current hour and minute settings, committing modifications directly onto the RTC registers.

---

## 🚀 Getting Started & Dependency Installation

To compile this project, bundle the official source dependencies from the [Waveshare ESP32-S3-Touch-LCD-4 Repository](https://github.com).

### 1. Clone the Repositories
Clone this repository to your local workspace, and download the official Waveshare repository as a source reference.

### 2. Copy the Board Support Package (BSP)
The main sketch relies on the embedded hardware abstraction layers provided by Waveshare. 
* Navigate to `examples/arduino/09_LVGL_Widgets/` inside the official Waveshare repository.
* Copy the entire **`src`** folder (which contains the `bsp` and `lvgl_port` subdirectories).
* Paste the **`src`** folder directly into the root of this project folder (`LVGL_Clock/`).

### 3. Install Bundled Libraries
Waveshare provides specifically tested library versions tailored for this hardware configuration.
* Navigate to `examples/arduino/libraries/` in the official Waveshare repository.
* Copy these libraries into your local Arduino environment directory (typically `Documents/Arduino/libraries/`) or configure them into your PlatformIO environment.

### 4. Build Configuration (Arduino CLI / IDE)
Configure your environment using the following parameters:
* **Board:** `ESP32S3 Dev Module`
* **Core Version:** `esp32:esp32@3.3.11` (or compatible v3.x core)
* **Flash Size:** `16MB`
* **PSRAM:** `OPI`
* **Partition Scheme:** `app3M_fat9M_16MB`
* **USB Mode:** `Hardware CDC and JTAG` / `CDC On Boot: Enabled`

---

## 🛠️ How to Extend this Project

### Adding a New Screen
To add another feature or screen (e.g., a Weather or Sensor display):
1. Declare a new screen pointer in `ui_manager.cpp` (e.g., `static lv_obj_t* weather_scr;`).
2. Construct your new UI layout inside `UIManager::init()`.
3. Add a navigation function like `UIManager::showWeatherScreen()`.
4. Trigger the navigation using a standard LVGL event callback (`LV_EVENT_CLICKED`).

### Expanding CAN Bus Interactions
To transmit telemetry packet blocks or react to touch interfaces across your physical node lines, invoke the internal `twai_transmit()` function directly inside `hardware_manager.cpp` when handling standard interactive interface press states.


## U R L
https://share.google/aimode/MljgL38NZst8s04h4

