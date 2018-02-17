#ifndef __CONTROLLER_HH
#define __CONTROLLER_HH

#include <iostream>

struct RGBW {
  uint8_t r, g, b, w;
  RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) :r(r), g(g), b(b), w(w) {}
  RGBW() : r(0), g(0), b(0), w(0) {}
  bool operator==(const RGBW &o) const {
    if(r == o.r && g == o.g && b == o.b && w == o.w) return true;
    else return false;
  }
  friend std::ostream& operator<<(std::ostream &o, const RGBW &c) {
    o << "(" << (int)c.r << ":" <<
      (int)c.g << ":" <<
      (int)c.b << ":" <<
      (int)c.w << ")" << std::endl;
    return o;
  }
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
  bool operator==(const ColorStatus &other) const {
    if(active == other.active && color == other.color) return true;
    else return false;
  }
  bool operator!=(const ColorStatus &other) const{
    return !(*this == other);
  }
    
};

#endif

