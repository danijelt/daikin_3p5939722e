import esphome.codegen as cg
from esphome.components import climate_ir

AUTO_LOAD = ["climate_ir"]

daikin_3p5939722e_ns = cg.esphome_ns.namespace("daikin_3p5939722e")
Dakin3p5939722eClimate = daikin_3p5939722e_ns.class_("Dakin3p5939722eClimate", climate_ir.ClimateIR)

CONFIG_SCHEMA = climate_ir.climate_ir_with_receiver_schema(Dakin3p5939722eClimate)


async def to_code(config):
    await climate_ir.new_climate_ir(config)
