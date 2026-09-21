# ESP32-S3-Touch-LCD-4 (v4) - Modular LVGL Clock Application

This repository contains a clean, modular, and easily extensible **LVGL Clock Application** developed for the **Waveshare ESP32-S3-Touch-LCD-4** development board. 

The project is built upon Waveshare's official `09_LVGL_Widgets` demonstration code but features a decoupled architecture separating hardware control, user interface (UI), and time management.

## 🌟 Features
* **Modular Codebase:** Fully separated concerns (`ui_manager`, `time_manager`, and main application logic) for effortless expansion.
* **Dark UI Theme:** A clean, high-contrast black design featuring crisp white text and UI components.
* **Touch Controls:** Interactive time configuration using native LVGL Rollers and Buttons.
* **Scalable Architecture:** Designed to easily inject external RTC hardware, WiFi/NTP synchronization, or additional application screens.

---

## 📂 Project Structure

```text
09_LVGL_Clock/
├── 09_LVGL_Clock.ino      # Main sketch (Hardware initialization & core loops)
├── ui_manager.h           # UI Architecture & state declarations
├── ui_manager.cpp         # UI implementation, dark theme styling, and screens
├── time_manager.h         # Timekeeping interface definitions
└── time_manager.cpp       # Software-based time tracker (easily swappable with RTC/NTP)
```

---

## ⚙️ Core Architecture & Components

### 1. Main Entrypoint (`09_LVGL_Clock.ino`)
Initializes the board's hardware wrapper via Waveshare's Board Support Package (BSP), initializes the LVGL port wrapper, and continuously drives both the background timekeeper and UI updates.

### 2. Time Manager (`time_manager.h` / `time_manager.cpp`)
A standalone software-based timekeeping engine using `millis()`. 
* **Extensibility Note:** Because this module is completely decoupled from the UI, you can easily replace the internal software logic with hardware RTC chips (e.g., PCF85063A) or WiFi/NTP internet time sync without modifying a single line of UI code.

### 3. UI Manager (`ui_manager.h` / `ui_manager.cpp`)
Handles the dark-themed user interface, layouts, styles, and screen navigation.
* **Main Screen:** Shows the current time (`HH:MM:SS`) in a bold white font centered on a pitch-black background with a settings entry button.
* **Settings Screen:** Provides user-friendly roller wheels for adjusting the hours and minutes, along with a stylized confirmation button to commit changes.

---

## 🛠️ How to Extend this Project

### Adding a New Screen
To add another feature or screen (e.g., a Weather or Sensor display):
1. Declare a new screen pointer in `ui_manager.cpp` (e.g., `static lv_obj_t* weather_scr;`).
2. Construct your new UI layout inside `UIManager::init()`.
3. Add a navigation function like `UIManager::showWeatherScreen()`.
4. Trigger the navigation using a standard LVGL event callback (`LV_EVENT_CLICKED`).

### Hooking up Hardware RTC or WiFi
To update how time is handled, open `time_manager.cpp` and modify `TimeManager::update()` to fetch data from your preferred hardware clock or network service instead of relying on the local system tick counter.

---

## 🚀 Getting Started
1. Clone this repository into your local Arduino IDE or PlatformIO workspace.
2. Ensure you have the official **Waveshare ESP32-S3-Touch-LCD-4 BSP** (`src/bsp/`) available in your project paths.
3. Select your target board (`ESP32S3 Dev Module`) and compile.
4. Flash the binary to the hardware via the USB-C port.
