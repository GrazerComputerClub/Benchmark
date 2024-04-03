#! /usr/bin/env python3
"""
Detect Raspberry Pi Model with gpiozero
by GC2 - mstroh
Licence: CC BY 4.0
"""

from gpiozero import pi_info

pi = pi_info()
#print('{0}'.format(pi))
#print('{0:full}'.format(pi)))
print('\n{0:board}'.format(pi))
print('\n{0:specs}'.format(pi))
#print('{0:headers}'.format(pi))
print('Model: {0}'.format(pi.model));
print('PCB rev.: {0}'.format(pi.pcb_revision));
print('\n')

if pi.soc=="BCM2712" :
    print("Raspberry Pi 5\n");
elif pi.soc=="BCM2711" :
    print("Raspberry Pi 4\n");
elif pi.soc=="BCM2837" :
    print("Raspberry Pi 3/Zero 2 SoC BCM2837 found");
    if "Zero" in pi.model :
        print("Raspberry Pi Zero 2\n");
    else : 
        print("Raspberry Pi 3\n");
elif pi.soc=="BCM2836" :
    print("Raspberry Pi 2\n");
elif pi.soc=="BCM2835" :
    print("Raspberry Pi 1/Zero SoC BCM2835 found");
    if "Zero" in pi.model :
        print("Raspberry Pi Zero\n");
    elif "B"==pi.model and "1.0"==pi.pcb_revision :
        print("Raspberry Pi Rev. 1.0 (First Raspberry Pi)\n");
    else :
        print("Raspberry Pi 1\n");

