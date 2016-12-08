#! /usr/bin/env python

import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT) # ensure the LED is tied to pin 17

GPIO.output(17, True)
raw_input("Hit any key to turn the LED back off")
GPIO.output(17, False)
