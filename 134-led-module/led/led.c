#include "led.h"
#include "hardware/gpio.h"

const uint LED_PIN = 25;

static bool led_state = false;

void led_init(void) {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    led_set(led_state);
}

void led_set(bool led_mode) {
    led_state = led_mode;
    gpio_put(LED_PIN, led_mode);
}

void led_toggle(void) {
    led_set(!led_state);
}

bool led_is_on(void) {
    return led_state;
}