#ifndef BH1750_BH1750_CONFIG_H
#define BH1750_BH1750_CONFIG_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BH1750_MTREG_MIN 0x1F
#define BH1750_MTREG_DEFAULT 0x45
#define BH1750_MTREG_MAX 0xFE
#define BH1750_MEASUREMENT_ACCURACY 1.2F

typedef float float32_t;

typedef enum {
    BH1750_ERR_OK = 0,
    BH1750_ERR_FAIL = 1 << 0,
    BH1750_ERR_NULL = 1 << 1,
} bh1750_err_t;

typedef enum {
    BH1750_SLAVE_ADDRESS_AD0_LOW = 0x55,
    BH1750_SLAVE_ADDRESS_AD0_HIG = 0x23,
} bh1750_slave_address_t;

typedef enum {
    BH1750_MODE_CONTINUOUS_HIGH_RES = 0x10,
    BH1750_MODE_CONTINUOUS_HIGH_RES_2 = 0x11,
    BH1750_MODE_CONTINUOUS_LOW_RES = 0x13,
    BH1750_MODE_ONETIME_HIGH_RES = 0x20,
    BH1750_MODE_ONETIME_HIGH_RES_2 = 0x21,
    BH1750_MODE_ONETIME_LOW_RES = 0x23,
} bh1750_mode_t;

typedef struct {
    bh1750_mode_t mode;
    uint8_t mtreg;
} bh1750_config_t;

typedef struct {
    void* bus_user;
    bh1750_err_t (*bus_initialize)(void*);
    bh1750_err_t (*bus_deinitialize)(void*);
    bh1750_err_t (*bus_transmit)(void*, uint8_t const*, size_t);
    bh1750_err_t (*bus_receive)(void*, uint8_t*, size_t);
} bh1750_interface_t;

inline float32_t bh1750_mode_to_resolution(bh1750_mode_t mode)
{
    if (mode == BH1750_MODE_CONTINUOUS_HIGH_RES ||
        mode == BH1750_MODE_ONETIME_HIGH_RES) {
        return 1.0F;
    } else if (mode == BH1750_MODE_CONTINUOUS_HIGH_RES_2 ||
               mode == BH1750_MODE_ONETIME_HIGH_RES_2) {
        return 0.5F;
    } else if (mode == BH1750_MODE_CONTINUOUS_LOW_RES ||
               mode == BH1750_MODE_ONETIME_LOW_RES) {
        return 4.0F;
    }

    return 0.0F;
}

inline float32_t bh1750_raw_to_scaled(uint16_t raw,
                                      bh1750_mode_t mode,
                                      uint8_t mtreg)
{
    return (float32_t)raw * (1.0F / BH1750_MEASUREMENT_ACCURACY) *
           (BH1750_MTREG_DEFAULT / mtreg) * bh1750_mode_to_resolution(mode);
}

#ifdef __cplusplus
}
#endif

#endif // BH1750_BH1750_CONFIG_H