#include<stdint>

typedef union{              // es como si fuera una estruct pero ocupa solo 6 byte (3 grupos de 2), puedo verlo de 2 maneras. vble.time.second o .bcd
struct {
    uint8_t seconds[2];
    uint8_t minutes[2];
    uint8_t hours[2];
} time;
uint8_t bcd[6];          //si lo llamo bcd son 6 elementos es un arreglo
}clock_time_t;              //esto iria en clock .H