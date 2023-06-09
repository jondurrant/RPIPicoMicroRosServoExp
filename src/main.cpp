/*
 * main.cpp
 *
 *  Created on: 9 Feb 2023
 *      Author: jondurrant
 */


#include <stdio.h>
#include "pico/stdlib.h"

#include <time.h>
extern"C"{
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>
#include <rmw_microros/rmw_microros.h>
#include <ros2_servo/msg/servo.h>

#include "pico_uart_transports.h"
}

#include "Servo.h"

const uint NUM_SERVOS = 2;

const uint LED_PIN = 14;
const uint PULSE_LED = 15;

rcl_publisher_t servo_pub;
ros2_servo__msg__Servo servo_msg;
ros2_servo__msg__Servo servo_target_msg;

enum states {
  WAITING_AGENT,
  AGENT_AVAILABLE,
  AGENT_CONNECTED,
  AGENT_DISCONNECTED
} state;

rcl_timer_t timer;
rcl_node_t node;
rcl_allocator_t allocator;
rclc_support_t support;
rclc_executor_t executor;


Servo servo_device[NUM_SERVOS];


const char * servo_target_topic_name = "servo_target";
rcl_subscription_t servo_sub;

void subscription_callback(const void * msgin){
    const ros2_servo__msg__Servo * msg = (const ros2_servo__msg__Servo *)msgin;
    servo_device[msg->servo].goRad(msg->radians);
}



void timer_callback(rcl_timer_t *timer, int64_t last_call_time){

	for (uint8_t i = 0; i < NUM_SERVOS; i++){
		servo_msg.servo = i;
		servo_msg.radians = servo_device[i].getRad();
		rcl_ret_t ret = rcl_publish(&servo_pub, &servo_msg, NULL);
	}

}

bool pingAgent(){
    // Wait for agent successful ping for 2 minutes.
    const int timeout_ms = 100;
    const uint8_t attempts = 1;

    rcl_ret_t ret = rmw_uros_ping_agent(timeout_ms, attempts);

    if (ret != RCL_RET_OK){
    	gpio_put(LED_PIN, 0);
    	return false;
    } else {
    	gpio_put(LED_PIN, 1);
    }
    return true;
}

void createEntities(){
	allocator = rcl_get_default_allocator();

	rclc_support_init(&support, 0, NULL, &allocator);

	rclc_node_init_default(&node, "pico_node", "", &support);

	rclc_publisher_init_default(
			&servo_pub,
			&node,
			ROSIDL_GET_MSG_TYPE_SUPPORT(ros2_servo, msg, Servo),
			"servo_current");

	rclc_timer_init_default(
		&timer,
		&support,
		RCL_MS_TO_NS(1000),
		timer_callback);

	const rosidl_message_type_support_t * type_support =
	    ROSIDL_GET_MSG_TYPE_SUPPORT(ros2_servo, msg, Servo);
	rclc_subscription_init_default(
	        &servo_sub,
	        &node,
	        type_support,
	        servo_target_topic_name);

	rclc_executor_init(&executor, &support.context, 2, &allocator);
	rclc_executor_add_timer(&executor, &timer);

	rclc_executor_add_subscription(&executor,
			&servo_sub,
			&servo_target_msg,
			&subscription_callback,
			ON_NEW_DATA);
}

void destroyEntities(){
	rmw_context_t * rmw_context = rcl_context_get_rmw_context(&support.context);
	(void) rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

	rcl_publisher_fini(&servo_pub, &node);
	rcl_subscription_fini(&servo_sub, &node);
	rcl_timer_fini(&timer);
	rclc_executor_fini(&executor);
	rcl_node_fini(&node);
	rclc_support_fini(&support);
}


void init_servos(){

	servo_device[0].setGP(2);
	servo_device[1].setGP(3);

	for (uint8_t i = 0 ; i < NUM_SERVOS; i++){
		servo_device[i].goRad(1.51);
	}

}



int main()
{
	stdio_init_all();
	init_servos();

    rmw_uros_set_custom_transport(
		true,
		NULL,
		pico_serial_transport_open,
		pico_serial_transport_close,
		pico_serial_transport_write,
		pico_serial_transport_read
	);

    bool pulse;
    gpio_init(PULSE_LED);
    gpio_set_dir(PULSE_LED, GPIO_OUT);
    gpio_put(PULSE_LED, pulse);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);
    sleep_ms(500);
    gpio_put(LED_PIN, 0);


    allocator = rcl_get_default_allocator();
    state = WAITING_AGENT;

    while (true){
    	switch (state) {
    	    case WAITING_AGENT:
    	      state = pingAgent() ? AGENT_AVAILABLE : WAITING_AGENT;
    	      break;
    	    case AGENT_AVAILABLE:
    	      createEntities();
    	      state = AGENT_CONNECTED ;
    	      break;
    	    case AGENT_CONNECTED:
    	      state = pingAgent() ? AGENT_CONNECTED : AGENT_DISCONNECTED;
    	      if (state == AGENT_CONNECTED) {
    	        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    	      }
    	      break;
    	    case AGENT_DISCONNECTED:
    	      destroyEntities();
    	      state = WAITING_AGENT;
    	      break;
    	    default:
    	      break;
    	  }

        pulse = ! pulse;
        gpio_put(PULSE_LED, pulse);
    }
    return 0;
}
