#ifndef __CONTROLLER_HH
#define __CONTROLLER_HH

struct RGBW {
  uint8_t r, g, b, w;
  RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) :r(r), g(g), b(b), w(w) {}
  RGBW() : r(0), g(0), b(0), w(0) {}
};

enum class ActiveColor {
  R, G, B, W, Off
};

struct ColorStatus {
  ActiveColor active;
  RGBW color;
  

  ColorStatus(ActiveColor a, uint8_t r, uint8_t g, uint8_t b, uint8_t w) :
    active(a), color(r, g, b, w) {}
  ColorStatus() : active(ActiveColor::Off), color() {}
};

#endif

