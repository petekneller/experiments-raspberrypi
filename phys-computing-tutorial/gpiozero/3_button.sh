#! /usr/bin/env python

from gpiozero import Button

button = Button(4)

print "Waiting for button press..."
button.wait_for_press()
print "Button pressed"

