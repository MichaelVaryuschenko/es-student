#pragma once

#include <stdint.h>

#define VECTOR_TABLE 0x10000100

extern const char DEVICE_PROJECT[];
extern const char DEVICE_BOARD[];

static void row(const char* name, uintptr_t start, uintptr_t end);
void mem_info(void);
void fw_info(void);
void boot_info(void);