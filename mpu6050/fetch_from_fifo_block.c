#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <time.h>

int fetchFifoCount(int fd)
{
  int hi = i2c_smbus_read_byte_data(fd, 0x72);
  int lo = i2c_smbus_read_byte_data(fd, 0x73);
  return (hi << 8) + lo;
}

void fetchFifo(int fd, int count)
{
  unsigned char data[32];
  int remaining = count;
  int fetched = 0;
  //printf("Asked for: %d \n", count);
  do
  {
    fetched = i2c_smbus_read_i2c_block_data(fd, 0x74, (count > 32 ? 32 : count), data);
    remaining -= fetched;
    //printf("Fetch returned: %d , remaining: %d \n", fetched, remaining);
  } while (remaining > 0 && fetched > 0);
}

int main(int argc, char* argv[])
{
  int fd;
  char fdname[] = "/dev/i2c-1";
  fd = open(fdname, O_RDWR);
  if (fd < 0) {
    printf("Opening the device failed. Errno is %d", errno);
    exit(1);
  }

  if (ioctl(fd, I2C_SLAVE, 0x68) < 0) {
    printf("Setting device address failed. Errno is %d", errno);
    exit(1);
  }
  
  i2c_smbus_write_byte_data(fd, 0x6B, 0x00); // wake up device
  i2c_smbus_write_byte_data(fd, 0x23, 0x40); // enable gyro X to FIFO
  i2c_smbus_write_byte_data(fd, 0x1A, 0x00); // set DLPF off
  i2c_smbus_write_byte_data(fd, 0x19, 3); // set sampling divider = x + 1
  i2c_smbus_write_byte_data(fd, 0x6A, 0x40); // enable FIFO

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
