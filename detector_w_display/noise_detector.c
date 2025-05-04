#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"

#include "buzzer.h"
#include "display.h"

/* can be tweaked */
#define SAMPLE_COUNT 10
#define SAMPLE_WINDOW_MICROSECONDS (100 * 1000) /* 100 ms */

/* should be tweaked */
#define VIOLATION_THRESHOLD 0.75f

#define ADC_NUM 0
#define ADC_PIN (26 + ADC_NUM)
#define ADC_RANGE (1 << 12)

/* remap [0,4095] to [0,1] */
#define ADC_NORMALIZE(adc_reading) ((adc_reading) * (1.0f / (ADC_RANGE - 1)))

/* tells the kids to shut the fuck up */
static void toot_horn(int violation_num)
{
  toot_buzzer(violation_num);
}

static void update_display(const char *fmt, ...)
{
  va_list ap;
  char buffer[32];

  va_start(ap, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, ap);
  va_end(ap);

  /* FIXME: actually update the display */
  puts(buffer);
  lcd_show(buffer);
}

/* coarse peak-to-peak adc voltage estimation over a 100 ms period */
static int peak_to_peak_100ms(void)
{
  uint64_t start = time_us_64();

  uint16_t min_sample = 4096;
  uint16_t max_sample = 0;

  while (1)
  {
    uint64_t current = time_us_64();
    if (current - start >= SAMPLE_WINDOW_MICROSECONDS)
    {
      /* done */
      break;
    }

    uint16_t sample = adc_read();
    if (sample < min_sample) min_sample = sample;
    if (sample > max_sample) max_sample = sample;
  }

  return max_sample - min_sample;
}

static float remap_float(float value, float low1, float high1, float low2, float high2)
{
  return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

int main(void)
{
  init_buzzer();
  stdio_init_all();

  display_lcd_message();

  adc_init();
  adc_gpio_init(ADC_PIN);
  adc_select_input(ADC_NUM);

  int violation_count = 0;
  int old_display_value = -1; /* refresh immediately */
  float lowest_loudness = 9999999; /* baseline for 0% noise */

  while (1)
  {
    int accum = 0;

    /* sample 10 times */
    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
      int sample = peak_to_peak_100ms();
      accum += sample;
    }

    /* take the average */
    int average = accum / SAMPLE_COUNT;

    /* normalize to [0-1] */
    float loudness = ADC_NORMALIZE(average);
	lowest_loudness = (loudness < lowest_loudness) ? loudness : lowest_loudness;

    if (loudness > VIOLATION_THRESHOLD)
    {
      violation_count++;

      update_display("Too loud!");

      toot_horn(violation_count);

      /* debug prints */
      printf("too loud (violation #%d): %.2f\n", violation_count, loudness);

	  /* invalidate so it gets recomputed */
	  old_display_value = -1;
    }
    else
    {
	  /* update the display...
	  temp fudge to make this more intuitive: treat lowest recorded as 0 and VIOLATION_THRESHOLD as 100%
	  value is cached off so we don't update it unnecessarily (causes ugly flickering) */
	  int display_value = (int)remap_float(loudness, lowest_loudness, VIOLATION_THRESHOLD, 0.0f, 100.0f);
	  if (display_value != old_display_value)
	  {
		update_display("Noise OK: %d%%", display_value);
		old_display_value = display_value;
	  }

      /* debug prints */
      printf("ok: %.2f\n", loudness);
    }
  }

  /* never reached */
  return 0;
}
