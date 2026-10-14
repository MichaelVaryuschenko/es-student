#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <string.h>

#include "led.h"
#include "log.h"
#include "device.h"
#include "memory.h"
#include "command.h"
#include "clock.h"

#define LINE_SIZE 32

const uint BLINK_HALF_PERIOD_MS = 500;
uint64_t last_toggle_us = 0;

// 4.7 s
const uint CALC_PI_TERMS = 1000000;
volatile double pi_result;

char line[LINE_SIZE];
uint line_length = 0;

double calc_pi(uint terms)
{
    double sum = 0.0;
    double sign = 1.0;

    for (int k = 0; k < terms; k++) {
        // Прибавить очередной член ряда: ±1 / (2k + 1)
        sum += sign / (2.0 * k + 1.0);
        sign = -sign;
    }

    // Сумма ряда равна π/4
    return 4.0 * sum;
}

void cmd_info(void) {
    device_info();
}

void cmd_version(void) {
    log_version();
}

void cmd_ping(void) {
    log_ping();
}

void cmd_mem_info(void) {
    mem_info();
}

void cmd_fw_info(void) {
    fw_info();
}

void cmd_dev_info(void) {
    dev_info();
}

void cmd_boot_info(void) {
    boot_info();
}

void cmd_clk_info(void) {
    clk_info();
}

void cmd_uptime(void) {
    uptime();
}

void cmd_calc_pi(void) {
    uint64_t start_us = time_us_64();
    pi_result = calc_pi(CALC_PI_TERMS);
    uint64_t spent_us = time_us_64() - start_us;

    printf("pi: %.8f\n", pi_result);
    printf("time: %llu ms\n", spent_us / 1000);
}

const struct command_t commands[] = {
    { "info", cmd_info },
    { "version", cmd_version },
    { "ping", cmd_ping },
    { "mem_info", cmd_mem_info },
    { "fw_info", cmd_fw_info },
    { "dev_info", cmd_dev_info },
    { "boot_info", cmd_boot_info },
    { "clk_info", cmd_clk_info },
    { "uptime", cmd_uptime },
    { "calc_pi", cmd_calc_pi },
};

const uint command_count = sizeof(commands) / sizeof(commands[0]);

void handle_command(char* command) {
    for(uint i = 0; i < command_count; i++) {
        if(strcmp(command, commands[i].name) == 0) {
            if(commands[i].handler != NULL) {
                commands[i].handler();
            }
            return;
        }
    }
    LOG_ERR("unknown command: %s\n", command);
}

void read_line(void) {
    int symbol = getchar_timeout_us(0);
    
    if(symbol == PICO_ERROR_TIMEOUT) {
        return;
    }

    putchar(symbol);

    if(symbol != '\n' && symbol != '\r') {
        if(line_length < LINE_SIZE + 1) {
            line[line_length] = (char)symbol;
            line_length++;
        }
        else {
            LOG_ERR("too many symbols in command; symbol not added\n");
        }
    }
    else {
        line[line_length] = '\0';
        if(line_length > 0) {
            LOG_DBG("got %s\n", line);
            handle_command(line);
        }
        else {
            LOG_DBG("got empty line\n");
        }
        line_length = 0;
    }
}

void blink(void) {
    uint64_t now_us = time_us_64();

    if(now_us - last_toggle_us >= BLINK_HALF_PERIOD_MS * 1000) {
        led_toggle();
        last_toggle_us = now_us;
    }
}

int main() {
    bool button_pin_last_value = true;
    bool button_pin_curr_value = true;

    led_init();

    stdio_init_all();

    while (1) {
        blink();
        read_line();
    }
}