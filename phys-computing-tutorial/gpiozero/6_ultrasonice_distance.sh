#! /usr/bin/env python

from gpiozero import DistanceSensor

sensor = DistanceSensor(echo=18, trigger=17)
print(sensor.distance)
