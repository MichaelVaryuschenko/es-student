#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <string.h>

#include "led.h"
#include "log.h"
#include "device.h"

#define LINE_SIZE 32

const uint BUTTON_PIN = 15;
const uint DEBOUNCE_MS = 200;

char line[LINE_SIZE];
uint line_length = 0;

bool get_button_debounce(uint pin) {
    bool pin_value = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return gpio_get(pin) && pin_value;
}

void handle_command(char* command) {
    if (!strcmp(command, "enable")) { // If lines are equal
        led_set(true);
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (!strcmp(command, "disable")) {
        led_set(false);
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (!strcmp(command, "info")) {
        log_version();
    }
    else if (!strcmp(command, "version")) {
        device_info();
    }
    else {
        LOG_ERR("unknown command: %c\n", command);
    }
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

int main() {
    bool button_pin_last_value = true;
    bool button_pin_curr_value = true;

    led_init();
    
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    stdio_init_all();

    while (1) {
        read_line();

        button_pin_curr_value = get_button_debounce(BUTTON_PIN);
        if(button_pin_curr_value != button_pin_last_value) {
            led_toggle();
            LOG_INF("led %s\n", led_is_on() ? "on" : "off");
        }
        button_pin_last_value = button_pin_curr_value;
    }
}