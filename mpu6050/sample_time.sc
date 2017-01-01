#! /usr/bin/env amm

import $ivy.`com.pi4j:pi4j-core:1.1`
import com.pi4j.wiringpi.I2C

@main
def main(N: Int): Unit = {
  val mpu = I2C.wiringPiI2CSetup(0x68)
  I2C.wiringPiI2CWriteReg8(mpu, 0x6B, 0x00) // wake up the device

  var previous_time = System.nanoTime()
  0 until N foreach { _ =>
    I2C.wiringPiI2CReadReg16(mpu, 0x3B)
    val now = System.nanoTime()
    println("%e".format((now - previous_time).toDouble / 1000000000))
    previous_time = now
  }
}
