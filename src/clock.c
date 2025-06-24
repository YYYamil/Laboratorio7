#include "clock.h"
#include <stddef.h>
#include <string.h>

struct clock_s {
    clock_time_t current_time;
    bool valid;
};

clock_t ClockCreate(uint16_t ticks_per_second) {
    static struct clock_s self[1];
    memset(self, 0, sizeof(struct clock_s));
    self->valid = false;
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


void ClockNewTick(clock_t self){
    //self->current_time.time.seconds[0]=0;
}