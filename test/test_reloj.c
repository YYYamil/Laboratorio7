/*********************************************************************************************************************
Copyright (c) 2025, Yamil Tolaba <yamiltolaba@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*********************************************************************************************************************/

/** @file test_reloj.c
 ** @brief Plantilla para la creación de archivos de código fuente en lenguaje C
 **/

/* === Headers files inclusions ==================================================================================== */

#include "unity.h"
/*
union{              // es como si fuera una estruct pero ocupa solo 6 byte (3 grupos de 2), puedo verlo de 2 maneras. vble.time.second o .bcd
struct {
    uint8_t seconds[2];
    uint8_t minutes[2];
    uint8_t hours[2];
} time
uint8_t bcd[6]          //si lo llamo bcd son 6 elementos es un arreglo
};              //esto iria en clock .H

*/


/**
 Al inicializar el reloj está en 00:00 y con hora invalida.
‣ Al ajustar la hora el reloj queda en hora y es válida.
‣ Después de n ciclos de reloj la hora avanza un segundo, diez
segundos, un minutos, diez minutos, una hora, diez horas y
un día completo.
‣ Fijar la hora de la alarma y consultarla.
‣ Fijar la alarma y avanzar el reloj para que suene.
‣ Fijar la alarma, deshabilitarla y avanzar el reloj para no
suene.
‣ Hacer sonar la alarma y posponerla.
‣ Hacer sonar la alarma y cancelarla hasta el otro dia
 * 
 */



 // Al inicializar el reloj está en 00:00 y con hora invalida.

void test_set_up_with_invalid_time(void){

    clock_time_ t current_time;                         //hago un struct de 6 digitos solo para la hora

    clock_t clock = ClockCreate();
    //TEST_ASSERT_FALSE ( ClockTimeIsValid(clock));   //ESTO DEBE SER FALSO, DEBE FALLAR
    TEST_ASSERT_FALSE(ClockGetTime(clock, & current_time));                             //devuelve nro binario en un array 
    TEST_ASSERT_EACH_EQUAL_UINT8(0, current_time.bcd, 6);       // prueba que todo esta en cero, compara en bcd

 }
/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function implementation ============================================================================== */

void test_inicial(void){
    TEST_FAIL_MESSAGE("Empezamos");
}

/* === End of documentation ======================================================================================== */
