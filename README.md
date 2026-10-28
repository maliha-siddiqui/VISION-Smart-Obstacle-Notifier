# VISION — Smart IoT-Based Obstacle Notifier

VISION is an IoT-based prototype designed to help visually impaired users detect nearby obstacles. It uses an ultrasonic sensor to measure distance and a buzzer to provide alerts. The firmware also supports optional Wi-Fi connectivity and Blynk reporting.

## Features

* Obstacle distance measurement using an HC-SR04 ultrasonic sensor.
* Buzzer alerts based on measured distance.
* A simple approaching-object speed estimate.
* Optional Wi-Fi connectivity and Blynk updates.
* Offline operation when Wi-Fi is unavailable.

## Hardware Requirements

* ESP8266 / NodeMCU development board
* HC-SR04 ultrasonic sensor
* Buzzer
* Jumper wires
* Suitable power supply

Check the source code for pin assignments and verify your wiring before building.

## Software Requirements

* Arduino IDE
* ESP8266 board support package
* Blynk library (optional, for Blynk features)

## Setup Instructions

1. Install Arduino IDE and the ESP8266 board support package.
2. Install the Blynk library if you intend to use Blynk features.
3. Open `VISION_FINAL_WITH_WIFI.cpp` in Arduino IDE. You may need to copy or rename it as an `.ino` sketch.
4. Replace the placeholder Wi-Fi and Blynk credentials with your own values locally.
5. Configure the appropriate Blynk template and event settings if using Blynk.
6. Select the correct board and port, then upload and test the firmware.

**Security:** Never commit real Wi-Fi passwords, Blynk authentication tokens, or other secrets to GitHub. Rotate any credentials that may have been exposed.

## Project Status

This repository documents an academic prototype developed during electronics and communication engineering training. Its performance has not been independently validated for safety-critical navigation. It should not replace established mobility aids such as a white cane or guide dog.

## Demo

See `vision-prototype-output.jpeg` for the project output.

## Credits

Academic project developed as part of electronics and communication engineering training. Contributors, mentors, and institution details can be added with their agreement.
