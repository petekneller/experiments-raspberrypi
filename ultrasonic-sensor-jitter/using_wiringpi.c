#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>

/*
   NB. this implementation expects the gpio pins to be exported intothe /sys/class/gpio namespace. This can be achieved using the gpio program:
  gpio export 17 in
  gpio export 18 out

   If this is not done, the program will appear to hang.
*/

#define Trigger 17
#define Echo 18

float distance(void)
{
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  while(digitalRead(Echo) != 1) {}
  struct timeval rising_time;
  gettimeofday(&rising_time, NULL);

  while(digitalRead(Echo) != 0) {}
  struct timeval falling_time;
  gettimeofday(&falling_time, NULL);

  long time1 = rising_time.tv_sec * 1000000 + rising_time.tv_usec;
  long time2 = falling_time.tv_sec * 1000000 + falling_time.tv_usec;
  float distance = (float)(time2 - time1) / 1000000 * 340 / 2;

  return distance;
}

int main(void)
{
  if(wiringPiSetupSys() == -1)
  { 
    // initialize wiring failed
    printf("setup wiringPi failed !");
    return 1; 
  }

  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);

  int N = 1000;
  int i;
  for (i = 0; i < N; i++)
  {
    printf("%i: %0.5f m\n", i, distance());
  }
  
  return 0;
}
