#include <Arduino.h>
#include <stdio.h>
#include "modulo.h" 

//Integrantes: Jairo Galarza, Joel Suarez. 
//Control Digital
// 03/06/24 
// Definición de Pines
typedef enum:uint8_t{ //Pines de Salida
    led_r = 4,
    led_y = 5,
    led_g = 6,
    led_b = 13
} gpioMapLeds_t;
typedef enum:uint8_t{ //Pines de Entrada
    SW1 = 2,
    SW2 = 3,
    SW3 = 10,
    SW4 = 9
} gpioMapBtns_t;

// Definición de Estados
typedef enum:bool{  //Define con que valor se encienden los leds (Segun Hardware)
    on = HIGH,
    off = LOW
} boolLeds_t;
typedef enum:bool{  //Se cambia si el boton esta en PullUp o PullDown
    pull = HIGH,
    down = LOW
} boolBtns_t;
typedef enum:bool { //Control de Orientacion
    ascendente=true,
    descendente=false
} orientacion_t;
typedef enum:uint16_t{ //Tiempos Preestablecidos
    time1 = 200,
    time2 = 750 
} tiempoleds_t;
// Definición de la estructura para control de secuencia de LEDs
  //el cual define los parametros de importancia para controlarla
typedef struct {
  const gpioMapLeds_t *secuencia;  // Puntero a la secuencia de LEDs
  tiempoleds_t tiempo;           // Tiempo entre cambios de LED
  orientacion_t orientacion;     // Orientación de la secuencia (ascendente o descendente)
} controlSecuencia_t;


//Declaracion de Secuencias Escalables y Prioridades
const gpioMapLeds_t leds_sec[] = {led_g, led_y, led_r, led_b}; //Secuencia (ordenada)
const uint8_t leds_dimen = sizeof(leds_sec) / sizeof(gpioMapLeds_t);
const gpioMapBtns_t btns_entr[] = {SW1, SW2, SW3, SW4}; // Mayor a Menor Prioridad 
const uint8_t btns_dimen = sizeof(btns_entr) / sizeof(gpioMapBtns_t);
controlSecuencia_t control1;

/*
  ********************* 
  INICIALIZACION DE FUNCIONES ADICIONALES
  *********************
*/
void configurarPines(const gpioMapLeds_t *leds, const uint8_t numLeds,
                    const gpioMapBtns_t *btns, const uint8_t numBtns);
void inicializarSecuencia(controlSecuencia_t *control, const gpioMapLeds_t *secuencia, 
                    tiempoleds_t tiempo, orientacion_t orientacion);
void ejecutarSecuencia(controlSecuencia_t *control, const uint8_t numLeds);
void comprobarBtns(const gpioMapBtns_t *btns, controlSecuencia_t *control,
                  const uint8_t numBtns);
boolBtns_t state_Tecla(uint8_t tecla); 
bool apagarLeds(); 

/*
  *********************
  FUNCIONES PRINCIPALES 
  *********************
*/
void setup() {
  Serial.begin(9600); //Para pruebas
  configurarPines(leds_sec, leds_dimen, btns_entr, btns_dimen);
  inicializarSecuencia(&control1, leds_sec, time1, ascendente);
  apagarLeds();
}


void loop() {
  comprobarBtns(btns_entr, &control1, btns_dimen);
  ejecutarSecuencia(&control1, leds_dimen);
}

/*
  *********************
  FUNCIONES ADICIONALES
  *********************
*/
void configurarPines(const gpioMapLeds_t *leds, const uint8_t numLeds,
                    const gpioMapBtns_t *btns, const uint8_t numBtns){
  //Configura y establece el valor inicial de los pines de Salida y Entrada
  for (uint8_t i=0; i<numLeds; ++i) {
    pinMode(leds[i], OUTPUT);}
  for (uint8_t i = 0; i<numBtns; ++i) {
    pinMode(btns[i], INPUT);}
}


bool apagarLeds() {
  //Funcion que permite Apagar todos los leds
  for (uint8_t i = 0; i<leds_dimen; i++){
    led_state(leds_sec[i],off);}
  return true;
}

boolBtns_t state_Tecla(uint8_t tecla) {
  //Funcion que permite Leer el estado de la Tecla
  if (leer_btn(tecla)) {
    return down;
  }else{
    delay(50); //Antirebote
    return pull;
  }
}

void comprobarBtns(const gpioMapBtns_t *btns, controlSecuencia_t *control, 
                  const uint8_t numBtns){
  //Funcion que Permite Intercambial el Valor de Control de Secuencia y ...
    //tambien definir la prioridad de los botones (PROGRAMACION DEFENSIVA)
  for (uint8_t i=0; i<numBtns; ++i){
    if(state_Tecla(btns[i])==pull){
      switch (btns[i]){
      case SW1: control->orientacion = descendente;
        return;
      case SW2: control->orientacion = ascendente;
        return;
      case SW3: control->tiempo = time1;
        return;
      case SW4: control->tiempo = time2;
        return;
      }
    break; //Rompe la Lectura de los Otros Botones (Prioridad)
    }
  }
}

void inicializarSecuencia(controlSecuencia_t *control, const gpioMapLeds_t *secuencia, 
  tiempoleds_t tiempo, orientacion_t orientacion) {
  // Función para inicializar una secuencia 
  control->secuencia = secuencia;
  control->tiempo = tiempo;
  control->orientacion = orientacion;
}


void ejecutarSecuencia(controlSecuencia_t *control, const uint8_t numLeds) {
  // Función para manejar la secuencia de LEDs
  static int8_t index =  0;  // Índice estático para mantener el estado entre llamadas
  static unsigned long lastTime = 0;  // Almacenar el último tiempo que se actualizo
  unsigned long currentTime = millis();  // Obtener el tiempo actual

  if (currentTime - lastTime >= control->tiempo){ //Retardo no Bloqueante
    led_state(control->secuencia[index], on);
    if (control->orientacion==ascendente){
      index += 1;
      if (index>numLeds){ index=0; apagarLeds();}
    }else{
      if(index<0){ index=numLeds; apagarLeds();}
      index -= 1; 
    }
    lastTime = millis();
  }  
}

