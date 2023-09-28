#pragma once

#include "esphome/components/climate_ir/climate_ir.h"

namespace esphome {
namespace daikin_3p5939722e {

// Values for Daikin 3P593972-2E
// Temperature
const uint8_t DAIKIN_TEMP_MIN = 18;  // Celsius
const uint8_t DAIKIN_TEMP_MAX = 32;  // Celsius

// Modes
const uint8_t DAIKIN_MODE_AUTO = 0x00;
const uint8_t DAIKIN_MODE_DRY = 0x20;
const uint8_t DAIKIN_MODE_COOL = 0x30;
const uint8_t DAIKIN_MODE_HEAT = 0x40;
const uint8_t DAIKIN_MODE_FAN = 0x60;
const uint8_t DAIKIN_MODE_OFF = 0x00;
const uint8_t DAIKIN_MODE_ON = 0x01;

// Fan Speed
const uint8_t DAIKIN_FAN_1 = 0x30;
const uint8_t DAIKIN_FAN_3 = 0x50;
const uint8_t DAIKIN_FAN_5 = 0x70;
const uint8_t DAIKIN_FAN_AUTO = 0xA0;
const uint8_t DAIKIN_FAN_QUIET = 0xB0;

// Swing mode
const uint8_t DAIKIN_SWING_VERTICAL = 0x0F;
const uint8_t DAIKIN_SWING_OFF = 0x00;

// Powerful and sleep mode
const uint8_t DAIKIN_POWERFUL_ON = 0x01;
const uint8_t DAIKIN_SLEEP_ON = 0x04;
const uint8_t DAIKIN_POWERFUL_SLEEP_OFF = 0x00;

// Eco mode
const uint8_t DAIKIN_ECO_ON = 0x04;
const uint8_t DAIKIN_ECO_OFF = 0x00;

// IR Transmission
const uint32_t DAIKIN_IR_FREQUENCY = 38000;
const uint32_t DAIKIN_HEADER_MARK = 3360;
const uint32_t DAIKIN_HEADER_SPACE = 1760;
const uint32_t DAIKIN_BIT_MARK = 520;
const uint32_t DAIKIN_ONE_SPACE = 1370;
const uint32_t DAIKIN_ZERO_SPACE = 360;
const uint32_t DAIKIN_MESSAGE_SPACE = 32300;

// State Frame size
const uint8_t DAIKIN_STATE_FRAME_SIZE = 19;

class Dakin3p5939722eClimate : public climate_ir::ClimateIR {
 public:
  Dakin3p5939722eClimate()
      : climate_ir::ClimateIR(DAIKIN_TEMP_MIN, DAIKIN_TEMP_MAX, 1.0f, true, true,
                              {climate::CLIMATE_FAN_AUTO, climate::CLIMATE_FAN_QUIET, climate::CLIMATE_FAN_LOW,
                               climate::CLIMATE_FAN_MEDIUM, climate::CLIMATE_FAN_HIGH},
                              {climate::CLIMATE_SWING_OFF, climate::CLIMATE_SWING_VERTICAL},
                              {climate::CLIMATE_PRESET_NONE, climate::CLIMATE_PRESET_BOOST, climate::CLIMATE_PRESET_ECO,
                               climate::CLIMATE_PRESET_SLEEP, climate::CLIMATE_PRESET_COMFORT}) {}

 protected:
  // Transmit via IR the state of this climate controller.
  void transmit_state() override;
  uint8_t operation_mode_();
  uint8_t fan_speed_();
  uint8_t swing_();
  uint8_t temperature_();
  uint8_t powerful_sleep_preset_();
  uint8_t eco_preset_();

};

}  // namespace daikin_3p5939722e
}  // namespace esphome
