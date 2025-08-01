#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include <stdbool.h>

typedef union {
    uint8_t bcd[6];
    struct {
        uint8_t sec_u, sec_d, min_u, min_d, hour_u, hour_d;
    };
} clock_time_t;

struct clock_s {
    bool valid;
    uint8_t ticks;
    uint8_t ticks_per_second;
    bool alarm_enabled;
    bool alarm_triggered;
    clock_time_t current_time;
    clock_time_t alarm_time;
};

typedef struct clock_s * clock_t;

clock_t ClockCreate(uint16_t ticks_per_second);
bool ClockSetTime(clock_t self, const clock_time_t * time);
bool ClockGetTime(clock_t self, clock_time_t * result);
bool ClockSetAlarmTime(clock_t self, const clock_time_t * alarm);
bool ClockGetAlarmTime(clock_t self, clock_time_t * alarm);
void ClockEnableAlarm(clock_t self);
void ClockDisableAlarm(clock_t self);
bool ClockIsAlarmEnabled(clock_t self);
bool ClockIsAlarmTriggered(clock_t self);
void ClockNewTick(clock_t self);

void ClockSnoozeAlarm(clock_t self, uint8_t minutes);
void ClockCancelAlarmForTomorrow(clock_t self);


#endif

