#ifndef __RGBW_STRIP_HH
#define __RGBW_STRIP_HH

#include <stdexcept> // std::out_of_range
#include <sstream> // stringstream

#include "controller.hh"

class RGBWStrip {
public:
  RGBWStrip() {}
  ~RGBWStrip() {}

  static const int _R = 108;
  static const int _C =   8;
  
  int Rows() { return _R; }
  int R() { return _R; }
  int Columns () { return _C; }
  int C() { return _C; }

  // For the indices we need to flip the even columns
  RGBW& operator()(int c, int r) {
    int idx = _R*_C + 1;
    if((c % 2) == 0) {
      idx = (_R * c) + ((_R - 1) - r);
    } else {
      idx = (_R * c) + r;
    }
    if(idx >= _R*_C) {
      std::stringstream stream;
      stream << "LED out of bounds: " << idx << " " << c << ":" << r;
      throw std::out_of_range(stream.str());
    }
    return strip[idx];
  }

  RGBW operator()(int c, int r) const {
    int idx = _R*_C + 1;
    if((c % 2) == 0) {
      idx = (_R * c) + ((_R - 1) - r);
    } else {
      idx = (_R * c) + r;
    }
    if(idx >= _R*_C) throw new std::out_of_range("LED out of bounds");
    return strip[idx];
  }

  RGBW* GetLEDs() {
    return strip;
  }
  
protected:
  RGBW strip[_R*_C];
};

#endif
