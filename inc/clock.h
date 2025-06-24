#include <stdint.h>
#include <stdbool.h>

typedef union {
    uint8_t seconds[2];
    uint8_t minutes[2];
    uint8_t hours[2];
    uint8_t bcd[6];
} clock_time_t;

typedef struct clock_s *clock_t;

clock_t ClockCreate(uint16_t ticks_per_second);

bool ClockGetTime(clock_t clock, clock_time_t *result);

bool ClockSetTime(clock_t clock, const clock_time_t *new_time);

void ClockNewTick(clock_t clock);

bool ClockSetAlarmTime(clock_t clock, const clock_time_t *time);
bool ClockGetAlarmTime(clock_t clock, clock_time_t *time);

void ClockEnableAlarm(clock_t clock);
void ClockDisableAlarm(clock_t clock);
bool ClockIsAlarmEnabled(clock_t clock);


bool ClockIsAlarmTriggered(clock_t clock);
