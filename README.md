# ESP32_Web_Server_Example
Example for ESP 32 project in VSCode for creating a web server that has some simple IO controls

## First Time Setup

+ Install VSCode https://code.visualstudio.com/Download
+ In VSCode, install the PlatformIO extention https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide

## Project Setup

+ Clone the project and open the folder in VSCode
+ Build the file system image using Platform IO "Project Tasks", Run "esp32dev" -> "Platform" -> "Build Filesystem Image"
+ Upload the file system image to the ESP32 using Platform IO "Project Tasks", Run "esp32dev" -> "Platform" -> "Upload Filesystem Image"
+ Open main.cpp and update the "ssid" and "password" strings to match your WiFi credentials
+ Build the software using Platform IO "Project Tasks", Run "esp32dev" -> "General" -> "Build"
+ Upload the software to the ESP32 using Platform IO "Project Tasks", Run "esp32dev" -> "General" -> "Upload and Monitor"

## Usage

At this point, you should be able to open a web browser on a device connected to the same network as the ESP32 and enter esp32/ into the address bar.  From there, you can control the pins on the ESP32.