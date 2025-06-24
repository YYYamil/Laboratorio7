#include "clock.h"
#include <stddef.h>
#include <string.h>

struct clock_s {
    clock_time_t current_time;
    clock_time_t alarm_time;

    bool valid;
    bool alarm_enabled;
    bool alarm_triggered;

    uint16_t ticks_per_second;


};

clock_t ClockCreate(uint16_t ticks_per_second) {
    static struct clock_s self[1];
    memset(self, 0, sizeof(struct clock_s));
    self->valid = false;
    self->ticks_per_second = ticks_per_second;
    self->alarm_triggered = false;


    return self;
}

bool ClockGetTime(clock_t self, clock_time_t *result) {
    memcpy(result, &self->current_time, sizeof(clock_time_t));
    return self->valid;
}

bool ClockSetTime(clock_t self, const clock_time_t *new_time) {
    self->valid = true;
    memcpy(&self->current_time, new_time, sizeof(clock_time_t));
    return self->valid;
}


void ClockNewTick(clock_t self) {
    static uint16_t ticks = 0;
    static const uint8_t max_bcd[] = {9, 5, 9, 5, 9, 2}; // 23:59:59 en BCD

    if (!self->valid) return;

    if (++ticks < self->ticks_per_second) return;
    ticks = 0;

    uint8_t *bcd = self->current_time.bcd;

    // Avanzar unidad de segundos
    if (++bcd[0] > 9) {
        bcd[0] = 0;
        // Avanzar decena de segundos
        if (++bcd[1] > 5) {
            bcd[1] = 0;
            // Avanzar unidad de minutos
            if (++bcd[2] > 9) {
                bcd[2] = 0;
                // Avanzar decena de minutos
                if (++bcd[3] > 5) {
                    bcd[3] = 0;
                    // Avanzar unidad de horas
                    if (++bcd[4] > 9 || (bcd[5] == 2 && bcd[4] > 3)) {
                        bcd[4] = 0;
                        // Avanzar decena de horas
                        if (++bcd[5] > 2) {
                            bcd[5] = 0;
                        }
                    }
                }
            }
        }
    }

    // Verificar si la alarma se debe activar
if (self->alarm_enabled &&
    memcmp(&self->current_time, &self->alarm_time, sizeof(clock_time_t)) == 0) {
    self->alarm_triggered = true;
} else {
    self->alarm_triggered = false;
}

}

bool ClockSetAlarmTime(clock_t self, const clock_time_t *time) {
    if (!self || !time) return false;
    memcpy(&self->alarm_time, time, sizeof(clock_time_t));
    return true;
}

bool ClockGetAlarmTime(clock_t self, clock_time_t *time) {
    if (!self || !time) return false;
    memcpy(time, &self->alarm_time, sizeof(clock_time_t));
    return true;
}


void ClockEnableAlarm(clock_t self) {
    if (self) self->alarm_enabled = true;
}

void ClockDisableAlarm(clock_t self) {
    if (self) self->alarm_enabled = false;
}

bool ClockIsAlarmEnabled(clock_t self) {
    return self ? self->alarm_enabled : false;
}



bool ClockIsAlarmTriggered(clock_t self) {
    return self ? self->alarm_triggered : false;
}
