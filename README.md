# VISION — Smart IoT-Based Obstacle Notifier

VISION is a prototype designed to help visually impaired users detect nearby obstacles. It uses an ultrasonic sensor to measure distance and a buzzer to provide alerts. The firmware also includes optional Wi-Fi/Blynk reporting when a network connection is available.

## Features implemented in the included firmware
- Distance measurement using an HC-SR04 ultrasonic sensor.
- Buzzer alerts based on measured distance.
- A simple approaching-object speed estimate.
- Optional Wi-Fi connection and Blynk updates/event reporting.
- Offline fallback when Wi-Fi is unavailable.

## Hardware mentioned in the source
- ESP8266 / NodeMCU development board
- HC-SR04 ultrasonic sensor
- Buzzer
- Jumper wires and suitable power supply

Check your actual wiring and hardware revision before building. The code pin assignments are documented in the source file.

## Libraries and setup
1. Install the ESP8266 board support package in Arduino IDE.
2. Install the Blynk library if you intend to use Blynk features.
3. Open `src/VISION_FINAL_WITH_WIFI.cpp` in Arduino IDE (you may need to rename/copy it as an `.ino` sketch matching the folder name).
4. Replace the placeholder values for `BLYNK_AUTH_TOKEN`, `ssid`, and `pass` with your own credentials locally. **Never commit real credentials to GitHub.**
5. Configure the matching Blynk template and event settings for your own Blynk project.
6. Select the correct ESP8266 board and port, then upload and test.

## Safety and privacy
The source has been prepared with credential placeholders. Do not upload Wi-Fi passwords, Blynk tokens, or other secrets. If a token or password was previously exposed, rotate it before using the project publicly.

## Project status
This repository documents an academic prototype. Its performance and detection reliability have not been independently validated for real-world navigation or safety-critical use. It should not replace a white cane, guide dog, or other established mobility aid.

## Demo image
See `images/vision-prototype-output.jpeg`.

## Credits
Academic project developed as part of electronics and communication engineering training. Add contributors, mentor, and institution details only with their agreement.
