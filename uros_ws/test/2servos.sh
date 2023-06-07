#!/bin/sh

#servo0 0.0 to 1.8 (1.0 Centre)
#servo1 1.0  to 1.7 (1.5 Centre)

while [ true ]
do
  #centre
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 0, radians: 1.0}'
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 1, radians: 1.5}'
  
  #Left
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 0, radians: 0.0}'
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 1, radians: 1.0}' 
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 1, radians: 1.5}'
  
  #Centre
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 0, radians: 1.0}'
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 1, radians: 1.0}' 
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 1, radians: 1.5}'
  
  #Right
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 0, radians: 1.8}'
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 1, radians: 1.0}' 
  ros2 topic pub -1 /servo_target ros2_servo/msg/Servo '{servo: 1, radians: 1.5}'
  
 

done