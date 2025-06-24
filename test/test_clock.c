#include "unity.h"
#include "clock.h"
// Al ajustar la hora el reloj queda en hora y es válida.
// - Después de n ciclos de reloj la hora avanza un segundo, diez segundos, un minutos, diez minutos, una hora, diez
// horas. Fijar la hora de la alarma y consultarla.
// - Fijar la alarma y deshabilitarla y avanzar el reloj para no suene.
// - Hacer sonar la alarma y posponerla.
// - Hacer sonar la alarma y cancelarla hasta el otro día...
// - Probar get_time con NULL como argumento


#define CLOCK_TICKS_PER_SECOND 5
#define TEST_ASSERT_TIME(seconds_units, seconds_tens, minutes_units, minutes_tens, hours_units, hours_tens, current_time) \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(seconds_units, current_time.bcd[0], "Difference in unit seconds"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(seconds_tens, current_time.bcd[1], "Difference in tens seconds"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(minutes_units, current_time.bcd[2], "Difference in unit minutes"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(minutes_tens, current_time.bcd[3], "Difference in tens minutes"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(hours_units, current_time.bcd[4], "Difference in unit hours"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(hours_tens, current_time.bcd[5], "Difference in tens hours")


static void SimulateSeconds(clock_t clock, uint8_t seconds) {
    for (uint8_t i = 0; i < CLOCK_TICKS_PER_SECOND * seconds; i++) {
        ClockNewTick(clock);
    }
}

clock_t clock;

void setUp(void){
    clock =ClockCreate(CLOCK_TICKS_PER_SECOND);
}



/* Al inicializar el reloj está en 00:00 y con hora inválida. */
void test_set_up_with_invalid_time(void) {
    clock_time_t current_time={
    .bcd = {1, 2, 3, 4, 5, 6}
    };

    clock_t clock = ClockCreate(CLOCK_TICKS_PER_SECOND);
    
    TEST_ASSERT_FALSE(ClockGetTime(clock, &current_time));
    TEST_ASSERT_EACH_EQUAL_UINT8(0, current_time.bcd, 6);
}
//Al ajusta la hora el reloj con valores correctos, queda en hora y es valida
void test_set_up_and_adjust_with_valid_time(void){
    clock_time_t new_time = {
        .seconds = {4, 5},
        .minutes = {3, 0},
        .hours = {1, 4}
    };

    clock_time_t current_time = {0};
    clock_t clock = ClockCreate(CLOCK_TICKS_PER_SECOND);

    TEST_ASSERT_TRUE(ClockSetTime(clock, &new_time));
    TEST_ASSERT_TRUE(ClockGetTime(clock, &current_time));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(new_time.bcd, current_time.bcd, 6);
}

// Después de n ciclos de reloj la hora avanza un segundo
void test_clock_advance_one_second(void) {
    clock_time_t current_time = {0};
    

    // Set initial time to 00:00:00
    ClockSetTime(clock, &(clock_time_t){0});

    SimulateSeconds(clock, 1);
    ClockGetTime(clock, &current_time);
    
    TEST_ASSERT_TIME(1,0,0,0,0,0,current_time);
}
