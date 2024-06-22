#include "IMD_2002.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <cstdlib>
#include <stdint.h>
#include "InnoSenT.h"

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
  while (this->available()) {
    uint8_t val;
    this->read_byte(&val);
    // ESP_LOGV(TAG, "Read byte: %d", val);  // Log the read byte
    this->buffer_.push_back(val);
    if (val == 0x16) {
      // ESP_LOGV(TAG, "data_frame end detected");
      buffer_.push_back('\0');
      // ESP_LOGV(TAG, "buffer content: %s", (reinterpret_cast<const char *>(this->buffer_.data())));

      std::copy(this->buffer_.begin(), this->buffer_.end(), data_frame);
      this->buffer_.clear();
      break;
    }
  }
}

void IMD_2002::update() {
  ESP_LOGV(TAG, "data_frame content: %s", data_frame);

  // Read target list 68 03 03 68 64 01 DA 3F 16
  ESP_LOGV(TAG, "Reading target list");
  const uint8_t READ_TARGET_LIST[] = {0x68, 0x03, 0x03, 0x68, 0x64, 0x01, 0xDA, 0x3F, 0x16};
  this->write_array(READ_TARGET_LIST, sizeof(READ_TARGET_LIST));

  // Evaluate the frame in the buffer
  // ESP_LOGV(TAG, "Evaluating frame");
  eval_frame(this->data_frame);

  // publish state
  // ESP_LOGV(TAG, "Publishing state");
  // this->publish_state(std::rand() % 1000 / 1000.0f);
}

void IMD_2002::eval_frame(unsigned char *data_frame) {
  static const char *TAG = "IMD_2002::eval_frame";
  IMD2002_Result_t result;
  IMD2002_TargetList_t targetList;

  // data_frame[255] = '\0';
  // ESP_LOGV(TAG, "data_frame content: %s", data_frame);

  result = IMD2002_decodeTargetFrame(data_frame, &targetList);

  if (result != IMD2002_API_ERR_OK) {
    ESP_LOGE(TAG, "Error decoding target frame: %d", result);
    return;
  } else {
    ESP_LOGV(TAG, "ui16_nrOfTargets: %d", targetList.ui16_nrOfTargets);
    ESP_LOGV(TAG, "ui16_targetListId: %d", targetList.ui16_targetListId);
    ESP_LOGV(TAG, "ui8_blockageDetected: %d", targetList.ui8_blockageDetected);
    ESP_LOGV(TAG, "ui8_blockageLevel: %d", targetList.ui8_blockageLevel);
    ESP_LOGV(TAG, "ui16_reserved2: %d", targetList.ui16_reserved2);

    if (targetList.ui16_nrOfTargets > 0) {
      float f32_strongest_signal_dB = -100.0f;
      for (uint16_t i = 0; i < targetList.ui16_nrOfTargets; i++) {
        if (targetList.target[i].f32_signal_dB > f32_strongest_signal_dB) {
          f32_strongest_signal_dB = targetList.target[i].f32_signal_dB;
        }
      }
      ESP_LOGV(TAG, "Strongest signal: %f dB", f32_strongest_signal_dB);
      this->publish_state(f32_strongest_signal_dB);
    } else {
      ESP_LOGV(TAG, "No targets detected");
      this->publish_state(NAN);
    }
  }
}

}  // namespace IMD_2002
}  // namespace esphome
