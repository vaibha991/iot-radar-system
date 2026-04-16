# IoT Radar System

A hobby project that combines an ESP32-CAM and an ultrasonic sensor to build a working radar-style object detection system — visualized in real time using Python and OpenCV.

I put this together to explore how cheap microcontroller hardware can interface with computer vision tools. Turns out you can get a surprisingly decent radar display running for under $10 in parts.

## How it works

The ESP32-CAM streams live video while the ultrasonic sensor continuously measures distances. A Python script reads both inputs and renders a circular radar visualization — objects appear as blips on the display based on their proximity. The closer something is, the more prominent it shows up on screen.

## Components

- ESP32-CAM module
- HC-SR04 Ultrasonic Sensor
- Python 3.x
- OpenCV

## Getting started

Clone the repo and install the Python dependencies:

```bash
git clone https://github.com/your-username/iot-radar-system.git
cd iot-radar-system
pip install -r requirements.txt
```

Flash the ESP32-CAM with the firmware in `/esp32` using Arduino IDE, then run the visualization:

```bash
python radar.py
```

## Demo

Full demo on LinkedIn — [link here]

## What's next

- Add a servo motor for a proper sweeping angle
- Improve object tracking accuracy
- Build a web-based dashboard instead of a local OpenCV window

## Author

Vaibhav Bhatt — [GitHub](https://github.com/vaibha991) · [LinkedIn](https://www.linkedin.com/in/vaibhav-bhatt-b39b08340/)