#! /usr/bin/env python

from gpiozero import LED, Button
from time import sleep

led = LED(17)
button = Button(4)
delay = 0.2

print(("Waiting for button press... (or hold the button to flicker it at {} seconds").format(delay))
while True:
  button.wait_for_press()
  led.toggle()
  sleep(delay)
