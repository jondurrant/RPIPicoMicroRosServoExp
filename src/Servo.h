/*
 * Servo.h
 *
 *  Created on: 1 Jun 2023
 *      Author: jondurrant
 */

#ifndef SERVO_SRC_SERVO_H_
#define SERVO_SRC_SERVO_H_

#include "pico/stdlib.h"

class Servo {
public:

	Servo();
	Servo(uint8_t gp);
	virtual ~Servo();

	/***
	 * Setup GPIO Pad for Servo
	 * @param gp - GPIO PAD number
	 */
	void setGP(uint8_t gp);

	/**
	 * move to angle: 0 to 180.
	 * @param degree
	 */
	void goDegree(float degree);

	/**
	 * move to angle: 0 to PI.
	 * @param radians
	 */
	void goRad(float rad);


	/***
	 * Get current position in degrees
	 * @return degrees
	 */
	float getDegree();

	/***
	 * Get current position in radians
	 * @return radians
	 */
	float getRad();

private:
	uint8_t xGP = 0;
	float xRad = 1.51;
};

#endif /* SERVO_SRC_SERVO_H_ */
