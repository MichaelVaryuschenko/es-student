#include "memory.h"
#include <stdio.h>
#include "hardware/regs/addressmap.h"
#include "pico/stdlib.h"

extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;

#define ROM_SIZE_KB 16 

static void row(const char* name, uintptr_t start, uintptr_t end) {
    printf("%-10s 0x%08x 0x%08x %8u\n", name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}

void mem_info(void) {
    printf("%-10s %-10s %-10s %-8s\n", "Section", "Start", "End", "Size");
    row("flash", XIP_BASE, XIP_BASE + PICO_FLASH_SIZE_BYTES);
    row("sram", SRAM_BASE, SRAM_END);
    row("rom", ROM_BASE, ROM_BASE + ROM_SIZE_KB * 1024);
    
    row("image", (uintptr_t)&__flash_binary_start, (uintptr_t)&__flash_binary_end);
    row("free", (uintptr_t)&__flash_binary_end, XIP_BASE + PICO_FLASH_SIZE_BYTES);
    row("boot2", (uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__);
    row("text", (uintptr_t)&__boot2_end__, (uintptr_t)&__etext);
    row("data flash", (uintptr_t)&__etext, (uintptr_t)&__etext + (unsigned)(&__data_end__ - &__data_start__));

    row("data ram", (uintptr_t)&__data_start__, (uintptr_t)&__data_end__);
    row("bss", (uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__);
    row("heap", (uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit);
    row("stack", (uintptr_t)&__StackBottom, (uintptr_t)&__StackTop);

    printf("Total\n");
    printf("  %-11s %8u = %s %u + %s %u + %s %u\n", 
        "flash image", (unsigned)((uintptr_t)&__flash_binary_end - (uintptr_t)&__flash_binary_start), 
        "boot2", (unsigned)((uintptr_t)&__boot2_end__ - (uintptr_t)&__boot2_start__), 
        "text", (unsigned)((uintptr_t)&__etext - (uintptr_t)&__boot2_end__), 
        "data", (unsigned)((uintptr_t)&__data_end__ - (uintptr_t)&__data_start__));
    printf("  %-11s %8u of %u\n", "flash free", XIP_BASE + PICO_FLASH_SIZE_BYTES - (unsigned)(uintptr_t)&__flash_binary_end, PICO_FLASH_SIZE_BYTES);
    printf("  %-11s %8u = %s %u + %s %u\n", 
        "ram used", (unsigned)(&__bss_end__ - &__data_start__),
        "data", (unsigned)(&__data_end__ - &__data_start__),
        "bss", (unsigned)(&__bss_end__ - &__bss_start__));
    printf("  %-11s %8u for %s and %u for %s\n", 
        "ram free", 
        (unsigned)(&__HeapLimit - &__bss_end__), "heap", 
        (unsigned)(&__StackTop - &__StackBottom), "stack");
}
