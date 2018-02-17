#ifndef __ALL_TOP_HH
#define __ALL_TOP_HH

#include "../mode.hh"

class AllOnTopMode : public Mode {
public:
  AllOnTopMode(int height = 16) : active(ActiveColor::B), height(height) { colorVal = 0x80; }
  virtual ~AllOnTopMode() {}

  virtual void Update(ColorStatus &status) {
    // All we care about is the current color and its value
    switch(status.active) {
    case ActiveColor::R: colorVal = status.color.r; break;
    case ActiveColor::G: colorVal = status.color.g; break;
    case ActiveColor::B: colorVal = status.color.b; break;
    case ActiveColor::W: colorVal = status.color.w; break;
    default:
      colorVal = 0x80;
      status.active = ActiveColor::B;
      break;
    }
    active = status.active;
    changed = true;
  }
  
  virtual bool Render(RGBWStrip &strip, float msDelta, bool wakeup) {
    if(changed || wakeup) {
      RGBW color, whiteColor = RGBW(0, 0, 0, colorVal);
      
      switch(active) {
      case ActiveColor::R: color = RGBW(colorVal, 0, 0, 0); break;
      case ActiveColor::G: color = RGBW(0, colorVal, 0, 0); break;
      case ActiveColor::B: color = RGBW(0, 0, colorVal, 0); break;
      case ActiveColor::W: color = RGBW(0, 0, 0, colorVal); break;
      default:
        color = RGBW(0, 0, 0x80, 0);
      }

      for(int c = 0; c < strip.C(); c++) {
        for(int r = 0; r < strip.R(); r++) {
          if(r < height) strip(c, r) = color;
          else           strip(c, r) = whiteColor;
        }
      }
      changed = false;
      return true;
    } else return false;
  }

  virtual ColorStatus GetDefaultStatus() {
    return ColorStatus(ActiveColor::B, 0x80, 0x80, 0x80, 0x80);
  }
  
protected:
  ActiveColor active;
  int height;
  uint8_t colorVal;
  bool changed;
};

#endif
