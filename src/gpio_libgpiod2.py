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

def detect_pi_modelno() -> int:
  with open('/proc/device-tree/system/linux,revision', mode="rb") as f:
    revision = f.read(4)
  byte0 = revision[3] >> 4 
  byte1 = revision[2] & 0x0F
  return int(byte1*16+byte0)

SigOUT = "GPIO24"
LOOPS = 1050000

if detect_pi_modelno()==23 :   # Raspberry Pi 5
    chip=gpiod.Chip('gpiochip4')
else :
    chip=gpiod.Chip('gpiochip0')
line = gpiod.find_line(SigOUT)
lines = chip.get_lines([line.offset()])
lines.request(consumer='gpio_bench', type=gpiod.LINE_REQ_DIR_OUT, default_vals=[0])

print("gpiod toggle {0} times at {1} (multi line function)\n".format( LOOPS, lines ))

t0 = time.time()

for i in range(LOOPS):
    lines.set_values([0])
    lines.set_values([1])

t1 = time.time()

print("gpiod:\t{:>6.0f} toggles per second (took {:>5.3f} sec)".format( (1.0 * LOOPS) / (t1 - t0), (t1 - t0) ))



