import com.pi4j.io.gpio._
import com.pi4j.wiringpi._
import java.time.Instant
import java.time.temporal.ChronoUnit.NANOS

object UsingPi4j {

  def main(): Unit = {

                                GpioUtil.enableNonPrivilegedAccess()
				GpioFactory.setDefaultProvider(new RaspiGpioProvider(RaspiPinNumberingScheme.BROADCOM_PIN_NUMBERING))

				val controller = GpioFactory.getInstance()
				val trigger = controller.provisionDigitalOutputPin(RaspiBcmPin.GPIO_17)
				val echo = controller.provisionDigitalInputPin(RaspiBcmPin.GPIO_18)

				def distance(): Double = {

					// clear trigger
					trigger.low()
					Thread.sleep(0, 2000)
					
					// perform a 10 microsecond pulse
					trigger.high()
					Thread.sleep(0, 10000)
					trigger.low()

					while (echo.isLow()) {}
					val rise_time = Instant.now()
					while (echo.isHigh()) {}
					val fall_time = Instant.now()

					NANOS.between(rise_time, fall_time).toDouble / 1000000000 * 340 / 2
				}

				val N = 1000

				(0 until N) foreach { i =>
					println(s"$i: ${distance()} m")
				}
  }
}
