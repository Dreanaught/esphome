import esphome.codegen as cg
from esphome.components import sensor, uart
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_VOLUME,
    DEVICE_CLASS_VOLUME_FLOW_RATE,
    DEVICE_CLASS_WATER,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_CUBIC_METER,
)

CODEOWNERS = ["@Dreanaught"]
DEPENDENCIES = ["uart"]

CONF_TOTAL_CONSUMED = "total_consumed"
CONF_CURRENT_FLOW = "current_flow"
CONF_REMAINING_HARDNESS = "remaining_hardness"
UNIT_LITRE_PER_HOUR = "l/h"

judo_ns = cg.esphome_ns.namespace("judo")
JudoComponent = judo_ns.class_("JudoComponent", cg.PollingComponent, uart.UARTDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(JudoComponent),
            cv.Optional(CONF_TOTAL_CONSUMED): sensor.sensor_schema(
                state_class=STATE_CLASS_TOTAL_INCREASING,
                device_class=DEVICE_CLASS_VOLUME,
                unit_of_measurement=UNIT_CUBIC_METER,
                accuracy_decimals=3,
            ),
            cv.Optional(CONF_CURRENT_FLOW): sensor.sensor_schema(
                state_class=STATE_CLASS_MEASUREMENT,
                device_class=DEVICE_CLASS_VOLUME_FLOW_RATE,
                unit_of_measurement=UNIT_LITRE_PER_HOUR,
                accuracy_decimals=0,
            ),
            cv.Optional(CONF_REMAINING_HARDNESS): sensor.sensor_schema(
                state_class=STATE_CLASS_MEASUREMENT,
                device_class=DEVICE_CLASS_WATER,
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)

FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema(
    "judo", baud_rate=19200, require_rx=True
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    if CONF_TOTAL_CONSUMED in config:
        sens = await sensor.new_sensor(config[CONF_TOTAL_CONSUMED])
        cg.add(var.set_total_consumed(sens))
    if CONF_CURRENT_FLOW in config:
        sens = await sensor.new_sensor(config[CONF_CURRENT_FLOW])
        cg.add(var.set_current_flow(sens))
    if CONF_REMAINING_HARDNESS in config:
        sens = await sensor.new_sensor(config[CONF_REMAINING_HARDNESS])
        cg.add(var.set_remaining_hardness(sens))
