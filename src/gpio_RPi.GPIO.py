#! /usr/bin/env python3
"""
GPIO write speed benchmark
using RPi.GPIO Python Interface
by GC2 - mstroh
Licence: CC BY 4.0
Install: sudo apt install python3-rpi.gpio
"""

import RPi.GPIO as GPIO
import time

SigOUT = 24
LOOPS = 4000000

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(SigOUT, GPIO.OUT)

print("RPi.GPIO toggle {0} times at GPIO {1}\n".format( LOOPS, SigOUT ))

t0 = time.time()

for i in range(LOOPS):
    GPIO.output(SigOUT,0)
    GPIO.output(SigOUT,1)

t1 = time.time()

print("RPi.GPIO:\t{:>6.0f} toggles per second (took {:>5.3f} sec)".format( (1.0 * LOOPS) / (t1 - t0), (t1 - t0) ))

GPIO.cleanup()

