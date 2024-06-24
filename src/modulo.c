#include <Arduino.h>
#include <stdio.h>
//incluye timedeay


void parpadear_led(uint8_t led_pin){
    digitalWrite(led_pin,!digitalRead(led_pin));
}

void led_state(uint8_t led_pin, _Bool state){
    digitalWrite(led_pin,state);
}

_Bool nMEF(int16_t time_delay){ //Retardo no Bloqueante
    int16_t time_base=1;
    static int16_t count = 0;
    int16_t n_time = time_delay / time_base; //# de Muestras por pausa
    if (count>=n_time){
        count = 0;
        return true;
    }else{
        delay(time_base); //1ms de pausa repetida <count> veces
        count++;
        return false;
    }
}

_Bool leer_btn(uint8_t btn_pin){ 
    _Bool btn_state=digitalRead(btn_pin);
    return btn_state;
}


