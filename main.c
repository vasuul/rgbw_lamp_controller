
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "spi.h"

#define NUM_LEDS 5

int main(int argc, char ** argv) {
  int i;
  int spi_fd = init_spi();
  unsigned char spi_buf[8] = {0x00, 0x01, 0xF0, 0x0F, 0x12, 0x45, 12, 45};

  struct spi_ioc_transfer msg[1] = {0};
  msg[0].tx_buf = spi_buf;
  msg[0].rx_buf = spi_buf;
  msg[0].len = 8;

  ioctl(spi_fd, SPI_IOC_MESSAGE(1), msg);
  
  for(i = 0; i < 8; i++) {
    printf("\t0x%X\n", spi_buf[i]);
  }
  
  close(spi_fd);
    
  
  return 0;
}
