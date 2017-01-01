#!/usr/bin/python

import smbus
from time import clock
from numpy import zeros, amin, amax, mean, std

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

N = 1000
sample_times = zeros(N)
for i in range(N):
  print take_sample()
  sample_times[i] = clock()

deltas = zeros(N-1)
for i in range(0, len(sample_times)-1):
  deltas[i] = sample_times[i+1] - sample_times[i]
  

print "Loop times for ", N, " samples"
print "Minimum: ", amin(deltas), " seconds"
print "Maximum: ", amax(deltas), " seconds"
print "Mean: ", mean(deltas), " seconds"
print "Std dev: ", std(deltas), " seconds"
