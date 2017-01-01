#!/usr/bin/python

import smbus
from time import clock
from numpy import zeros, amin, amax, mean, std
import sys

# Power management registers
power_mgmt_1 = 0x6b

def read_byte(adr):
    return bus.read_byte_data(address, adr)

def read_word(adr):
    high = bus.read_byte_data(address, adr)
    low = bus.read_byte_data(address, adr+1)
    val = (high << 8) + low
    return val

def read_word_2c(adr):
    val = read_word(adr)
    if (val >= 0x8000):
        return -((65535 - val) + 1)
    else:
        return val

def take_sample():
  return read_word_2c(0x43)

bus = smbus.SMBus(1)
address = 0x68       # This is the address value read via the i2cdetect command

# Now wake the 6050 up as it starts in sleep mode
bus.write_byte_data(address, power_mgmt_1, 0)

N = int(sys.argv[1])
times = []
last_time = clock()
for i in range(N):
  take_sample()
  now = clock()
  times.append(now - last_time)
  last_time = now

for t in times:
  print t
