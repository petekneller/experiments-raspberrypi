#! /usr/bin/env python

from gpiozero import LED

led = LED(17) # ensure the led is in pin 17

led.on()
raw_input("Hit any key to switch off the LED")
led.off()
