#include "IMD_2002.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <cstdlib>
#include <string.h>

namespace esphome {
namespace IMD_2002 {

static const char *const TAG = "IMD_2002.sensor";

void IMD_2002::setup() {
  // Get the serial number
  const uint8_t GET_SERIAL[] = {0x68, 0x05, 0x05, 0x68, 0x64, 0x01, 0xD2, 0x00, 0x00, 0x37, 0x16};
  this->write_array(GET_SERIAL, sizeof(GET_SERIAL));

  // Start acquisition 68 05 05 68 64 01 D1 00 00 36 16
  const uint8_t START_ACQUISITION[] = {0x68, 0x05, 0x05, 0x68, 0x64, 0x01, 0xD1, 0x00, 0x00, 0x36, 0x16};
  this->write_array(START_ACQUISITION, sizeof(START_ACQUISITION));
}

void IMD_2002::loop() {
  // The loop method is called repeatedly and automatically. Therefore, as soon as data is received via UART, it is copied to the buffer.
  while (this->available() > 0) {
    uint8_t data;
    this->read_byte(&data);

    // ESP_LOGV(TAG, "Read byte from sensor: %x", data);

    if (this->buffer_.empty())
      continue;

    this->buffer_.push_back(data);
    // if (this->buffer_.size() == 4)
    //   this->check_buffer_();
  }
}

void IMD_2002::update() {
  // The update method is inherited from the PollingComponent class and is called automatically at the specified interval defined in the configuration file ("example.yaml").

  // Read target list 68 03 03 68 64 01 DA 3F 16
  const uint8_t READ_TARGET_LIST[] = {0x68, 0x03, 0x03, 0x68, 0x64, 0x01, 0xDA, 0x3F, 0x16};
  this->write_array(READ_TARGET_LIST, sizeof(READ_TARGET_LIST));

  // publish state
  this->publish_state(std::rand() % 1000 / 1000.0f);

  // Free the buffer
  this->buffer_.clear();
}

int IMD_2002::read_frame(int readch, char *buffer, int len) {
  // Read the sensor
  // const int max_line_length = 80;
  // static char buffer[max_line_length];

  // while (available()) {
  //   if (readline(read(), buffer, max_line_length) > 0) {
  //     publish_state(buffer);
  //   }
  // Evaluate the message

  // Publish the state
  // if (this->state == true) {
  //   this->publish_state(false);
  // } else {
  //   this->publish_state(true);
  // }
  return -1;
}
}  // namespace IMD_2002
}  // namespace esphome
