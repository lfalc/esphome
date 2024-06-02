import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    STATE_CLASS_MEASUREMENT,
    UNIT_METER,
    ICON_ARROW_EXPAND_VERTICAL,
    DEVICE_CLASS_DISTANCE,
)

CODEOWNERS = ["@lfalc"]
DEPENDENCIES = ["uart"]

IMD_2002_ns = cg.esphome_ns.namespace("IMD_2002")
IMD_2002Component = IMD_2002_ns.class_(
    "IMD_2002", sensor.Sensor, cg.Component, uart.UARTDevice
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
    IMD_2002Component,
    unit_of_measurement=UNIT_METER,
    icon=ICON_ARROW_EXPAND_VERTICAL,
    accuracy_decimals=3,
    state_class=STATE_CLASS_MEASUREMENT,
    device_class=DEVICE_CLASS_DISTANCE,
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("60s"))
)

FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema(
    "IMD_2002",
    baud_rate=256000,
    require_tx=False,
    require_rx=True,
    data_bits=8,
    parity=None,
    stop_bits=1,
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
