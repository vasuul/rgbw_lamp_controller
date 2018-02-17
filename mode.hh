#ifndef __MODE_HH
#define __MODE_HH

#include "rgbw_strip.hh"
#include "controller.hh"

class Mode {
public:
  Mode() {}
  virtual ~Mode() {}

  virtual void Update(ColorStatus &status) = 0;
  virtual bool Render(RGBWStrip &strip, float msDelta, bool wakeup) = 0;
  virtual ColorStatus GetDefaultStatus() {
    return ColorStatus(ActiveColor::Off, 0, 0, 0, 0);
  }

protected:
};

#endif
