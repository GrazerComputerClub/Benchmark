#! /usr/bin/env python3
"""
Detect Raspberry Pi Model (new-style revision) 
  see https://www.raspberrypi.com/documentation/computers/raspberry-pi.html#raspberry-pi-revision-codes
by GC2 - mstroh
Licence: CC BY 4.0
"""


def detect_model() -> str:
  with open('/proc/device-tree/model') as f:
    model = f.read()
  return model

def detect_revision() -> str:
  with open('/proc/device-tree/system/linux,revision', mode="rb") as f:
    revision = f.read(4)
  int_val = int.from_bytes(revision, "big")
  return hex(int_val)

def detect_serial() -> str:
  with open('/proc/device-tree/system/linux,serial', mode="rb") as f:
    serial = f.read(8)
  int_val = int.from_bytes(serial, "big")
  return hex(int_val)


def detect_modelno() -> int:
  with open('/proc/device-tree/system/linux,revision', mode="rb") as f:
    revision = f.read(4)
  byte0 = revision[3] >> 4 
  byte1 = revision[2] & 0x0F
  return int(byte1*16+byte0)


def detect_RAMSize() -> int:
  with open('/proc/device-tree/system/linux,revision', mode="rb") as f:
    revision = f.read(4)
  byte = (revision[1] & 0x70) >> 4 
  return int(byte)


model = detect_model()
revision = detect_revision()
if int(revision,16)<0x800000 :
    print("model: {0}".format( model ))
    print("old revision format, modelno detection not supported\n")
    quit()
modelno = detect_modelno()
RAMSizeID = detect_RAMSize()
if  RAMSizeID == 0:
    RAMSIZE=256
elif RAMSizeID == 1:
    RAMSIZE=512
elif RAMSizeID == 2:
    RAMSIZE=1024
elif RAMSizeID == 3:
    RAMSIZE=2048
elif RAMSizeID == 4:
    RAMSIZE=4096
elif RAMSizeID == 5:
    RAMSIZE=8192
serial = detect_serial()
print("model: {0}, modelno: {3}, RAM: {4} MB({5}), revision: {1}, serial: {2}".format( model, revision, serial, modelno , RAMSIZE, RAMSizeID))
