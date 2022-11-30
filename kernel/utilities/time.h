#include "../inc/types.h"
#include "../drivers/ports.h"
#include "../inc/util.h"

#ifndef TIME_H

#define TIME_H

typedef struct Time {
        int second;
        int minute;
        int hour;
        int month;
        int year;
} Time;

/* Port 0x70 / 0x71 values
 Out       Get                 Format
 0x00      Seconds             0–59
 0x02      Minutes             0–59
 0x04      Hours               0–23 in 24-hour mode, 
                               1–12 in 12-hour mode, highest bit set if pm
 0x06      Weekday             1–7, Sunday = 1
 0x07      Day of Month        1–31
 0x08      Month               1–12
 0x09      Year                0–99
 0x32      Century (maybe)     19–20?
 0x0A      Status Register A
 0x0B      Status Register B
*/

void init_time() {
    port_byte_out(0x70, 0x8B);		    // Select register B, and disable NMI
    char prev = port_byte_in(0x71);	    // Read the current value of register B
    port_byte_out(0x70, 0x8B);		    // Set the index again (a read will reset the index to register D)
    port_byte_out(0x71, prev | 0x40);	// write the previous value ORed with 0x40. This turns on bit 6 of register B
}

Time get_kernel_time() {
    port_byte_out(0x70, 0x00);
    int second = port_byte_in(0x71);
    port_byte_out(0x70, 0x02);
    int minute = port_byte_in(0x71);
    port_byte_out(0x70, 0x04);
    int hour = port_byte_in(0x71);
    port_byte_out(0x70, 0x08);
    int month = port_byte_in(0x71);
    port_byte_out(0x70, 0x04);
    int year = port_byte_in(0x71);       // This is from 0 - 99
    port_byte_out(0x70, 0x04);
    /*u8*/ int century = port_byte_in(0x71);    // This is from 19 - 20?

    int years;
    append_int_to_int(years, century);
    append_int_to_int(years, year);

    Time time = {second, minute, hour, month, years};
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