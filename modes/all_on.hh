#ifndef __ALL_ON_HH
#define __ALL_ON_HH

#include "../mode.hh"

class AllOnMode : public Mode {
public:
  AllOnMode() { color = RGBW(0, 0, 0, 0x80); }
  virtual ~AllOnMode() {}

  virtual void Update(ColorStatus &status) {
    // All we care about is the current color and its value
    switch(status.active) {
      case ActiveColor::R: color = RGBW(status.color.r, 0, 0, 0); break;
      case ActiveColor::G: color = RGBW(0, status.color.g, 0, 0); break;
      case ActiveColor::B: color = RGBW(0, 0, status.color.b, 0); break;
      case ActiveColor::W: color = RGBW(0, 0, 0, status.color.w); break;
    default:
      color = RGBW(0, 0, 0, 0x80); break;
    }
    changed = true;
  }
  
  virtual bool Render(RGBWStrip &strip, float msDelta, bool wakeup) {
    if(changed || wakeup) {
      RGBW color;
      for(int r = 0; r < strip.R(); r++) {
        for(int c = 0; c < strip.C(); c++) {
          strip(r, c) = color;
        }
      }
      return true;
    } else return false;
  }

  virtual ColorStatus GetDefaultStatus() {
    return ColorStatus(ActiveColor::W, 0x80, 0x80, 0x80, 0x80);
  }
  
protected:
  RGBW color;
  bool changed;
};

#endif
