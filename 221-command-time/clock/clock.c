#include "clock.h"

#include <stdio.h>
#include "pico\stdlib.h"
#include "hardware\clocks.h"

static void row(const char *name, uint32_t set_khz, uint32_t measured_khz)
{
    printf("%-8s %9u %12u\n", name, (unsigned)set_khz, (unsigned)measured_khz);
}

void clk_info(void) {
    uint32_t freq_hz_set;
    uint32_t freq_khz_meas;
    
    printf("%-9s  %-7s %-12s\n", "signal", "set_khz", "measured_khz");

    freq_hz_set = clock_get_hz(clk_ref);
    freq_khz_meas = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_REF);
    row("clk_ref", freq_hz_set / 1000, freq_khz_meas);
    freq_hz_set = clock_get_hz(clk_sys);
    freq_khz_meas = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    row("clk_sys", freq_hz_set / 1000, freq_khz_meas);
    freq_hz_set = clock_get_hz(clk_peri);
    freq_khz_meas = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    row("clk_peri", freq_hz_set / 1000, freq_khz_meas);
    freq_hz_set = clock_get_hz(clk_usb);
    freq_khz_meas = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    row("clk_usb", freq_hz_set / 1000, freq_khz_meas);
    freq_hz_set = clock_get_hz(clk_adc);
    freq_khz_meas = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
    row("clk_adc", freq_hz_set / 1000, freq_khz_meas);
    freq_khz_meas = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    printf("%-9s  %7s %12u\n", "cld_ref", "-", freq_khz_meas);
}