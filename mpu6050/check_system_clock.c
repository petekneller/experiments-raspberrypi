#include  <stdio.h>
#include  <time.h>

int main(int argc, char* argv[])
{
  int success;
  int clock = CLOCK_REALTIME;

  printf("Testing the global system clock CLOCK_REALTIME... \n");
  struct timespec clock_res = {};
  success = clock_getres(clock, &clock_res);
  printf("Got return code: %d \n", success);
  printf("Clock reported to have precision of %f seconds and %d nanoseconds \n", (double)clock_res.tv_sec, clock_res.tv_nsec);
  
  printf("Running a couple of tests...\n");
  struct timespec time1 = {};
  struct timespec time2 = {};

  printf("Taking 2 samples from the clock approximately a second apart - do they look sensible? \n");
  clock_gettime(clock, &time1);
  struct timespec one_second = { 1, 0 };
  nanosleep(&one_second, NULL);
  clock_gettime(clock, &time2);
  printf("Clock time 1: %f seconds, %d nanoseconds \n", (double)time1.tv_sec, time1.tv_nsec);
  printf("Clock time 2: %f seconds, %d nanoseconds \n", (double)time2.tv_sec, time2.tv_nsec);

  printf("Taking 2 samples from the clock approximately 0.3 seconds apart - do they look sensible? \n");
  clock_gettime(clock, &time1);
  struct timespec one_third_second = { 0, 300000000 };
  nanosleep(&one_third_second, NULL);
  clock_gettime(clock, &time2);
  printf("Clock time 1: %f seconds, %d nanoseconds \n", (double)time1.tv_sec, time1.tv_nsec);
  printf("Clock time 2: %f seconds, %d nanoseconds \n", (double)time2.tv_sec, time2.tv_nsec);

  return 0;
}
