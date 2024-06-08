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

  /**
   * @brief Setup method for IMD_2002 sensor.
   *
   * This method is called once during the setup phase of the sensor.
   * It sends commands to the sensor to get the serial number and start acquisition.
   */
  void setup() override;

  /**
   * @brief Loop method for IMD_2002 component.
   *
   * This method is called repeatedly and automatically.
   * It reads data from the sensor via UART and stores it in a buffer.
   */
  void loop() override;

  /**
   * @brief Update method for IMD_2002 sensor.
   *
   * This method is inherited from the PollingComponent class and is called automatically at the specified
   * interval defined in the configuration file ("example.yaml").
   * It sends a command to read the target list, publishes a random state, and clears the buffer.
   */
  void update() override;

  // void dump_config() override;

  // custom methods
  void eval_frame(uint8_t *buffer);

 protected:
  std::vector<uint8_t> buffer_;

 private:
  unsigned char data_frame[512];
};

}  // namespace IMD_2002
}  // namespace esphome
