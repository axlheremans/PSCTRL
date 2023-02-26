#ifndef _GPIO_H_
#define _GPIO_H_

void init_gpio();
void close_gpio();

void gpio_write_acceleration(short value);
void gpio_write_steer(short value);

void gpio_delay(int t);

#endif // _GPIO_H_
