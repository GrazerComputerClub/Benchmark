#! /usr/bin/env python3
"""
GPIO write speed benchmark
using gpiozero Python Interface
by GC2 - mstroh
Licence: CC BY 4.0
Install: sudo apt install python3-gpiozero
"""

from gpiozero import LED
import time
from signal import pause

SigOUT = 24
LOOPS = 105000

gpio = LED(SigOUT)
gpio.off()

print("gpiozero toggle {0} times at {1}\n".format( LOOPS, gpio ))

t0 = time.time()

for i in range(LOOPS):
    gpio.off()
    gpio.on()

t1 = time.time()

print("gpiozero:\t{:>6.0f} toggles per second (took {:>5.3f} sec)".format( (1.0 * LOOPS) / (t1 - t0), (t1 - t0) ))



