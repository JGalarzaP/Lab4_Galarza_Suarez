#include <Arduino.h>
#include <stdio.h>

void parpadear_led(uint8_t led_pin){
    digitalWrite(led_pin,!digitalRead(led_pin));
}

void led_state(uint8_t led_pin, _Bool state){
    digitalWrite(led_pin,state);
}

_Bool leer_btn(uint8_t btn_pin){
    _Bool btn_state=digitalRead(btn_pin);
    return btn_state;
}

