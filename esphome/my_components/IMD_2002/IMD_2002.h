#pragma once

#include <vector>

#include "../../core/component.h"
#include "../../components/sensor/sensor.h"
#include "../../components/uart/uart.h"
// #include "../../components/text_sensor/text_sensor.h"
// #include "../../components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace IMD_2002 {

class IMD_2002 : public sensor::Sensor, public PollingComponent, public uart::UARTDevice {
 public:
  // Inherited methods
  void setup() override;

  void loop() override;

  void update() override;

  // void dump_config() override;

  // custom methods
  int read_frame(int readch, char *buffer, int len);

 protected:
  std::vector<uint8_t> buffer_;

 private:
};

}  // namespace IMD_2002
}  // namespace esphome
