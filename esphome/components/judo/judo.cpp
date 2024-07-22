#include "judo.h"

#include "esphome/core/log.h"

namespace esphome {
namespace judo {

static const char *const TAG = "judo";

void JudoComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "judo:");
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with Judo ISoft failed!");
  }
  LOG_UPDATE_INTERVAL(this);

  LOG_SENSOR("  ", "Volume", this->volume_);

  this->check_uart_settings(19200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
}

float JudoComponent::get_setup_priority() const { return setup_priority::DATA; }

/*void JudoComponent::update() {
  if (this->volume_ != nullptr) {
    // TODO: implement
  }
}*/

void JudoComponent::loop() {
  uint8_t data_;
  uint8_t buffer[4];
  // read this line
  // 01 33 28 00 04 00 AB 90 03 00 2A ED //8
  if (this->available()) {
    // verify 01 33
    this->read_byte(&data_);
    if (data_ == 0x01) {
      this->read_byte(&data_);
      if (data_ == 0x33) {
        // verify 28 00
        this->read_byte(&data_);
        if (data_ == 0x28) {
          this->read_byte(&data_);
          if (data_ == 0x00) {
            // verify 04 00
            this->read_byte(&data_);
            if (data_ == 0x04) {
              this->read_byte(&data_);
              if (data_ == 0x00) {
                // read 4 bytes of data into buffer
                this->read_array(buffer, sizeof(buffer));
                // transform lsb to msb
                uint32_t value = (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];
                // publish data
                if (this->volume_ != nullptr) {
                  this->volume_->publish_state(value / 1000.0f);
                }
              }
            }
          }
        }
      }
    }
  }
}

}  // namespace judo
}  // namespace esphome
