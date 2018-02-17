#include <chrono>
#include <iostream>
#include <vector>

#ifdef __linux__
  #include <unistd.h>
  #include <sys/ioctl.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <stdint.h> 
  #include "driver/rgbw_strip.h"
#endif

#include "controller.hh"
#include "rgbw_strip.hh"
#include "spi.hh"
#include "mode.hh"

#include "modes/all_on.hh"
#include "modes/all_top.hh"

using namespace std;
using namespace std::chrono;

int kernel_strip_fd = -1;
void send_buffer(RGBWStrip &strip) {
#ifdef __linux__
  rgbw_led_t *leds = static_cast<rgbw_led_t*>(Strip.GetLEDs());
  ioctl(kernel_strip_fd, RGBW_STRIP_RENDER_ALL, leds);
#endif
}

struct ModeInfo {
  Mode *mode;
  ColorStatus color;

  ModeInfo(Mode *mode) : mode(mode) {
    color = mode->GetDefaultStatus();
  }
};

int main(int argc, char **argv) {
  bool done = false;
  std::vector<ModeInfo> lightModes;
  lightModes.push_back(ModeInfo(new AllOnMode()));
  lightModes.push_back(ModeInfo(new AllOnTopMode()));
  uint activeMode = 0;  
  
  // Create our strip and default all the LEDs to 0
  RGBWStrip strip;

  // Create our interface to the encoder and front panel button
  RGBW_SPIEncoder enc;

  // Need to set our initial time
  auto start = system_clock::now();
  bool wakeup = true; // True if we are activating a mode for the first time
  while(!done) {
    // Get the time now and calculate the delta
    auto now = system_clock::now();
    float ms = duration<float, std::milli>(now - start).count();
    if(activeMode < lightModes.size()) {
      bool send = lightModes[activeMode].mode->Render(strip, ms, wakeup);
      wakeup = false;
      start = system_clock::now();
      if(send) {
        send_buffer(strip);
      }
    }

    // Then check out input from the PIC
    RGBWEncoder vals = enc.GetStatus();    
    // Save the current values first
    if(activeMode < lightModes.size()) {
      lightModes[activeMode].color = vals.activeColor;
      lightModes[activeMode].mode->Update(vals.activeColor);
    }
    if(vals.button_held) {
      // Turn off here?
    } else if(vals.button_pressed) {
      // The button was pressed :: switch modes
      activeMode = (activeMode + vals.button_pressed) % lightModes.size();
      if(activeMode < lightModes.size()) {
        // Restore the previous values
        enc.SetColors(lightModes[activeMode].color);
        wakeup = true;
      }
    }
  }
}
