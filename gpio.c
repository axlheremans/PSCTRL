#include "gpio.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

#define ACCELERATION_PIN	1
#define STEER_LEFT_PIN		5
#define STEER_RIGHT_PIN		4
#define PWM_RANGE			255

void init_gpio() {
	wiringPiSetup();
	
	pinMode(ACCELERATION_PIN, OUTPUT);
	softPwmCreate(ACCELERATION_PIN, 1, PWM_RANGE);
	
	pinMode(STEER_RIGHT_PIN, OUTPUT);
	pinMode(STEER_LEFT_PIN, OUTPUT);
}

void close_gpio() {

}

void gpio_write_acceleration(short value) {
	printf("Acceleration: %d\n", value);
	softPwmWrite(ACCELERATION_PIN, value);
	
}

void gpio_write_steer(short value) {
	printf("Steer: %d\n", value);
	if (value < 0) {
		printf("LEFT\n");
		digitalWrite(STEER_LEFT_PIN, HIGH);
		digitalWrite(STEER_RIGHT_PIN, LOW);
	} else if (value > 0) {
		printf("RIGHT\n");
		digitalWrite(STEER_LEFT_PIN, LOW);
		digitalWrite(STEER_RIGHT_PIN, HIGH);
	} else {
		digitalWrite(STEER_RIGHT_PIN, LOW);
		digitalWrite(STEER_LEFT_PIN, LOW);
	}
}

void gpio_delay(int t) {
	delay(t);
}
