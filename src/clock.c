#include "clock.h"
#include <stddef.h>
#include <string.h>



clock_t ClockCreate(uint16_t ticks_per_second) {
    static struct clock_s self[1];
    memset(self, 0, sizeof(struct clock_s));
    self->valid = false;                // Reloj inválido al crear
    self->ticks = 0;
    self->ticks_per_second = ticks_per_second;
    self->alarm_enabled = false;
    self->alarm_triggered = false;
    return self;
}




bool ClockGetTime(clock_t self, clock_time_t *result) {
    if (!self || !self->valid) {
        if (result) memset(result, 0, sizeof(clock_time_t));
        return false;
    }
    if (result) memcpy(result, &self->current_time, sizeof(clock_time_t));
    return true;
}



bool ClockSetTime(clock_t self, const clock_time_t * time) {
    if (!self || !time) return false;
    memcpy(&self->current_time, time, sizeof(clock_time_t));
    self->ticks = 0;
    self->valid = true;
    return true;
}



void ClockNewTick(clock_t self) {
    if (!self || !self->valid) return;

    self->ticks++;
    if (self->ticks < self->ticks_per_second) return;
    self->ticks = 0;

    uint8_t *bcd = self->current_time.bcd;

    // Avance BCD: [0]=sec_u, [1]=sec_d, [2]=min_u, [3]=min_d, [4]=hour_u, [5]=hour_d
    if (++bcd[0] > 9) {
        bcd[0] = 0;
        if (++bcd[1] > 5) {
            bcd[1] = 0;

            if (++bcd[2] > 9) {
                bcd[2] = 0;
                if (++bcd[3] > 5) {
                    bcd[3] = 0;

                    if (++bcd[4] > 9) {
                        bcd[4] = 0;
                        if (++bcd[5] > 2 || (bcd[5] == 2 && bcd[4] > 3)) {
                            bcd[5] = 0;
                            bcd[4] = 0;
                        }
                    }
                }
            }
        }
    }

    // Verificar y disparar alarma
    if (self->alarm_enabled &&
        memcmp(&self->current_time, &self->alarm_time, sizeof(clock_time_t)) == 0) {
        self->alarm_triggered = true;
    }
}


bool ClockSetAlarmTime(clock_t self, const clock_time_t * alarm) {
    if (!self || !alarm) return false;
    memcpy(&self->alarm_time, alarm, sizeof(clock_time_t));
    return true;
}

bool ClockGetAlarmTime(clock_t self, clock_time_t * alarm) {
    if (!self || !alarm) return false;
    memcpy(alarm, &self->alarm_time, sizeof(clock_time_t));
    return true;
}



void ClockEnableAlarm(clock_t self) {
    if (self) self->alarm_enabled = true;
}
void ClockDisableAlarm(clock_t self) {
    if (self) self->alarm_enabled = false;
}
bool ClockIsAlarmEnabled(clock_t self) {
    return self && self->alarm_enabled;
}
bool ClockIsAlarmTriggered(clock_t self) {
    return self && self->alarm_triggered;
}
