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
  RGBW_SPIEncoder(const std::string &path = "/dev/spidev0.0", int speed = 5*1000);
  ~RGBW_SPIEncoder();

  int Speed() const { return _speed; }
  int Bits() const { return _bits; }
  int Mode() const { return _mode; }
  
  void SetColors(const ColorStatus &status);
  void SetButton(bool status);
  RGBWEncoder GetStatus();
  
protected:
  void SendMessage(uint8_t spi_buf[8]);
  int spi_dev, _mode, _speed, _bits;
};

#endif
