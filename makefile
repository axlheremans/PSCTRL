all: psctrl

psctrl: main.o gpio.o
	gcc main.o gpio.o -lwiringPi -o psctrl
	
main.o: main.c
	gcc -c main.c
	
gpio.o: gpio.c
	gcc -c gpio.c
	
clean:
	rm -rf *.o psctrl
