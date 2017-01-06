#include  <wiringPiI2C.h>
#include  <stdio.h>
#include  <time.h>

int main(int argc, char* argv[])
{
  printf("Resetting device and all signal paths\n");

  int fd = wiringPiI2CSetup (0x68);
  wiringPiI2CWriteReg8 (fd,0x6B,0x80); // Reset device
  struct timespec one_hundred_millis = { 0, 100e6 };
  nanosleep(&one_hundred_millis, NULL);
  wiringPiI2CWriteReg8 (fd,0x6A,0x01); // Reset all signal paths

  return 0;
}
