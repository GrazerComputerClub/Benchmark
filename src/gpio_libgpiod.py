#! /usr/bin/env python3
"""
GPIO write speed benchmark
using gpiod Python Interface
by GC2 - mstroh
Licence: CC BY 4.0
Install: sudo apt install python3-libgpiod
"""

import gpiod
import time
from signal import pause

SigOUT = "GPIO24"
LOOPS = 1000000

chip=gpiod.Chip('gpiochip0')
line = gpiod.find_line("GPIO24")
lines = chip.get_lines([line.offset()])
lines.request(consumer='gpio_bench', type=gpiod.LINE_REQ_DIR_OUT, default_vals=[0])

print("libgpio toggle {0} times at {1}\n".format( LOOPS, lines ))

t0 = time.time()

for i in range(LOOPS):
    lines.set_values([0])
    lines.set_values([1])

t1 = time.time()

print("libgpio:\t{:>6.0f} toggles per second (took {:>5.3f} sec)".format( (1.0 * LOOPS) / (t1 - t0), (t1 - t0) ))



