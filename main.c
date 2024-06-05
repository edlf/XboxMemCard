// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "pico/stdlib.h"
#include "tusb.h"

static void blink_cb(void *arg) {  // Blink periodically
  gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get_out_level(PICO_DEFAULT_LED_PIN));
  (void) arg;
}


int main(void) {
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  stdio_init_all();

  tusb_init();

  for (;;) {
    tud_task();
  }

  return 0;
}