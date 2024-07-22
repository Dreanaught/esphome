#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

namespace esphome {
namespace judo {

class JudoComponent : public PollingComponent, public uart::UARTDevice {
 public:
  // void set_volume(sensor::Sensor *volume) { this->volume_ = volume; }
  void dump_config() override;
  float get_setup_priority() const override;
  // void update() override;
  void loop() override;

 protected:
  sensor::Sensor *volume_{nullptr};
};

}  // namespace judo
}  // namespace esphome
