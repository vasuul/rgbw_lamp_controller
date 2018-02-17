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


using namespace std;
using namespace std::chrono;

int kernel_strip_fd = -1;
void send_buffer(RGBWStrip<> &strip) {
#ifdef __linux__
  rgbw_led_t *leds = static_cast<rgbw_led_t*>(Strip.GetLEDs());
  ioctl(kernel_strip_fd, RGBW_STRIP_RENDER_ALL, leds);
#endif
}

int main(int argc, char **argv) {
  bool done = false;
  std::vector<Mode> lightModes;
  int activeMode = -1;
  
  // Create our strip and default all the LEDs to 0
  RGBWStrip<> strip;

  // Create our interface to the encoder and front panel button
  RGBW_SPIEncoder enc;

  // Need to set our initial time
  auto start = system_clock::now();
  bool wakeup = true; // True if we are activating a mode for the first time
  while(!done) {
    // Get the time now and calculate the delta
    auto now = system_clock::now();
    float ms = duration<float, std::milli>(now - start).count();
    if(activeMode >= 0 && activeMode < lightModes.size()) {
      // Set all the leds in the strip
      bool send = lightModes[activeMode].Render(strip, ms, wakeup);
      wakeup = false;
      start = system_clock::now();
      if(send) {
        send_buffer(strip);
      }
    }

    // Then check out input from the PIC
    RGBWEncoder vals = enc.GetStatus();
    if(vals.button_held) {

    } else if(vals.button_pressed) {

    }
  }
}
