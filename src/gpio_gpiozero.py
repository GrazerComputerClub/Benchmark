#! /usr/bin/env python3
"""
This program toggles a GPIO pin and measures rate.
Using gpiozero Python Interface
"""

from gpiozero import LED
import time
from signal import pause

LOOPS = 1000000
gpio24 = LED(24)

t0 = time.time()

for i in range(LOOPS):
    gpio24.off()
    gpio24.on()

t1 = time.time()

print("gpiozero\t{:>10.0f} toggles per second ".format((1.0 * LOOPS) / (t1 - t0)))


