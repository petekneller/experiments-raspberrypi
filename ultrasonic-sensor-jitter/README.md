An experiment to see how the 'level' of the language (eg. C/Python/Java) affects the jitteriness of reading sensor values (in this case, an ultrasonic sensor)

Each script file named `using_xyz` represents a different implementation of a loop that asks an ultrasonic sensor to make N determinations of the range to an object. For this experiment I've put an obstacle as close as I can to 30cm from the front of the sensor.

The script files will dump to stdout a series of lines of the form `n: x m` where `n` is the current iteration and `x` is the distance as measured by the sensor (in metres)

From there I run: `./using_xyz | grep -P '^\d+:' | awk 'BEGIN { FS=" " } { print $2 }' | ./stats` where `stats` is a little python script that generates min,max,mean,std deviation from the samples

The results:
  N = 1000
  using_       min      max     mean    stddev     time using_
  ------       ---      ---     ----    ------     -----------
  gpiozero*    0.2928   0.2944  0.2744  1.896e-4   2.2s
  gpio         0.2839   0.3056  0.2889  2.009e-3   2.8s
  wiringpi     0.2882   0.3053  0.2742  2.267e-3   2.6s
  pi4j						   

* for gpiozero N = 10,000. See notes below for explanation

Notes:

* gpiozero's `SmoothedInputDevice` (a superclass of `DistanceSensor`) smooths out input values by using a background thread to (by default) constantly fill a queue (default size 30) with sensor readings. A fetch of the sensors `.value` actually provides the average of all the values in the queue. So it is possible to query the sensor faster than the background thread is providing real readings (I did see duplicates appearing in the list of values from `DistanceSensor`). So in this experiment I've increased N for gpiozero just to get a larger number of unique readings
* I'm using wiringPi in '/sys/class/gpio' mode, in which the pins are exported into the '/sys/...' namespace using a utility before running the application. This enables the app to use gpio without running as root. The gpio utility can be used as such:
** `gpio exports` shows all exported pins and their setup
** `gpio unexportall` sets things a-fresh
** `gpio export nn in/out` sets pin nn as either in or out
