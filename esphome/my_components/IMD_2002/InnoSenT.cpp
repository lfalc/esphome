#include <string.h>
#include <cstdlib>
#include "InnoSenT.h"
#include <stdint.h>
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

#define IMD2002_MAX_TARGETS (3)

namespace esphome {
namespace IMD_2002 {
/***********************************************************************
Function: decodes target list frame received from IMD-2002.
Input arguments:
- Frame array: array with from IMD2002 received target list frame
Output arguments:
- targetList: struct with decoded target list
Return value:
- ErrorCode
***********************************************************************/
IMD2002_Result_t IMD2002_decodeTargetFrame(unsigned char *frame_array, IMD2002_TargetList_t *targetList) {
  static const char *TAG = "IMD2002_decodeTargetFrame";

  // ESP_LOGV(TAG, "Decoding target frame: %s", frame_array);

  // ESP_LOGV(TAG, "Initializing locals...");
  uint16_t ui16_fc;
  uint16_t ui16_nrOfTargets;
  uint8_t *pData;
  uint32_t ui32_tmp;
  uint16_t ui16_tmp;
  uint16_t ui16_targetCounter;
  // ESP_LOGV(TAG, "Done.");

  // ESP_LOGV(TAG, "Checking frame ...");
  if (frame_array[0] == 0x68) /* check SD2 Frame */
  {
    return IMD2002_API_ERR_COMMAND_NO_VALID_FRAME_FOUND; /* variable length frames not supported
                                                          */
  } else {
    ui16_fc = 3; /* set function code bit for fixed length frames */
  }
  ui16_nrOfTargets =
      ((uint16_t) (frame_array[ui16_fc + 1] & 0x00ff) << 8) + (uint16_t) (frame_array[ui16_fc + 2] & 0x00ff);
  // ESP_LOGV(TAG, "Number of targets: %d", ui16_nrOfTargets);
    
  if (ui16_nrOfTargets > IMD2002_MAX_TARGETS) {
    return IMD2002_API_ERR_COMMAND_FAILURE;
  }
  targetList->ui16_nrOfTargets = ui16_nrOfTargets;
  targetList->ui16_targetListId =
      ((uint16_t) (frame_array[ui16_fc + 3] & 0x00ff) << 8) + (uint16_t) (frame_array[ui16_fc + 4] & 0x00ff);
  targetList->ui8_blockageDetected = frame_array[ui16_fc + 5];
  targetList->ui8_blockageLevel = frame_array[ui16_fc + 6];

  targetList->ui16_reserved2 =
      ((uint16_t) (frame_array[ui16_fc + 7] & 0x00ff) << 8) + (uint16_t) (frame_array[ui16_fc + 8] & 0x00ff);
  pData = &frame_array[ui16_fc + 9];
  for (ui16_targetCounter = 0u; ui16_targetCounter < ui16_nrOfTargets; ui16_targetCounter++) {
    ui32_tmp = (((*pData++) & 0x000000ff) << 24);
    ui32_tmp |= (((*pData++) & 0x000000ff) << 16);
    ui32_tmp |= (((*pData++) & 0x000000ff) << 8);
    ui32_tmp |= ((*pData++) & 0x000000ff);
    targetList->target[ui16_targetCounter].f32_range_m = *(float *) &ui32_tmp;
    ui32_tmp = (((*pData++) & 0x000000ff) << 24);
    ui32_tmp |= (((*pData++) & 0x000000ff) << 16);
    ui32_tmp |= (((*pData++) & 0x000000ff) << 8);
    ui32_tmp |= ((*pData++) & 0x000000ff);
    targetList->target[ui16_targetCounter].f32_velocity_mps = *(float *) &ui32_tmp;
    ui32_tmp = (((*pData++) & 0x000000ff) << 24);
    ui32_tmp |= (((*pData++) & 0x000000ff) << 16);
    ui32_tmp |= (((*pData++) & 0x000000ff) << 8);
    ui32_tmp |= ((*pData++) & 0x000000ff);
    targetList->target[ui16_targetCounter].f32_signal_dB = *(float *) &ui32_tmp;
    ui32_tmp = (((*pData++) & 0x000000ff) << 24);
    ui32_tmp |= (((*pData++) & 0x000000ff) << 16);
    ui32_tmp |= (((*pData++) & 0x000000ff) << 8);
    ui32_tmp |= ((*pData++) & 0x000000ff);
    targetList->target[ui16_targetCounter].f32_estimatedTimeOfArrival_s = *(float *) &ui32_tmp;
    ui32_tmp = (((*pData++) & 0x000000ff) << 24);
    ui32_tmp |= (((*pData++) & 0x000000ff) << 16);
    ui32_tmp |= (((*pData++) & 0x000000ff) << 8);
    ui32_tmp |= ((*pData++) & 0x000000ff);
    targetList->target[ui16_targetCounter].f32_incidentAngle_deg = *(float *) &ui32_tmp;
  }
  return IMD2002_API_ERR_OK;
}
}  // namespace IMD_2002
}  // namespace esphome