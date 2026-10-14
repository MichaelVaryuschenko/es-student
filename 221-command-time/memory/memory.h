#pragma once

#include <stdint.h>

extern const char DEVICE_BOARD[];

static void row(const char* name, uintptr_t start, uintptr_t end);
void mem_info(void);
void fw_info(void);
void boot_info(void);