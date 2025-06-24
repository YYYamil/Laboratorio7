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

clock_t clock;

void setUp(void){
    clock =ClockCreate(CLOCK_TICKS_PER_SECOND);
}

static void SimulateSeconds(clock_t clock, uint8_t seconds) {
    for (uint8_t i = 0; i < CLOCK_TICKS_PER_SECOND * seconds; i++) {
        ClockNewTick(clock);
    }
}





/* Al inicializar el reloj está en 00:00 y con hora inválida. */
void test_set_up_with_invalid_time(void) {
    clock_time_t current_time={
    .bcd = {1, 2, 3, 4, 5, 6}
    };

    //clock_t clock = ClockCreate(CLOCK_TICKS_PER_SECOND);
    
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
    //clock_t clock = ClockCreate(CLOCK_TICKS_PER_SECOND);

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

void test_clock_advance_ten_seconds(void) {
    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulateSeconds(clock, 10);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 1, 0, 0, 0, 0, current_time);  // 10 segundos
}

void test_clock_advance_one_minute(void) {
    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulateSeconds(clock, 60);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 0, 0, 1, 0, 0, current_time);  // 01:00
}

void test_clock_advance_ten_minutes(void) {
    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulateSeconds(clock, 600);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 0, 0, 0, 0, 1, current_time);  // 10:00
}

void test_clock_advance_one_hour(void) {
    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulateSeconds(clock, 3600);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 0, 0, 0, 1, 0, current_time);  // 01:00:00
}

void test_clock_advance_ten_hours(void) {
    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulateSeconds(clock, 36000);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 0, 0, 0, 0, 1, current_time);  // 10:00:00
}

void test_set_and_get_alarm_time(void) {
    clock_time_t alarm_time = {
        .seconds = {0, 0},
        .minutes = {0, 0},
        .hours = {5, 1} // 15:00:00
    };
    
    clock_time_t retrieved_time = {0};

    TEST_ASSERT_TRUE(ClockSetAlarmTime(clock, &alarm_time));
    TEST_ASSERT_TRUE(ClockGetAlarmTime(clock, &retrieved_time));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(alarm_time.bcd, retrieved_time.bcd, 6);
}


void test_alarm_does_not_trigger_when_disabled(void) {
    clock_time_t time = {
        .seconds = {0, 0},
        .minutes = {0, 0},
        .hours = {0, 0}
    };

    ClockSetTime(clock, &time);
    ClockSetAlarmTime(clock, &time);
    ClockDisableAlarm(clock);

    SimulateSeconds(clock, 1); // avanzar 1 segundo, alarma debería estar deshabilitada

    // Comprobamos que la alarma está deshabilitada y no sonaría (no hay "evento" aún)
    TEST_ASSERT_FALSE(ClockIsAlarmEnabled(clock));
}


void test_alarm_triggers_when_time_matches(void) {
    clock_time_t time = {
        .seconds = {0, 0},
        .minutes = {0, 0},
        .hours = {0, 0}
    };

    ClockSetTime(clock, &time);
    ClockSetAlarmTime(clock, &time);
    ClockEnableAlarm(clock);

    SimulateSeconds(clock, 1); // Simula 1 segundo

    TEST_ASSERT_TRUE(ClockIsAlarmTriggered(clock)); // La hora coincide con la alarma
}
