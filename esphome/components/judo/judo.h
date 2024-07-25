#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

namespace esphome {
namespace judo {

class JudoComponent : public PollingComponent, public uart::UARTDevice {
 public:
  void set_total_consumed(sensor::Sensor *sensor) { this->total_consumed_ = sensor; }
  void set_total_softened(sensor::Sensor *sensor) { this->total_softened_ = sensor; }
  void set_current_flow(sensor::Sensor *sensor) { this->current_flow_ = sensor; }
  void set_remaining_hardness(sensor::Sensor *sensor) { this->remaining_hardness_ = sensor; }
  void dump_config() override;
  float get_setup_priority() const override;
  void loop() override;
  void update() override;

 protected:
  // Sensors
  sensor::Sensor *total_consumed_{nullptr};
  sensor::Sensor *total_softened_{nullptr};
  sensor::Sensor *current_flow_{nullptr};
  sensor::Sensor *remaining_hardness_{nullptr};
};

}  // namespace judo
}  // namespace esphome
