# Example project for SPI OLED displays, written for Arduino with an optional server
## Context
This is basic example code to use with an SPI OLED display on an Arduino. I tested it on an Arduino Leonardo, though Arduino code is C++ and is universal amongst the main kinds of boards so you shouldn't have trouble replicating this with an Arduino Uno or such. With the help of instructions down below, you can get your OLED screen to show you temperature data and a Local IP from the computer it's connected to if you'd like to run a Python based server I wrote.
## Instructions
Obviously, all of this is done on a breadboard. A standard one will do the job.
### Screen
For the screen itself, I recommend following Last Minute Enginneers's in-depth tutorial: https://lastminuteengineers.com/oled-display-arduino-tutorial/
### Thermistor (temperature data)
To get the temperature data, you need these:
- A thermistor (I'm using one without a built-in resistor.)
- 10K Resistor (you don't need this if you have a thermistor with a built-in resistor)
- A free Analog 0 pin on your Arduino.
Here's a basic schematic of how the thermistor should be wired up:
![image](https://github.com/user-attachments/assets/5dc5d9d5-210e-4bc8-964c-ad0fa801e0d3)

## Software side
### Arduino code
The code for the Arduino is an Arduino IDE project. It uses the Adafruit SSD1306 library. It includes everything you need for the things I've mentioned, but feel free to touch it up however you like. I highly recommend reading through it and knowing what is where, especially for beginners learning to code in C++ in general. Try tweaking things how you like - serial output delays, the position of text on the display, etc. 
### The Python server
- **IMPORTANT!** You have to set which COM port the server should connect to manually. I will leave the text "WRITE HERE" in the field so that you can replace it with your COM port of choice. You can easily find which COM port your Arduino is assigned to in the Arduino IDE.
The Python server is built to serve as a Serial Monitor for the messages that the Arduino sends to the computer, like the temperature which is by default announced when a difference of 0.1 is detected instead of continiously spammed in order to reduce delays to a minimum and to not overwhelm serial output. You can change this behavior in the Arduino code, not here! It also provides your computer's local IP address to the Arduino for display on the SPI display. This can be useful if you have the Arduino connected to a device with an SSH server so you can easily figure out it's IP and connect to it. This terminal app also has error handling, like if it can't fetch the IP, it announces it instead of crashing. If the Arduino disconnects, the server will try to reconnect to it instead of exiting.

# Have fun!
Thank you for reading this. If this project was useful to you, give it a star!


