#ifndef __ALARM_CLOCK_HH
#define __ALARM_CLOCK_HH

#include "../mode.hh"
#include <ctime>

#include <iostream>

class AlarmClockMode : public Mode {

public:
  AlarmClockMode() {
    wakingUp= false;
    alarming = false;
    cleared = false;
    alarmHour = 6;
    alarmMinute = 0;
  }
  
  virtual ~AlarmClockMode() {}

  virtual void Update(ColorStatus &status) {
    alarming = false;
  }

  virtual bool Render(RGBWStrip & strip, float msDelta, bool wakeup) {
    
    if(wakeup) {
      wakingLevel = 0;
      wakingCount = 0;
      wakingUp = true;
    }
    
    float R = strip.R();
    float R2 = strip.R() * 2.0;
    
    if(wakingUp) {
      // spend 2ms in each row
      wakingCount += msDelta;
      if(wakingCount > 1) {
        wakingLevel++;
        wakingCount -= 1;
      }

      if(wakingLevel >= R2) {
        wakingUp = false;
      }
      
      
      float red = (R2 - wakingLevel) / R2;
      float blue = (wakingLevel - R2) / R2;
      float green = (wakingLevel < R) ?
        (R - wakingLevel) / R :
        (wakingLevel - R) / R;
      
      RGBW color = RGBW(red * 254, green * 254, blue * 254, 0);
      RGBW colorBlack = RGBW(0, 0, 0, 0);
      
      for(int r = 0; r < strip.R(); r++) {
        for(int c = 0; c < strip.C(); c++) {          
          if((r == wakingLevel) && wakingUp) {
            strip(c, r) = corColor(color, 1.0);
          } else if((r == wakingLevel-1 || r == wakingLevel+1) && wakingUp) {
            strip(c, r) = corColor(color, .75);
          } else if((r == wakingLevel-2 || r == wakingLevel+2) && wakingUp) {
            strip(c, r) = corColor(color, .5);
          } else if((r == wakingLevel-3 || r == wakingLevel+3) && wakingUp) {
            strip(c, r) = corColor(color, .25);
          }

          else if(r == (R2 - wakingLevel) && wakingUp) {
            strip(c, r) = corColor(color, 1.0);
          } else if((r == R2 - wakingLevel + 1 || r == R2 - wakingLevel - 1) && wakingUp) {
            strip(c, r) = corColor(color, .75);
          } else if((r == R2 - wakingLevel + 2 || r == R2 - wakingLevel - 2) && wakingUp) {
            strip(c, r) = corColor(color, .5);
          } else if((r == R2 - wakingLevel + 3 || r == R2 - wakingLevel - 3) && wakingUp) {
            strip(c, r) = corColor(color, .25);
          } 

          else if(c >= 0 && c < strip.C() && r >= 0 && r < strip.R()) {
            strip(c, r) = colorBlack;
          }
        }
      }
      
      return true;
    }

    // Check to see if the alarm went off
    std::time_t now = std::time(0);
    std::tm* now_tm = std::localtime(&now);
    int hour = now_tm->tm_hour;
    int minute = now_tm->tm_min;

    float dur_3 = alarmDuration / 3.0;
    float dur_2 = alarmDuration / 2.0;
    
    if(!alarming) {
      if(hour == alarmHour && minute == alarmMinute) {
        alarming = true;
        cleared = false;
      } else if(!cleared) {
        // Make sure all the LEDs are off
      }
    } else {
      // Now we just have to generate a patten to send based on the time since the
      //  alarm
      // TODO: This isn't going to work if the alarm crosses a day

      int dmin = (minute - alarmMinute) + 60*(hour - alarmHour);
      int seconds = now_tm->tm_sec + 60*dmin;

      bool changed = false;
      for(int r = 0; r < strip.R(); r++) {
        RGBW color(0, 0, 0, 0);
        {
          // RED
          //  Gradually light them up over the course of Duration/2 minutes
          //   Go from 0 to 255 in .7 * duration/2
          //   Go from 255 to 0 in .3 * duration/2
          float startAt = r * (dur_2 / strip.R());
          float level = 0.0;
	  float rise = .7 * dur_2;
	  float fall = .3 * dur_2;
          if(seconds > startAt && seconds < (startAt + rise)) {
            // Rising
            level = (seconds - startAt) / rise;
	    color.r = cor(level);
          } else if(seconds >= (startAt + rise) && seconds < startAt + dur_2) {
            // Falling
            level = (fall - (seconds - (startAt + rise))) / fall; 
	    color.r = cor(level);
          } else {
	    color.r = 0;
          }
        }
                
        {
          // WHITE
          //  Gradually light them up over the course of 2*(Duration/3) seconds
          //   Wait Duration/3 seconds first
	  float dur_3_2 = dur_3 * 2;
          float startAt = r * (dur_3 / strip.R()) + dur_3;
          float level = 0.0;
          if(seconds > startAt && seconds < (startAt + dur_3_2)) {
            // Rising
            level = (seconds - startAt) / dur_3_2;
	    color.w = cor(level);
          } else if(seconds <= startAt) {
	    color.w = 0x0;
	  } else {
	    color.w = 0xFF;
          }
        }
        
        for(int c = 0; c < strip.C(); c++) {
          if(strip(c, strip.R() - r - 1) != color) {            
            strip(c, strip.R() - r - 1) = color;
            changed = true;
          }
        }
      }
      return changed;
    }
    
    return false;
  }

  virtual ColorStatus GetDefaultStatus() {
    return ColorStatus(ActiveColor::R, 0x80, 0x80, 0x80, 0x80);
  }

protected:
  bool wakingUp, alarming, cleared;

  int alarmHour;
  int alarmMinute;

  int wakingLevel;
  float wakingCount;

  unsigned char cor(float a) const {
    if(a > 1.0) a = 1.0;
    if(a < 0.0) a = 0.0;

    unsigned char val = gamma8[(int)(a * 255)];
    if(val == 0) val = 1;
    return val;
  }

  RGBW corColor(const RGBW &c, float a) const {
    if(a < 0) a = 0;
    if(a > 1.0) a = 1.0;
    
    return RGBW(gamma8[(int)(a * c.r)], gamma8[(int)(a * c.g)],
                gamma8[(int)(a * c.b)], gamma8[(int)(a * c.w)]
      );
  }

  const int alarmDuration = (60 * 40); // In seconds

  const unsigned char gamma8[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
    115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
    144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
    177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
    215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
};

#endif
