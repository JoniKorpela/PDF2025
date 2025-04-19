#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"

void init_buzzer();
void toot_buzzer();

#endif