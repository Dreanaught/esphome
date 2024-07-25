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

  LOG_SENSOR("  ", "Volume", this->total_consumed_);
  LOG_SENSOR("  ", "Current Flow", this->current_flow_);
  LOG_SENSOR("  ", "Remaining Hardness", this->remaining_hardness_);

  this->check_uart_settings(19200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
}

float JudoComponent::get_setup_priority() const { return setup_priority::DATA; }

void JudoComponent::update() {
  // noop
}

void JudoComponent::loop() {
  uint8_t data_;
  // read this line
  // 01 33 28 00 04 00 AB 90 03 00 2A ED
  if (this->available()) {
    // verify 01 33
    this->read_byte(&data_);
    if (data_ == 0x01) {
      this->read_byte(&data_);
      if (data_ == 0x33) {
        // verify 28 00
        this->read_byte(&data_);
        // read 28 00 04 00 AB 90 03 00 2A ED
        if (data_ == 0x28) {
          this->read_byte(&data_);
          if (data_ == 0x00) {
            // verify 04 00
            this->read_byte(&data_);
            if (data_ == 0x04) {
              this->read_byte(&data_);
              if (data_ == 0x00) {
                // read 4 bytes of data into buffer
                uint8_t buffer[0x04];
                this->read_array(buffer, sizeof(buffer));
                // transform lsb to msb
                uint32_t value = (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];
                // publish data
                if (this->total_consumed_ != nullptr) {
                  this->total_consumed_->publish_state(value / 1000.0f);
                }
              }
            }
          }
        } else if (data_ == 0x29) {
          this->read_byte(&data_);
          if (data_ == 0x00) {
            this->read_byte(&data_);
            if (data_ == 0x04) {
              this->read_byte(&data_);
              if (data_ == 0x00) {
                // read 4 bytes of data into buffer
                uint8_t buffer[0x04];
                this->read_array(buffer, sizeof(buffer));
                // transform lsb to msb
                uint32_t value = (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];
                // publish data
                if (this->total_softened_ != nullptr) {
                  this->total_softened_->publish_state(value / 1000.0f);
                }
              }
            }
          }
        }
        // read 43 00 20 00 00 03 0F 30 00 00 10 00 05 04 0D 00 E4 01 CC 00 00 00 00 00 00 00 82 0F 44 04 10 02 38 00 34
        // 00 A8 8C
        else if (data_ = 0x43) {
          this->read_byte(&data_);
          if (data_ == 0x00) {
            this->read_byte(&data_);
            if (data_ == 0x20) {
              this->read_byte(&data_);
              if (data_ == 0x00) {
                // read 00030F380000000005040D00D201D5006101FA000000B90F10040F0030002E00
                // read 00030F380000000005030D00D501D1005702C3010000B90F10040F0038002E00
                // read 00030F380000000005040D00D601D3007B02CE010000B90F10040F0030002E00 // 635
                // read 00030F380000000005040D00D601D3008D02DE010000B90F10040F0030002E00 // 0
                // read 00030F380000000005030D00D501D1005702C3010000B90F10040F0038002E00
                // read 00030F380000100005040D00D601D300000000000000B90F10040F0238002E00
                // read 00030F380000000005040D00D601D300A60122010000B90F10040F0030002E00
                uint8_t buffer[0x20];
                this->read_array(buffer, sizeof(buffer));
                ESP_LOGVV(TAG, "Received frame: %s", format_hex_pretty(buffer, sizeof(buffer)).c_str());
                if (buffer[0] == 0x00 && buffer[1] == 0x03) {
                  // parse current flow
                  if (this->current_flow_ != nullptr) {
                    uint8_t b2[2];
                    b2[0] = buffer[17];
                    b2[1] = buffer[16];
                    ESP_LOGVV(TAG, "current flow bytes: %s", format_hex_pretty(b2, sizeof(b2)).c_str());
                    uint16_t value = (buffer[17] << 8) | buffer[16];
                    this->current_flow_->publish_state(value * 1.0f);
                  }
                  // parse remaining hardness
                  if (this->remaining_hardness_ != nullptr) {
                    uint8_t b2[1];
                    b2[0] = buffer[8];
                    ESP_LOGVV(TAG, "remaining hardness bytes: %s", format_hex_pretty(b2, sizeof(b2)).c_str());
                    uint8_t value = buffer[8];
                    this->remaining_hardness_->publish_state(value * 1.0f);
                  }
                  // parse raw hardness
                  if (this->raw_hardness_ != nullptr) {
                    uint8_t b2[1];
                    b2[0] = buffer[26];
                    ESP_LOGVV(TAG, "raw hardness bytes: %s", format_hex_pretty(b2, sizeof(b2)).c_str());
                    uint8_t value = buffer[9];
                    this->raw_hardness_->publish_state(value / 2 + 2.0f);
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
