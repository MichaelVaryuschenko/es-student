#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint LED_PIN = 25;
const uint BUTTON_PIN = 15;
const uint DEBOUNCE_MS = 20;

bool get_button_debounce(uint pin) {
    bool pin_value = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return gpio_get(pin) && pin_value;
}

int main() {
    bool led_mode = false;
    bool button_pin_last_value = true;
    bool button_pin_curr_value = true;

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_put(LED_PIN, led_mode);

    while (1) {
        button_pin_curr_value = get_button_debounce(BUTTON_PIN);
        if(button_pin_curr_value != button_pin_last_value) {
            led_mode = !led_mode;
            gpio_put(LED_PIN, led_mode);
        }
        button_pin_last_value = button_pin_curr_value;
    }
}