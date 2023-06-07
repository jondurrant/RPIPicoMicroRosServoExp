#!/bin/sh

while [ true ]
do
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 0, radians: 0.0}'
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 0, radians: 1.5}'
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 0, radians: 3.1}'
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 0, radians: 1.5}'

done