#include "IMD_2002.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <cstdlib>
#include <stdint.h>
#include "InnoSenT.h"

#define IMD2002_MAX_TARGETS (15)

namespace esphome {
namespace IMD_2002 {

static const char *TAG = "IMD_2002";

void IMD_2002::setup() {
  // Get the serial number
  const uint8_t GET_SERIAL[] = {0x68, 0x05, 0x05, 0x68, 0x64, 0x01, 0xD2, 0x00, 0x00, 0x37, 0x16};
  this->write_array(GET_SERIAL, sizeof(GET_SERIAL));

  // Start acquisition 68 05 05 68 64 01 D1 00 00 36 16
  const uint8_t START_ACQUISITION[] = {0x68, 0x05, 0x05, 0x68, 0x64, 0x01, 0xD1, 0x00, 0x00, 0x36, 0x16};
  this->write_array(START_ACQUISITION, sizeof(START_ACQUISITION));
}

void IMD_2002::loop() {
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
  // Read target list 68 03 03 68 64 01 DA 3F 16
  ESP_LOGV(TAG, "Reading target list");
  const uint8_t READ_TARGET_LIST[] = {0x68, 0x03, 0x03, 0x68, 0x64, 0x01, 0xDA, 0x3F, 0x16};
  this->write_array(READ_TARGET_LIST, sizeof(READ_TARGET_LIST));

  // Evaluate the frame in the buffer
  ESP_LOGV(TAG, "Evaluating frame");
  eval_frame(this->buffer_.data());

  // publish state
  ESP_LOGV(TAG, "Publishing state");
  this->publish_state(std::rand() % 1000 / 1000.0f);

  // Free the buffer
  ESP_LOGV(TAG, "Clearing buffer");
  this->buffer_.clear();
}

void IMD_2002::eval_frame(uint8_t *data_frame) {
  static const char *TAG = "IMD_2002::eval_frame";
  IMD2002_Result_t result;
  IMD2002_TargetList_t targetList;

  ESP_LOGV(TAG, "data_frame: %x", data_frame);

  result = IMD2002_decodeTargetFrame(data_frame, &targetList);

  if (result != IMD2002_API_ERR_OK) {
    ESP_LOGE(TAG, "Error decoding target frame: %d", result);
    return;
  } else {
    ESP_LOGV(TAG, "Decoded target frame: %d", result);
  }
}

}  // namespace IMD_2002
}  // namespace esphome
