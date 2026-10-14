#pragma once

#include <string.h>
#include <stdint.h>
#include "pico/unique_id.h"

#define DEVICE_NAME "es-usb-command"
#define FIRMWARE_VERSION "1.1.0"

typedef struct info_t {
    uint32_t version;
    char name[13];
    uint8_t revision;
};

extern struct info_t device_card;

void device_info(void);
void dev_info(void);