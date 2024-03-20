# RPIPicoMicroRosServoExp

Add a custom msg to MicroROS for Servo control. Example code for [Video Blog](https://youtu.be/IPxyBB2nrXE)

## uros_ws
This folder contains the custom msg and Micro ROS workspace. You need to rebuild the microros library to add the custom msg to the Pico.

## src
This contains a simple applciation for the Pico to control two servos on GP02 and GP03. It uses the MicroROS library build in the uros_ws.

## Hardware

+ MG996R with PWM connected to GPIO2
+ MG996R with PWM connected to GPIO3
+ Status LED (via 100ohm resistor) to GPIO15
+ Blink LED (via 100ohm resistor) to GPIO15
