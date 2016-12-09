#! /usr/bin/env python

from gpiozero import LED, Button
from time import sleep

led = LED(17)
button = Button(4)

print "Waiting for button press"
while True:
  button.wait_for_press()
  led.on()
  sleep(1)
  led.off()
