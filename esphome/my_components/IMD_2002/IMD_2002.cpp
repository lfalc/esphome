// Datasheet https://wiki.dfrobot.com/A01NYUB%20Waterproof%20Ultrasonic%20Sensor%20SKU:%20SEN0313

#include "IMD_2002.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace IMD_2002 {

static const char *const TAG = "IMD_2002.sensor";

void IMD_2002::setup() {}

void IMD_2002::loop() {
  // Read the sensor
  const int max_line_length = 80;
  static char buffer[max_line_length];
  while (available()) {
    if (readline(read(), buffer, max_line_length) > 0) {
      publish_state(buffer);
    }
    // Evaluate the message

    // Publish the state
    if (this->state == true) {
      this->publish_state(false);
    } 
    else {
      this->publish_state(true);
    }
  }
}

int IMD_2002::readline(int readch, char *buffer, int len) {
  static int pos = 0;
  int rpos;

  if (readch > 0) {
    switch (readch) {
      case '\n':  // Ignore new-lines
        break;
      case '\r':  // Return on CR
        rpos = pos;
        pos = 0;  // Reset position index ready for next time
        return rpos;
      default:
        if (pos < len - 1) {
          buffer[pos++] = readch;
          buffer[pos] = 0;
        }
    }
  }
  // No end of line has been found, so return -1.
  return -1;
}
}  // namespace IMD_2002
}  // namespace esphome
