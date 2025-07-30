#ifndef BH1750_BH1750_COMMANDS_H
#define BH1750_BH1750_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BH1750_CMD_POWER_DOWN = 0x00,
    BH1750_CMD_POWER_ON = 0x01,
    BH1750_CMD_RESET = 0x07,
} bh1750_cmd_t;

#ifdef __cplusplus
}
#endif

#endif // BH1750_BH1750_COMMANDS_H