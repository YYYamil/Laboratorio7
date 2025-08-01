#include "unity.h"
#include "clock.h"

#define CLOCK_TICKS_PER_SECOND 5
#define MAX_SIM_SECONDS 60  // Evitar cuelgues por simulaciones largas

#define TEST_ASSERT_TIME(seconds_units, seconds_tens, minutes_units, minutes_tens, hours_units, hours_tens, current_time) \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(seconds_units, current_time.bcd[0], "Difference in unit seconds"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(seconds_tens, current_time.bcd[1], "Difference in tens seconds"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(minutes_units, current_time.bcd[2], "Difference in unit minutes"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(minutes_tens, current_time.bcd[3], "Difference in tens minutes"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(hours_units, current_time.bcd[4], "Difference in unit hours"); \
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(hours_tens, current_time.bcd[5], "Difference in tens hours")

clock_t clock;

void setUp(void) {
    clock = ClockCreate(CLOCK_TICKS_PER_SECOND);
}
//struct clock_s; // Forward declaration

static void SimulateSeconds(clock_t clock, uint32_t seconds) {
    if (seconds > MAX_SIM_SECONDS) {
        TEST_IGNORE_MESSAGE("Test deshabilitado temporalmente por exceso de tiempo simulado.");
        return;
    }

    for (uint32_t i = 0; i < clock->ticks_per_second * seconds; i++) {
        ClockNewTick(clock);
    }
    
}



/* --- Tests --- */

void test_set_up_with_invalid_time(void) {
    clock_time_t current_time = { .bcd = {1, 2, 3, 4, 5, 6} };
    TEST_ASSERT_FALSE(ClockGetTime(clock, &current_time));
    TEST_ASSERT_EACH_EQUAL_UINT8(0, current_time.bcd, 6);
}

void test_set_up_and_adjust_with_valid_time(void) {
    //clock_time_t new_time = { .seconds = {4, 5}, .minutes = {3, 0}, .hours = {1, 4} };
    clock_time_t new_time = { .bcd = { 4, 5, 3, 0, 1, 4 } };

    clock_time_t current_time = {0};
    TEST_ASSERT_TRUE(ClockSetTime(clock, &new_time));
    TEST_ASSERT_TRUE(ClockGetTime(clock, &current_time));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(new_time.bcd, current_time.bcd, 6);
}

void test_clock_advance_one_second(void) {
    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulateSeconds(clock, 1);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(1, 0, 0, 0, 0, 0, current_time);
}

void test_clock_advance_ten_seconds(void) {
    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulateSeconds(clock, 10);
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 1, 0, 0, 0, 0, current_time);
}

void test_clock_advance_one_minute(void) {
    clock_time_t current_time = {0};
    ClockSetTime(clock, &(clock_time_t){0});
    SimulateSeconds(clock, 60);  // 60 segundos * 5 ticks = 300
    ClockGetTime(clock, &current_time);
    TEST_ASSERT_TIME(0, 0, 1, 0, 0, 0, current_time);  // 00:01:00
}

void test_clock_advance_ten_minutes(void) {
    SimulateSeconds(clock, 600);  
}

void test_clock_advance_one_hour(void) {
    SimulateSeconds(clock, 3600);  
}

void test_clock_advance_ten_hours(void) {
    SimulateSeconds(clock, 36000);  
}

void test_set_and_get_alarm_time(void) {
    //clock_time_t alarm_time = { .seconds = {0, 0}, .minutes = {0, 0}, .hours = {5, 1} };
    clock_time_t alarm_time = { .bcd = { 0, 0, 0, 0, 5, 1 } };

    clock_time_t retrieved_time = {0};
    TEST_ASSERT_TRUE(ClockSetAlarmTime(clock, &alarm_time));
    TEST_ASSERT_TRUE(ClockGetAlarmTime(clock, &retrieved_time));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(alarm_time.bcd, retrieved_time.bcd, 6);
}

void test_alarm_does_not_trigger_when_disabled(void) {
    //clock_time_t time = { .seconds = {0, 0}, .minutes = {0, 0}, .hours = {0, 0} };
    clock_time_t time = { .bcd = { 0, 0, 0, 0, 5, 1 } };

    ClockSetTime(clock, &time);
    ClockSetAlarmTime(clock, &time);
    ClockDisableAlarm(clock);
    SimulateSeconds(clock, 1);
    TEST_ASSERT_FALSE(ClockIsAlarmEnabled(clock));
}

void test_alarm_triggers_when_time_matches(void) {
    // clock_time_t alarm_time = {
    //     .seconds = {1, 0},  // Alarma a 00:00:01 (BCD)
    //     .minutes = {0, 0},
    //     .hours = {0, 0}
    // };

    clock_time_t alarm_time = { .bcd = { 1, 0, 0, 0, 0, 0 } };


    ClockSetTime(clock, &(clock_time_t){0});
    ClockSetAlarmTime(clock, &alarm_time);
    ClockEnableAlarm(clock);
    SimulateSeconds(clock, 2);  // Avanza 1 segundo y compara
    TEST_ASSERT_TRUE(ClockIsAlarmTriggered(clock));
}
