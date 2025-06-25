#ifndef BH1750_BH1750_H
#define BH1750_BH1750_H

#include "bh1750_commands.h"
#include "bh1750_config.h"

typedef struct {
    bh1750_config_t config;
    bh1750_interface_t interface;
} bh1750_t;

bh1750_err_t bh1750_initialize(bh1750_t* bh1750,
                               bh1750_config_t const* config,
                               bh1750_interface_t const* interface);
bh1750_err_t bh1750_deinitialize(bh1750_t* bh1750);

bh1750_err_t bh1750_get_light_data_scaled(bh1750_t const* bh1750, float32_t* scaled);
bh1750_err_t bh1750_get_light_data_raw(bh1750_t const* bh1750, uint16_t* raw);

bh1750_err_t bh1750_get_device_id(bh1750_t const* bh1750, uint8_t* id);

bh1750_err_t bh1750_send_power_down_cmd(bh1750_t const* bh1750);
bh1750_err_t bh1750_send_power_on_cmd(bh1750_t const* bh1750);
bh1750_err_t bh1750_send_reset_cmd(bh1750_t const* bh1750);

bh1750_err_t bh1750_set_mtreg(bh1750_t const* bh1750, uint8_t reg);
bh1750_err_t bh1750_set_mode(bh1750_t const* bh1750, bh1750_mode_t mode);
bh1750_err_t bh1750_set_power_state(bh1750_t const* bh1750, bh1750_cmd_t cmd);

#endif // BH1750_BH1750_H