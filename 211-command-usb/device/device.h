#pragma once

#include <string.h>
#include "pico/unique_id.h"

#define DEVICE_NAME "es-usb-command"
#define FIRMWARE_VERSION "1.2.0"

/*typedef struct info_t {
    uint32_t revision;
    uint8_t version;
    char name[13];
};

extern info_t device_card;*/

void device_info(void);