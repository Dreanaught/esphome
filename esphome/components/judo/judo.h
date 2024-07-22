#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

namespace esphome {
namespace judo {

class JudoComponent : public PollingComponent, public uart::UARTDevice {
 public:
  void set_total_consumed(sensor::Sensor *sensor) { this->total_consumed_ = sensor; }
  void dump_config() override;
  float get_setup_priority() const override;
  void loop() override;

 protected:
  sensor::Sensor *total_consumed_{nullptr};
};

}  // namespace judo
}  // namespace esphome
