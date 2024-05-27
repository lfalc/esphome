#pragma once

#include <vector>

#include "../../core/component.h"
#include "../../components/sensor/sensor.h"
#include "../../components/uart/uart.h"
// #include "../../components/text_sensor/text_sensor.h"
#include "../../components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace IMD_2002 {

class IMD_2002 : public binary_sensor::BinarySensor, public Component {
  public:
  // IMD_2002(uart::UARTComponent *parent) : UARTDevice(parent) {}

  void setup() override;

  void loop() override;

  void dump_config() override;

  int readline(int readch, char *buffer, int len);
};

}  // namespace IMD_2002
}  // namespace esphome
