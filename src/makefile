CC = gcc
CFLAGS = -Wall 

all: gpiod pigpio wiringpi lgpio gpiolib

gpiolib:
	${CC} ${CFLAGS} gpio_gpiolib.c pimodel.c gpiolib/gpiolib.c gpiolib/gpiochip_bcm2712.c gpiolib/gpiochip_bcm2835.c gpiolib/gpiochip_rp1.c gpiolib/util.c -o gpio_gpiolib
gpiod:
	${CC} ${CFLAGS} gpio_libgpiod.c pimodel.c -o gpio_gpiod -Wall -lgpiod
pigpio:
	${CC} ${CFLAGS} gpio_pigpio.c -o gpio_pigpio -lpigpio -Wall
wiringpi:
	${CC} ${CFLAGS} gpio_wiringpi.c -o gpio_wiringpi -Wall -lwiringPi
	${CC} ${CFLAGS} gpio_wiringpi_sysfs.c -o gpio_wiringpi_sysfs -Wall -lwiringPi
lgpio:
	${CC} ${CFLAGS} gpio_lgpio.c pimodel.c -o gpio_lgpio -Wall -llgpio
bench:
	./gpio_gpiod && echo  && ./gpio_lgpio && echo  && sudo ./gpio_pigpio && echo  && ./gpio_wiringpi && echo  && ./gpio_wiringpi_sysfs
pybench:
	./gpio_gpiozero.py && echo  && ./gpio_libgpiod.py && echo  && ./gpio_libgpiod2.py && echo  && ./gpio_RPi.GPIO.py 
pi5bench:
	./gpio_gpiozero.py && echo  && ./gpio_libgpiod.py && echo  && ./gpio_gpiod && echo  && ./gpio_lgpio && ./gpio_wiringpi_sysfs
install:
	sudo apt install python3-gpiozero python3-rpi.gpio python3-libgpiod python3-lgpio libgpiod2 libgpiod-dev pigpio libpigpio-dev liblgpio1 liblgpio-dev 
clean:
	rm -f gpio_wiringpi gpio_pigpio gpio_gpiod gpio_wiringpi_sysfs gpio_gpiolib
