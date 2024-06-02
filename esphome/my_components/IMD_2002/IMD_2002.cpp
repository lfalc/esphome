#include "IMD_2002.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <cstdlib>
#include <string.h>
#include "InnoSenT.h"

#define IMD2002_MAX_TARGETS (15)

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef int sint16_t;
typedef unsigned long uint32_t;
typedef long sint32_t;
typedef float float32_t;

typedef enum {
  IMD2002_API_ERR_OK = 0x0000,
  IMD2002_API_ERR_FUNCTION_DEPRECATED,
  IMD2002_API_ERR_DLL_NOT_FINISHED,
  IMD2002_API_ERR_HANDLE_NOT_INITIALIZED,
  IMD2002_API_ERR_COMPORT_DOESNT_EXIST,
  IMD2002_API_ERR_COMPORT_CANT_INITIALIZE,
  IMD2002_API_ERR_COMPORT_ACCESS_DENIED,
  IMD2002_API_ERR_COMPORT_BAUDRATE_NOT_VALID,
  IMD2002_API_ERR_COMPORT_CANT_OPEN,
  IMD2002_API_ERR_COMPORT_CANT_SET_FLOW_CONTROL,
  IMD2002_API_ERR_COMPORT_CANT_SET_PARITY,
  IMD2002_API_ERR_COMPORT_CANT_SET_STOP_BITS,
  IMD2002_API_ERR_COMPORT_CANT_SET_DATA_BITS,
  IMD2002_API_ERR_COMPORT_CANT_SET_BAUDRATE,
  IMD2002_API_ERR_COMPORT_ALREADY_INITIALIZED,
  IMD2002_API_ERR_COMPORT_EQUALS_NULL,
  IMD2002_API_ERR_COMPORT_NOT_OPEN,
  IMD2002_API_ERR_COMPORT_NOT_READABLE,
  IMD2002_API_ERR_COMPORT_NOT_WRITEABLE,
  IMD2002_API_ERR_COMPORT_CANT_WRITE,
  IMD2002_API_ERR_COMPORT_CANT_READ,
  IMD2002_API_ERR_COMMAND_NOT_WRITTEN,
  IMD2002_API_ERR_COMMAND_NOT_READ,
  IMD2002_API_ERR_COMMAND_NO_DATA_RECEIVED,
  IMD2002_API_ERR_COMMAND_NO_VALID_FRAME_FOUND,
  IMD2002_API_ERR_COMMAND_RX_FRAME_DAMAGED,
  IMD2002_API_ERR_COMMAND_FAILURE,
  IMD2002_API_ERR_UNDEFINED_READ,
  IMD2002_API_ERR_COMPORT_LESS_DATA_READ,
  IMD2002_API_ERR_COMPORT_SYSTEM_INIT_FAILED,
  IMD2002_API_ERR_COMPORT_SYSTEM_ALREADY_INITIALIZED,
  IMD2002_API_ERR_COMMAND_RX_FRAME_LENGTH,
  IMD2002_API_ERR_COMMAND_MAX_DATA_OVERFLOW,
  IMD2002_API_ERR_COMMAND_MAX_IQPAIRS_OVERFLOW,
  IMD2002_API_ERR_COMMAND_NOT_ACCEPTED,
  IMD2002_API_ERR_NULL_POINTER,
  IMD2002_API_ERR_PARAMETER_OUT_OF_RANGE,
  IMD2002_API_ERR_COMMAND_UNEXPECTED_FRAMETYPE,
  IMD2002_API_ERR_COMMAND_WITH_FAILURE_CODE
} IMD2002_Result_t;

typedef struct {
  uint16_t ui16_nrOfTargets;
  uint16_t ui16_targetListId;
  uint8_t ui8_blockageDetected;
  uint8_t ui8_blockageLevel;
  uint16_t ui16_reserved2;
  IMD2002_Target_t target[IMD2002_MAX_TARGETS];
} IMD2002_TargetList_t;

namespace esphome {
namespace IMD_2002 {

/**
 * @brief Setup method for IMD_2002 sensor.
 * 
 * This method is called once during the setup phase of the sensor.
 * It sends commands to the sensor to get the serial number and start acquisition.
 */
void IMD_2002::setup() {
  // Get the serial number
  const uint8_t GET_SERIAL[] = {0x68, 0x05, 0x05, 0x68, 0x64, 0x01, 0xD2, 0x00, 0x00, 0x37, 0x16};
  this->write_array(GET_SERIAL, sizeof(GET_SERIAL));

  // Start acquisition 68 05 05 68 64 01 D1 00 00 36 16
  const uint8_t START_ACQUISITION[] = {0x68, 0x05, 0x05, 0x68, 0x64, 0x01, 0xD1, 0x00, 0x00, 0x36, 0x16};
  this->write_array(START_ACQUISITION, sizeof(START_ACQUISITION));
}

/**
 * @brief Loop method for IMD_2002 component.
 * 
 * This method is called repeatedly and automatically.
 * It reads data from the sensor via UART and stores it in a buffer.
 */
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

/**
 * @brief Update method for IMD_2002 sensor.
 * 
 * This method is inherited from the PollingComponent class and is called automatically at the specified
 * interval defined in the configuration file ("example.yaml").
 * It sends a command to read the target list, publishes a random state, and clears the buffer.
 */
void IMD_2002::update() {
  // Read target list 68 03 03 68 64 01 DA 3F 16
  const uint8_t READ_TARGET_LIST[] = {0x68, 0x03, 0x03, 0x68, 0x64, 0x01, 0xDA, 0x3F, 0x16};
  this->write_array(READ_TARGET_LIST, sizeof(READ_TARGET_LIST));

  // publish state
  this->publish_state(std::rand() % 1000 / 1000.0f);

  // Free the buffer
  this->buffer_.clear();
}

void IMD_2002::read_frame(int readch, char *buffer, int len) {
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
