#ifndef __RGBW_SPI_ENCODER_HH
#define __RGBW_SPI_ENCODER_HH

#include <string>

#include "controller.hh"

struct RGBWEncoder {
  ColorStatus activeColor;
  uint8_t button_pressed;
  bool button_held;
};

class RGBW_SPIEncoder {
public:
  RGBW_SPIEncoder(std::string path = "/dev/spidev0.0", int speed = 6*1000);
  ~RGBW_SPIEncoder();

  int Speed() const { return _speed; }
  int Bits() const { return _bits; }
  int Mode() const { return _mode; }
  
  void SetColors(uint8_t active, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
  void SetButton(bool status);
  RGBWEncoder GetStatus();
  
protected:
  void SendMessage(uint8_t spi_buf[8]);
  int spi_dev, _mode, _speed, _bits;
};

#endif
