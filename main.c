/*
 * main.c
 * 
 * Copyright 2023  <pi@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

#include "ps4.h"
#include "gpio.h"

struct axis_state {
	short value;
};


int read_event(int fc, struct js_event *event);
size_t get_axis_state(struct js_event *event, struct axis_state axes[3]);
size_t get_axis_count(int fd);
size_t get_button_count(int fd);



int main(int argc, char **argv)
{
	printf("Hello, World!\n");
	
	const char *device;
	int js;
	
	struct js_event event;
	struct axis_state axes[8] = {0};
	axes[AXIS_R2].value = (short)-32767;
	axes[AXIS_L2].value = (short)-32767;
	size_t axis;
	
	if (argc > 1) {
		device = argv[1];
	} else {
		device = "/dev/input/js0";
	}
	
	init_gpio();
	
	// Connect to joystick
	js = open(device, O_RDONLY);
	while (js == -1) {
		perror("Could not open joystick");
		sleep(5);
		js = open(device, O_RDONLY);
	}
	
	size_t numButtons = get_button_count(js);
	size_t numAxis = get_axis_count(js);
	printf("Found joystick\n\tNum buttons: %d\n\tNum axis:%d\n", numButtons, numAxis);
	
	int run = 1;
	while((read_event(js, &event) == 0) && (run == 1)) {
		switch (event.type) {
			case JS_EVENT_BUTTON:
				switch (event.number) {
					case BUTTON_SHARE:
						if (event.value) {
							run = 0;
						}
						break;
					default:
						//printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");
						break;
				}
				break;
			case JS_EVENT_AXIS:
				axis = get_axis_state(&event, axes);
				if (axis < 8) {
					printf("Axis %zu at (%6d)\n", axis, axes[axis].value);
				}
				break;
			default:
				break;
		}
		
		printf("Axis %zu at (%6d)\n", AXIS_R2, axes[AXIS_R2].value);
		short acceleration = (axes[AXIS_R2].value >> 8) + 128;
		gpio_write_acceleration(acceleration);
		
		short steer = axes[AXIS_LEFT_X].value;
		gpio_write_steer(steer);
		
		fflush(stdout);
	}
	
	close(js);
	close_gpio();
	return 0;
}


int read_event(int fd, struct js_event *event) {
	ssize_t numBytes;
	numBytes = read(fd, event, sizeof(*event));
	if (numBytes == sizeof(*event)) {
		return 0;
	}
	return -1;
}

size_t get_axis_state(struct js_event *event, struct axis_state axes[8]) {
	printf("Event number: %d\n", event->number);
	size_t axis = event->number;
	if (axis<8) {
		axes[axis].value = event->value;
	}
	return axis;
}

size_t get_axis_count(int fd) {
	__u8 axes;
	if (ioctl(fd, JSIOCGAXES, &axes) == -1) {
		return 0;
	}
	return axes;
}

size_t get_button_count(int fd) {
	__u8 buttons;
	if (ioctl(fd, JSIOCGBUTTONS, &buttons) == -1) {
		return 0;
	}
	return buttons;
}



