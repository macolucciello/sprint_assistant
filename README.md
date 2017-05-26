# sprint_assistant
UCF 2016-2017 Senior Project esp32 code built using esp32 Arduino core (https://github.com/espressif/arduino-esp32). Builds upon basic http server example.

Times a sprint using LDR sensor. neopixel ring and speaker programmed for user feedback.
Prints time on webpage hosted on the esp32 connected to your home network (can be accessed from any device with a webview or browser on the network)
GPIO 34 (ADC): LDR Sensor
GPIO 12: WS2812 neopixel ring
GPIO 25 (DAC): speaker

Changes I want to make:

1. Setup esp32 as access point and setup a socket with the device connected to it.
  - Send data directly instead of hosting a webpage
2. Integrate with an Android app (current app opens a webview connecting to the esp32).
3. Add more elegant timing method with ability to track multiple times with minimal error.
