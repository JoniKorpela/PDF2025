

#include "pico/stdlib.h"
#include "hardware/pwm.h"

const uint HORN_PIN = 16;

void init_buzzer() {
gpio_init(HORN_PIN);
gpio_set_dir(HORN_PIN, GPIO_OUT);
}

void toot_buzzer(int duration) {
    gpio_put(HORN_PIN, 1);
    sleep_ms(duration * 500);
    gpio_put(HORN_PIN, 0);
}