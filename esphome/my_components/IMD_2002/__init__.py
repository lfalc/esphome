CODEOWNERS = ["@lfalc"]

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, sensor, uart
from esphome.const import (
    CONF_ID, 
    CONF_TRIGGER_ID
)

AUTO_LOAD = [ "binary_sensor", "sensor" ]

DEPENDENCIES = [ "uart" ]

imd_2002_ns = cg.esphome_ns.namespace('imd_2002')
IMD_2002 = imd_2002_ns.class_('IMD2002', binary_sensor.BinarySensor, cg.Component)

CONFIG_SCHEMA = binary_sensor.BINARY_SENSOR_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(IMD_2002),
    }
).extend(cv.COMPONENT_SCHEMA)



async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await binary_sensor.register_binary_sensor(var, config)

    # pin = await cg.gpio_pin_expression(config[CONF_INPUT_OBST])
    # cg.add(var.set_input_obst_pin(pin))