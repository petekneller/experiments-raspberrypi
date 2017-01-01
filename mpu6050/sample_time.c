#include  <wiringPiI2C.h>
#include  <stdio.h>
#include  <math.h>
#include  <time.h>

int fd;

int read_word_2c(int addr)
{
  int val;
  val = wiringPiI2CReadReg8(fd, addr);
  val = val << 8;
  val += wiringPiI2CReadReg8(fd, addr+1);
  if (val >= 0x8000)
    val = -(65536 - val);

  return val;
}

int take_sample()
{
  return read_word_2c(0x3B);
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    printf("Requires 1 argument - the number of samples to take\n");
    return 1;
  }

  fd = wiringPiI2CSetup (0x68);
  wiringPiI2CWriteReg8 (fd,0x6B,0x00); //disable sleep mode 
  
  clock_t previous_time = clock();
  int N = atoi(argv[1]);
  double times[N];
  int i;
  for (i = 0; i < N; i++)
  {
    take_sample();
    clock_t now = clock();
    times[i] = ((double) (now - previous_time)) / CLOCKS_PER_SEC;
    previous_time = now; 
  }

  for (i = 0; i < N; i++)
  {
    printf("%e\n", times[i]);
  }
  
  return 0;
}
