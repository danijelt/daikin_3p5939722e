#include "daikin_3p5939722e.h"
#include "esphome/components/remote_base/remote_base.h"

namespace esphome {
namespace daikin_3p5939722e {

static const char *const TAG = "daikin.climate";

void Dakin3p5939722eClimate::transmit_state() {
  uint8_t remote_state[35] = {0x11, 0xDA, 0x27, 0x00, 0xC5, 0x00, 0x00, 0xD7,
                              0x11, 0xDA, 0x27, 0x00, 0x42, 0x49, 0x05, 0xA2,
                              0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0xC5, 0x00, 0x08, 0x00};
                            //  16    17    18    19    20    21    22    23    24    25    26    27    28    29    30    31    32    33    34
                            //  16    17    18    19    20  mode  temp    23  fan+sw  25    26    27    28 bst&slp  29    15   eco    17 cksum

  remote_state[21] = this->operation_mode_();
  remote_state[22] = this->temperature_();
  remote_state[24] = this->fan_speed_() + this->swing_();
  remote_state[29] = this->powerful_sleep_preset_();
  remote_state[32] = this->eco_preset_();

  // Calculate checksum
  for (int i = 16; i < 34; i++) {
    remote_state[34] += remote_state[i];
  }

  auto transmit = this->transmitter_->transmit();
  auto *data = transmit.get_data();
  data->set_carrier_frequency(DAIKIN_IR_FREQUENCY);

  data->mark(DAIKIN_HEADER_MARK);
  data->space(DAIKIN_HEADER_SPACE);
  for (int i = 0; i < 8; i++) {
    for (uint8_t mask = 1; mask > 0; mask <<= 1) {  // iterate through bit mask
      data->mark(DAIKIN_BIT_MARK);
      bool bit = remote_state[i] & mask;
      data->space(bit ? DAIKIN_ONE_SPACE : DAIKIN_ZERO_SPACE);
    }
  }
  data->mark(DAIKIN_BIT_MARK);
  data->space(DAIKIN_MESSAGE_SPACE);
  data->mark(DAIKIN_HEADER_MARK);
  data->space(DAIKIN_HEADER_SPACE);

  for (int i = 8; i < 16; i++) {
    for (uint8_t mask = 1; mask > 0; mask <<= 1) {  // iterate through bit mask
      data->mark(DAIKIN_BIT_MARK);
      bool bit = remote_state[i] & mask;
      data->space(bit ? DAIKIN_ONE_SPACE : DAIKIN_ZERO_SPACE);
    }
  }
  data->mark(DAIKIN_BIT_MARK);
  data->space(DAIKIN_MESSAGE_SPACE);
  data->mark(DAIKIN_HEADER_MARK);
  data->space(DAIKIN_HEADER_SPACE);

  for (int i = 16; i < 35; i++) {
    for (uint8_t mask = 1; mask > 0; mask <<= 1) {  // iterate through bit mask
      data->mark(DAIKIN_BIT_MARK);
      bool bit = remote_state[i] & mask;
      data->space(bit ? DAIKIN_ONE_SPACE : DAIKIN_ZERO_SPACE);
    }
  }
  data->mark(DAIKIN_BIT_MARK);
  data->space(0);

  transmit.perform();
}

uint8_t Dakin3p5939722eClimate::operation_mode_() const {
  uint8_t operating_mode = DAIKIN_MODE_ON;
  switch (this->mode) {
    case climate::CLIMATE_MODE_COOL:
      operating_mode |= DAIKIN_MODE_COOL;
      break;
    case climate::CLIMATE_MODE_DRY:
      operating_mode |= DAIKIN_MODE_DRY;
      break;
    case climate::CLIMATE_MODE_HEAT:
      operating_mode |= DAIKIN_MODE_HEAT;
      break;
    case climate::CLIMATE_MODE_HEAT_COOL:
      operating_mode |= DAIKIN_MODE_AUTO;
      break;
    case climate::CLIMATE_MODE_FAN_ONLY:
      operating_mode |= DAIKIN_MODE_FAN;
      break;
    case climate::CLIMATE_MODE_OFF:
    default:
      operating_mode = DAIKIN_MODE_OFF;
      break;
  }

  return operating_mode;
}

uint8_t Dakin3p5939722eClimate::fan_speed_() const {
  switch (this->fan_mode.value()) {
    case climate::CLIMATE_FAN_QUIET:
      return DAIKIN_FAN_QUIET;
    case climate::CLIMATE_FAN_LOW:
      return DAIKIN_FAN_1;
    case climate::CLIMATE_FAN_MEDIUM:
      return DAIKIN_FAN_3;
    case climate::CLIMATE_FAN_HIGH:
      return DAIKIN_FAN_5;
    case climate::CLIMATE_FAN_AUTO:
    default:
      return DAIKIN_FAN_AUTO;
  }
}

uint8_t Dakin3p5939722eClimate::swing_() const {
  switch (this->swing_mode) {
    case climate::CLIMATE_SWING_VERTICAL:
      return DAIKIN_SWING_VERTICAL;
    case climate::CLIMATE_SWING_OFF:
    default:
      return DAIKIN_SWING_OFF;
  }
}

uint8_t Dakin3p5939722eClimate::temperature_() const {
  // Force special temperatures depending on the mode
  switch (this->mode) {
    case climate::CLIMATE_MODE_FAN_ONLY:
      return 0x32;
    case climate::CLIMATE_MODE_DRY:
      return 0xC0;
    default:
      uint8_t temperature = (uint8_t) roundf(clamp<float>(this->target_temperature, DAIKIN_TEMP_MIN, DAIKIN_TEMP_MAX));
      return temperature << 1;
  }
}

uint8_t Dakin3p5939722eClimate::powerful_sleep_preset_() const {
  if (this->preset == climate::CLIMATE_PRESET_BOOST) {
    return DAIKIN_POWERFUL_ON;
  } else if (this->preset == climate::CLIMATE_PRESET_SLEEP) {
    return DAIKIN_SLEEP_ON;
  } else if (this->preset == climate::CLIMATE_PRESET_COMFORT) {
    return DAIKIN_SLEEP_ON;
  } else {
    return DAIKIN_POWERFUL_SLEEP_OFF;
  }
}

uint8_t Dakin3p5939722eClimate::eco_preset_() const {
  if (this->preset == climate::CLIMATE_PRESET_COMFORT) {
    return DAIKIN_ECO_ON;
  } else if (this->preset == climate::CLIMATE_PRESET_ECO) {
    return DAIKIN_ECO_ON;
  } else {
    return DAIKIN_ECO_OFF;
  }
}

}  // namespace daikin_3p5939722e
}  // namespace esphome
