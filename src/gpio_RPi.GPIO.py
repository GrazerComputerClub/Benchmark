#! /usr/bin/env python3
"""
This program toggles a GPIO pin and measures rate.
Using RPi.GPIO Python Interface
"""

import RPi.GPIO as GPIO
import time
from signal import pause

SigOUT = 24
LOOPS = 1000000

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(SigOUT, GPIO.OUT)

t0 = time.time()

for i in range(LOOPS):
    GPIO.output(SigOUT,0)
    GPIO.output(SigOUT,1)

t1 = time.time()

print("RPi.GPIO\t{:>10.0f} toggles per second ".format((1.0 * LOOPS) / (t1 - t0)))

GPIO.cleanup()

