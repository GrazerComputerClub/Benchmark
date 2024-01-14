CC = gcc
CFLAGS = -Wall 

all: gpiod pigpio wiringpi  

gpiod: 
	${CC} ${CFLAGS} gpio_libgpiod.c -o gpio_gpiod -Wall -lgpiod
pigpio:
	${CC} ${CFLAGS} gpio_pigpio.c -o gpio_pigpio -lpigpio -Wall
wiringpi:
	${CC} ${CFLAGS} gpio_wiringpi.c -o gpio_wiringpi -Wall -lwiringPi
bench:
	./gpio_gpiod  && echo  && sudo ./gpio_pigpio && echo  && ./gpio_wiringpi
pybench:
	./gpio_gpiozero.py && echo  && ./gpio_libgpiod.py && echo  && ./gpio_RPi.GPIO.py 
clean:
	rm -f gpio_wiringpi gpio_pigpio gpio_gpiod 
