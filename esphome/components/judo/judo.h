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
  void set_raw_hardness(sensor::Sensor *sensor) { this->raw_hardness_ = sensor; }
  void set_remaining_hardness(sensor::Sensor *sensor) { this->remaining_hardness_ = sensor; }
  void set_hours_till_next_service(sensor::Sensor *sensor) { this->hours_till_next_service_ = sensor; }
  void set_requested_service_count(sensor::Sensor *sensor) { this->requested_service_count_ = sensor; }
  void set_registered_service_count(sensor::Sensor *sensor) { this->registered_service_count_ = sensor; }
  void dump_config() override;
  float get_setup_priority() const override;
  void loop() override;
  void update() override;

 protected:
  // Sensors
  sensor::Sensor *total_consumed_{nullptr};
  sensor::Sensor *total_softened_{nullptr};
  sensor::Sensor *current_flow_{nullptr};
  sensor::Sensor *raw_hardness_{nullptr};
  sensor::Sensor *remaining_hardness_{nullptr};
  sensor::Sensor *hours_till_next_service_{nullptr};
  sensor::Sensor *requested_service_count_{nullptr};
  sensor::Sensor *registered_service_count_{nullptr};
};

}  // namespace judo
}  // namespace esphome
