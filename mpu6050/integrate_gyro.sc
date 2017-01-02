#! /usr/bin/env amm

import $ivy.`com.pi4j:pi4j-core:1.1`
import com.pi4j.wiringpi.I2C
import System.nanoTime
import math.pow

val mpu = I2C.wiringPiI2CSetup(0x68)
I2C.wiringPiI2CWriteReg8(mpu, 107, 0x00) // wake up device

def readGyroX(): Double = {
  val raw = (I2C.wiringPiI2CReadReg8(mpu, 67) << 8) + 
    I2C.wiringPiI2CReadReg8(mpu, 68)
  val signed = if (raw >= pow(2, 15))
   raw - 1
  else 
    raw
  signed.toShort.toDouble / 131
}

val bias = (0 until 10 map { _ => readGyroX } sum) / 10

var current_posX: Double = 0
var previous_time = nanoTime()
while (true) {
  val current_velX = readGyroX() - bias
  val current_time = nanoTime()
  val time_delta = (current_time - previous_time) / 1e9
  current_posX = current_posX + current_velX * time_delta
  println(s"time delta: $time_delta acc: $current_velX pos: $current_posX")
  previous_time = current_time
}
