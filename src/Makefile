CC = gcc
CFLAGS = -Wall
OUTDIR = ../bin
RESULTSDIR = ../results
LDFLAGS = 

tests_c = gpio_gpiod gpio_pigpio gpio_wiringpi isr_wiringpi gpio_lgpio gpio_gpiolib

all: $(tests_c)

all-gperf: LDFLAGS=-Bstatic -Wl,--no-as-needed,-lprofiler,--as-needed
all-gperf: all

all-perf: LDFLAGS=
all-perf: CFLAGS=-g3 -fno-inline -O0 -Wall
all-perf: all

bench-flamegraphs: all-gperf
bench-flamegraphs: bench-gperf
bench-flamegraphs: clean-bin
bench-flamegraphs: all-perf
bench-flamegraphs: bench-perf

gpio_gpiod:
	${CC} ${CFLAGS} ./C/gpio_libgpiod.c ./C/pimodel.c -o ${OUTDIR}/gpio_gpiod $(LDFLAGS) -lgpiod

gpio_pigpio:
	${CC} ${CFLAGS} ./C/gpio_pigpio.c -o ${OUTDIR}/gpio_pigpio $(LDFLAGS) -lpigpio

gpio_wiringpi:
	${CC} ${CFLAGS} ./C/gpio_wiringpi.c -o ${OUTDIR}/gpio_wiringpi $(LDFLAGS) -lwiringPi

gpio_wiringpi_sysfs:
	${CC} ${CFLAGS} ./C/gpio_wiringpi_sysfs.c -o ${OUTDIR}/gpio_wiringpi_sysfs $(LDFLAGS) -lwiringPi

isr_wiringpi:
	${CC} ${CFLAGS} ./C/isr_wiringpi.c -o ${OUTDIR}/isr_wiringpi $(LDFLAGS) -lwiringPi

gpio_lgpio:
	${CC} ${CFLAGS} ./C/gpio_lgpio.c ./C/pimodel.c -o ${OUTDIR}/gpio_lgpio $(LDFLAGS) -llgpio 

gpio_gpiolib:
	${CC} ${CFLAGS} ./C/gpio_gpiolib.c ./C/pimodel.c gpiolib/gpiolib.c gpiolib/gpiochip_bcm2712.c gpiolib/gpiochip_bcm2835.c gpiolib/gpiochip_rp1.c gpiolib/util.c -o ${OUTDIR}/gpio_gpiolib $(LDFLAGS)

.ONESHELL:
bench:
	./${OUTDIR}/gpio_gpiod 
	echo
	./${OUTDIR}/gpio_lgpio
	echo
	sudo ./${OUTDIR}/gpio_pigpio
	echo 
	./${OUTDIR}/gpio_wiringpi
	echo
	./${OUTDIR}/gpio_gpiolib

bench-gperf:  
	for t in $(tests_c) ; do \
		echo === profiling: gperf $${t} === ; \
		CPUPROFILE=./${OUTDIR}/$${t}.prof CPUPROFILE_FREQUENCY=10000 ./${OUTDIR}/$${t} ; \
		google-pprof ./${OUTDIR}/$${t} ./${OUTDIR}/$${t}.prof --collapsed | ../submodules/FlameGraph/flamegraph.pl > ./${RESULTSDIR}/$${t}.flamegraph_g.svg ; \
		echo ; \
	done

bench-perf:  
	for t in $(tests_c) ; do \
		echo === profiling: perf via flamegraph-rs $${t} === ; \
		flamegraph -o ./${RESULTSDIR}/$${t}.flamegraph.svg -- ./${OUTDIR}/$${t} ; \
		echo ; \
	done

ONESHELL:
bench-wiringpi:
	flamegraph -o ./${RESULTSDIR}/gpio_wiringpi.flamegraph.svg -- ./${OUTDIR}/gpio_wiringpi

ONESHELL:
pybench:
	./python/gpio_gpiozero.py
	echo
	./python/gpio_libgpiod.py
	echo
	./python/gpio_libgpiod2.py
	echo
	./python/gpio_RPi.GPIO.py 

.ONESHELL:
pi5bench:
	./python/gpio_gpiozero.py
	echo
	./python/gpio_libgpiod.py
	echo
	./${OUTDIR}/gpio_gpiod
	echo
	./${OUTDIR}/gpio_lgpio
	echo
	./${OUTDIR}/gpio_gpiolib

install:
	sudo apt install python3-gpiozero python3-rpi.gpio python3-libgpiod python3-lgpio libgpiod2 libgpiod-dev pigpio libpigpio-dev liblgpio1 liblgpio-dev -y

install-perf:
	sudo apt install cargo python3-bpfcc linux-perf google-perftools libgoogle-perftools-dev -y ; \
	cargo install flamegraph ; \
	DTEMPLATE=/usr/share/d3-flame-graph/d3-flamegraph-base.html ; \
	if [ -f "$${DTEMPLATE}" ]; then \
    echo "d3-flamegraph-base template exists." ; \
	else \
		sudo mkdir /usr/share/d3-flame-graph/ ; \
		sudo wget https://cdn.jsdelivr.net/npm/d3-flame-graph@4.1.3/dist/templates/d3-flamegraph-base.html -O $${DTEMPLATE} ; \
	fi
install-perf: install

# flamegraphs:
# https://www.brendangregg.com/flamegraphs.html

clean-bin:
	rm -f ${OUTDIR}/*

clean: 
	rm -f ${RESULTSDIR}/*
clean: clean-bin
