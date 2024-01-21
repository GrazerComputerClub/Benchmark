CC = gcc
CFLAGS = -Wall 

all: gpiod pigpio wiringpi  

gpiod: 
	${CC} ${CFLAGS} gpio_libgpiod.c -o gpio_gpiod -Wall -lgpiod
pigpio:
	${CC} ${CFLAGS} gpio_pigpio.c -o gpio_pigpio -lpigpio -Wall
wiringpi:
	${CC} ${CFLAGS} gpio_wiringpi.c -o gpio_wiringpi -Wall -lwiringPi
	${CC} ${CFLAGS} gpio_wiringpi_sysfs.c -o gpio_wiringpi_sysfs -Wall -lwiringPi
bench:
	./gpio_gpiod  && echo  && sudo ./gpio_pigpio && echo  && ./gpio_wiringpi && echo  && ./gpio_wiringpi_sysfs
pybench:
	./gpio_gpiozero.py && echo  && ./gpio_libgpiod.py && echo  && ./gpio_libgpiod2.py && echo  && ./gpio_RPi.GPIO.py 
pi5bench:
	./gpio_gpiozero.py && echo  && ./gpio_libgpiod.py && echo  && ./gpio_gpiod
clean:
	rm -f gpio_wiringpi gpio_pigpio gpio_gpiod 
