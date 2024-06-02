#ifndef MODULO_H
#define MODULO_H

#include <stdio.h>
#include <modulo.c>

// Definición de la estructura para control de secuencia de LEDs
void parpadear_led(uint8_t led);
void led_state(uint8_t led_pin, _Bool state);
_Bool leer_btn(uint8_t btn_pin);

#endif