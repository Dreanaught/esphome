import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    CONF_ID,
    UNIT_CUBIC_METER,
    STATE_CLASS_TOTAL_INCREASING,
    DEVICE_CLASS_VOLUME,
)

CODEOWNERS = ["@Dreanaught"]
DEPENDENCIES = ["uart"]

CONF_TOTAL_CONSUMED = "total_consumed"

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
        cg.add(var.set_volume(config[CONF_TOTAL_CONSUMED]))
