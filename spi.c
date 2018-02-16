#include "spi.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

int init_spi(void) {
  int spi_dev = open("/dev/spidev0.0", O_RDWR);
  int mode, bits, speed;
  if(spi_dev < 0) {
    fprintf(stderr, "Could not open SPI device\n");
    return spi_dev;
  }

  mode = SPI_MODE_0;
  if(ioctl(spi_dev, SPI_IOC_WR_MODE, &mode) < 0) {
    fprintf(stderr, "Could not write spi mode\n");
  }
  if(ioctl(spi_dev, SPI_IOC_RD_MODE, &mode) < 0) {
    fprintf(stderr, "Could not read spi mode\n");
  }

  bits = 8;
  if(ioctl(spi_dev, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
    fprintf(stderr, "Could not write bits per word\n");
  }
  if(ioctl(spi_dev, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
    fprintf(stderr, "Could not read bits per word\n");
  }

  speed = 6 * 1000;
  if(ioctl(spi_dev, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
    fprintf(stderr, "Could not write frequency\n");
  }
  if(ioctl(spi_dev, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
    fprintf(stderr, "Could not read frequency\n");
  }
  
  printf("Setup results -- Mode: %d, Bits: %d, Freq: %d\n", mode, bits, speed);

  return spi_dev;
}
