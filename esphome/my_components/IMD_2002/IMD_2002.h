#pragma once

#include <vector>

#include "../../esphome/core/component.h"
#include "../../esphome/components/sensor/sensor.h"
#include "../../esphome/components/uart/uart.h"

namespace esphome {
namespace IMD_2002 {

class IMD_2002Component : public sensor::Sensor, public Component, public uart::UARTDevice {
 public:
  // ========== INTERNAL METHODS ==========
  void loop() override;
  void dump_config() override;

 protected:
  void check_buffer_();

  std::vector<uint8_t> buffer_;
};

}  // namespace IMD_2002
}  // namespace esphome
