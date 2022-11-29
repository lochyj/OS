#include "../inc/types.h"
#include "../drivers/ports.h"
#include "../inc/util.h"

#ifndef TIME_H

#define TIME_H

typedef struct Time {
        u8 second;
        u8 minute;
        u8 hour;
} Time;

Time get_kernel_time() {
    port_byte_out(0x70, 0x00);
    u8 second = port_byte_in(0x71);
    port_byte_out(0x70, 0x02);
    u8 minute = port_byte_in(0x71);
    port_byte_out(0x70, 0x04);
    u8 hour = port_byte_in(0x71);

    Time time = {second, minute, hour};
    return time;
}

void print_time(Time time) {
    print_int(time.hour);
    print_string(":");
    print_int(time.minute);
    print_string(":");
    print_int(time.second);
    print_nl();
}

#endif