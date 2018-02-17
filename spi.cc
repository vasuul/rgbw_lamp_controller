#include "spi.hh"


#ifdef __linux__
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <sys/ioctl.h>
  #include <linux/types.h>
  #include <linux/spi/spidev.h>
#endif

#define STATUS_CMD 0
#define LEDS_CMD   1
#define BTN_CMD    2

#define BTN_NDX    2
#define ACTIVE_NDX 3
#define R_NDX      4
#define G_NDX      5
#define B_NDX      6
#define W_NDX      7

RGBW_SPIEncoder::RGBW_SPIEncoder(std::string path, int speed) {
#ifdef __linux__
  spi_dev = open(path.c_str(), O_RDWR);
  int mode, bits, speed;
  if(spi_dev < 0) {
    return;
  }

  mode = SPI_MODE_0;
  if(ioctl(spi_dev, SPI_IOC_WR_MODE, &mode) < 0) {
    close(spi_dev); spi_dev = -1; return;
  }
  if(ioctl(spi_dev, SPI_IOC_RD_MODE, &mode) < 0) {
  } else _mode = mode;

  bits = 8;
  if(ioctl(spi_dev, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
    close(spi_dev); spi_dev = -1; return;
  }
  if(ioctl(spi_dev, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
  } else _bits = bits;

  if(ioctl(spi_dev, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
    close(spi_dev); spi_dev = -1; return;
  }
  if(ioctl(spi_dev, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
  } else _speed = speed;
#else
  spi_dev = -1;
  _speed = speed;
  _mode = 0;
  _bits = 8;
#endif
}

RGBW_SPIEncoder::~RGBW_SPIEncoder() {
#ifdef __linux__
  if(spi_dev >= 0) { close(spi_dev); spi_dev = -1; }
#endif
}

void RGBW_SPIEncoder::SendMessage(uint8_t spi_buf[8]) {
#ifdef __linux__
  struct spi_ioc_transfer msg[1] = {0};
  msg[0].tx_buf = spi_buf;
  msg[0].rx_buf = spi_buf;
  msg[0].len = 8;
  ioctl(spi_fd, SPI_IOC_MESSAGE(1), msg);
#else
  // Do something here based on the command type
#endif
}

void RGBW_SPIEncoder::SetColors(const ColorStatus &status) {
  uint8_t active = 4;
  switch(status.active) {
  case ActiveColor::R: active = 0; break; 
  case ActiveColor::G: active = 1; break;
  case ActiveColor::B: active = 2; break;
  case ActiveColor::W: active = 3; break;
  default: active = 4; break;    
  }
  uint8_t spi_msg[8] = { LEDS_CMD, active,
                         status.color.r,
                         status.color.g,
                         status.color.b,
                         status.color.w,
                         0, 0 };
  SendMessage(spi_msg);
}

void RGBW_SPIEncoder::SetButton(bool status) {
  uint8_t spi_msg[8] = { BTN_CMD, status, 0, 0, 0, 0, 0 };
  SendMessage(spi_msg);
}

RGBWEncoder RGBW_SPIEncoder::GetStatus() {
  RGBWEncoder ret;
  uint8_t spi_msg[8] = { STATUS_CMD, 0, 0, 0, 0, 0, 0 };
  SendMessage(spi_msg);
  if(spi_msg[BTN_NDX] == 255) {
    ret.button_held = true;
  } else {
    ret.button_pressed = spi_msg[BTN_NDX];
  }
  switch(spi_msg[ACTIVE_NDX]) {
  case 0: ret.activeColor.active = ActiveColor::R;   break;
  case 1: ret.activeColor.active = ActiveColor::G;   break;
  case 2: ret.activeColor.active = ActiveColor::B;   break;
  case 3: ret.activeColor.active = ActiveColor::W;   break;
  case 4: ret.activeColor.active = ActiveColor::Off; break;
  }
  ret.activeColor.color.r = spi_msg[R_NDX];
  ret.activeColor.color.g = spi_msg[G_NDX];
  ret.activeColor.color.b = spi_msg[B_NDX];
  ret.activeColor.color.w = spi_msg[W_NDX];

  return ret;
}
