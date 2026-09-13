#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

#include "led.h"

const uint BUTTON_PIN = 15;
const uint DEBOUNCE_MS = 200;

bool get_button_debounce(uint pin) {
    bool pin_value = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return gpio_get(pin) && pin_value;
}

void handle_command(int command) {
    if (command == 'e') {
        led_set(true);
        printf("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (command == 'd') {
        led_set(false);
        printf("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (command != PICO_ERROR_TIMEOUT) {
        printf("unknown command: %c\n", command);
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
        int command = getchar_timeout_us(0);
        handle_command(command);

        button_pin_curr_value = get_button_debounce(BUTTON_PIN);
        if(button_pin_curr_value != button_pin_last_value) {
            led_toggle();
            printf("led %s\n", led_is_on() ? "on" : "off");
        }
        button_pin_last_value = button_pin_curr_value;
    }
}