# BatMobile
**Bachelor's degree thesis project in Mechatronics at KTH Royal Institute of Technology.** 

*Object tracking vehicle using ultrasonic sensors for autonomous following.*

This project utilizes two **HY-SRF05** ultrasonic sensors to enable a 4-wheeled vehicle to track and follow a target. The system is powered by an **Arduino UNO R3**, two **Maxon DC motors**, and an **SG90 servo** for Ackermann steering. For detailed methodology and results, please refer to the full thesis: [Insert DiVA Link Here].

---

## Repository Structure

### /Batmobile

*To run the firmware, the entire `Batmobile` folder must be downloaded and kept intact. Opening the primary file, `Batmobile.ino`, within the Arduino IDE will automatically load and link the associated files containing the system's modular logic.*

* **`Batmobile.ino`**: The primary entry point of the firmware. It handles pin assignments, system initialization (`setup`), and the core execution logic (`loop`).
* **`Sensor.ino`**: Manages data acquisition from the ultrasonic sensors. Includes trigger/echo logic, signal processing via a low-pass filter, and safety-related distance checks.
* **`Control.ino`**: Implements the control theory. It contains the PID-controller algorithms that process sensor feedback into actionable control signals for longitudinal movement.
* **`Actuator.ino`**: The hardware abstraction layer. Defines functions to drive the two Maxon DC motors and the SG90 steering servo.

### /MatLab
*Scripts used for post-experimental data analysis and visualization.*

* **`GraphData.m`**: Used to plot raw sensor data, low-pass filtered signals, control outputs, and loop timing for individual test runs.
* **`GraphResult.m`**: Aggregates data from ten separate trials across two test scenarios. It calculates and visualizes the mean, standard deviation, and grand mean to evaluate system performance.

---

## Dependencies
* **`Servo.h`**: The `Batmobile.ino` sketch requires the standard Arduino Servo library to manage the PWM signals for the steering actuator.

## Authors
* Siim Silman
* John Otterstadh

---
*Stockholm, May 2026*
