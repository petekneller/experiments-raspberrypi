#include <stdio.h>
#include <time.h>
#include <wiringPiI2C.h>

int fetchFifoCount(int fd)
{
  int hi = wiringPiI2CReadReg8(fd, 0x72);
  int lo = wiringPiI2CReadReg8(fd, 0x73);
  return (hi << 8) + lo;
}

void fetchFifo(int fd, int count)
{
  int i;
  for (i = 0; i < count; i++)
  {
    wiringPiI2CReadReg8(fd, 0x74);
  }
}

int main(int argc, char* argv[])
{
  int fd = wiringPiI2CSetup(0x68);
  wiringPiI2CWriteReg8(fd, 0x6B, 0x00); // wake up device
  wiringPiI2CWriteReg8(fd, 0x23, 0x40); // enable gyro X to FIFO
  wiringPiI2CWriteReg8(fd, 0x1A, 0x00); // set DLPF off
  wiringPiI2CWriteReg8(fd, 0x19, 7); // set sampling divider = x + 1
  wiringPiI2CWriteReg8(fd, 0x6A, 0x40); // enable FIFO

  while (1)
  {
    int cnt = fetchFifoCount(fd);
    struct timespec t1 = {};
    clock_gettime(CLOCK_REALTIME, &t1);
    fetchFifo(fd, cnt);
    struct timespec t2 = {};
    clock_gettime(CLOCK_REALTIME, &t2);
    double delta_sec = ((double)t2.tv_sec) - ((double)t1.tv_sec);
    long delta_nano = t2.tv_nsec - t1.tv_nsec;
    double delta = delta_sec + ((double)delta_nano / 1e9);
    printf("Fetched %4d bytes from FIFO and took %f seconds \n", cnt, delta);
  }

  return 0;
}
