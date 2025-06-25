#include "bh1750.h"
#include "bh1750_commands.h"
#include "bh1750_config.h"
#include <assert.h>
#include <string.h>

static bh1750_err_t bh1750_bus_initialize(bh1750_t const* bh1750)
{
    return bh1750->interface.bus_initialize
               ? bh1750->interface.bus_initialize(bh1750->interface.bus_user)
               : BH1750_ERR_NULL;
}

static bh1750_err_t bh1750_bus_deinitialize(bh1750_t const* bh1750)
{
    return bh1750->interface.bus_deinitialize
               ? bh1750->interface.bus_deinitialize(bh1750->interface.bus_user)
               : BH1750_ERR_NULL;
}

static bh1750_err_t bh1750_bus_transmit(bh1750_t const* bh1750,
                                        uint8_t const* data,
                                        size_t data_size)
{
    return bh1750->interface.bus_transmit
               ? bh1750->interface.bus_transmit(bh1750->interface.bus_user, data, data_size)
               : BH1750_ERR_NULL;
}

static bh1750_err_t bh1750_bus_receive(bh1750_t const* bh1750, uint8_t* data, size_t data_size)
{
    return bh1750->interface.bus_receive
               ? bh1750->interface.bus_receive(bh1750->interface.bus_user, data, data_size)
               : BH1750_ERR_NULL;
}

bh1750_err_t bh1750_initialize(bh1750_t* bh1750,
                               bh1750_config_t const* config,
                               bh1750_interface_t const* interface)
{
    assert(bh1750 && config && interface);

    memset(bh1750, 0, sizeof(*bh1750));
    memcpy(&bh1750->config, config, sizeof(*config));
    memcpy(&bh1750->interface, interface, sizeof(*interface));

    return bh1750_bus_initialize(bh1750);
}

bh1750_err_t bh1750_deinitialize(bh1750_t* bh1750)
{
    assert(bh1750);

    bh1750_err_t err = bh1750_bus_deinitialize(bh1750);

    memset(bh1750, 0, sizeof(*bh1750));

    return err;
}

bh1750_err_t bh1750_get_light_data_scaled(bh1750_t const* bh1750, float32_t* scaled)
{
    assert(bh1750 && scaled);

    uint16_t raw = {};

    bh1750_err_t err = bh1750_get_light_data_raw(bh1750, &raw);

    *scaled = bh1750_raw_to_scaled(raw, bh1750->config.mode, bh1750->config.mtreg);

    return err;
}

bh1750_err_t bh1750_get_light_data_raw(bh1750_t const* bh1750, uint16_t* raw)
{
    assert(bh1750 && raw);

    bh1750_err_t err = bh1750_set_mode(bh1750, bh1750->config.mode);

    uint8_t data[2] = {};

    err |= bh1750_bus_receive(bh1750, data, sizeof(data));

    *raw = (uint16_t)((data[0] & 0xFFU) << 8U) | (data[1] & 0xFFU);

    return err;
}

bh1750_err_t bh1750_get_device_id(bh1750_t const* bh1750, uint8_t* id)
{
    assert(bh1750 && id);

    return bh1750_bus_receive(bh1750, id, sizeof(*id));
}

bh1750_err_t bh1750_send_power_down_cmd(bh1750_t const* bh1750)
{
    assert(bh1750);

    uint8_t data[1] = {};

    data[0] = (BH1750_CMD_POWER_DOWN & 0xFFU);

    return bh1750_bus_transmit(bh1750, data, sizeof(data));
}

bh1750_err_t bh1750_send_power_on_cmd(bh1750_t const* bh1750)
{
    assert(bh1750);

    uint8_t data[1] = {};

    data[0] = (BH1750_CMD_POWER_ON & 0xFFU);

    return bh1750_bus_transmit(bh1750, data, sizeof(data));
}

bh1750_err_t bh1750_send_reset_cmd(bh1750_t const* bh1750)
{
    assert(bh1750);

    uint8_t data[1] = {};

    data[0] = (BH1750_CMD_RESET & 0xFFU);

    return bh1750_bus_transmit(bh1750, data, sizeof(data));
}

bh1750_err_t bh1750_set_mtreg(bh1750_t const* bh1750, uint8_t reg)
{
    assert(bh1750);

    if (reg > BH1750_MTREG_MAX) {
        reg = BH1750_MTREG_MAX;
    }
    if (reg < BH1750_MTREG_MIN) {
        reg = BH1750_MTREG_MIN;
    }

    uint8_t data = {};

    data = 0x40U & (reg >> 5U);

    bh1750_err_t err = bh1750_bus_transmit(bh1750, &data, sizeof(data));

    data = 0x60U & (reg & 0x01FU);

    err |= bh1750_bus_transmit(bh1750, &data, sizeof(data));

    return err;
}

bh1750_err_t bh1750_set_mode(bh1750_t const* bh1750, bh1750_mode_t mode)
{
    assert(bh1750);

    uint8_t data = {};

    data = mode & 0xFFU;

    return bh1750_bus_transmit(bh1750, &data, sizeof(data));
}

bh1750_err_t bh1750_set_power_state(bh1750_t const* bh1750, bh1750_cmd_t cmd)
{
    assert(bh1750);

    uint8_t data = {};

    data = cmd & 0xFFU;

    return bh1750_bus_transmit(bh1750, &data, sizeof(data));
}
