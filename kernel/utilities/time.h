#include "kernel/inc/types.h"
#include "kernel/drivers/ports.h"
#include "kernel/inc/util.h"

#ifndef TIME_H

#define TIME_H

typedef struct Time {
        int second;
        int minute;
        int hour;
        int month;
        // int year;
} Time;

enum {
    cmos_address = 0x70,
    cmos_data    = 0x71
};

/* Port cmos_address / cmos_data values
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

int get_update_in_progress_flag() {
    port_byte_out(cmos_address, 0x0A);
    return (port_byte_in(cmos_data) & 0x80);
}

void init_time() {
    port_byte_out(cmos_address, 0x8B);		    // Select register B, and disable NMI
    char prev = port_byte_in(cmos_data);	    // Read the current value of register B
    port_byte_out(cmos_address, 0x8B);		    // Set the index again (a read will reset the index to register D)
    port_byte_out(cmos_data, prev | 0x40);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

    port_byte_out(cmos_address, 0x0C);	// select register C
    port_byte_in(cmos_data);		    // just throw away contents
}

Time get_kernel_time() {
    while (get_update_in_progress_flag()){};    // Make sure an update isn't in progress
    port_byte_out(cmos_address, 0x00);
    u8 second = port_byte_in(cmos_data);
    port_byte_out(cmos_address, 0x02);
    u8 minute = port_byte_in(cmos_data);
    port_byte_out(cmos_address, 0x04);
    u8 hour = port_byte_in(cmos_data);
    port_byte_out(cmos_address, 0x08);
    u8 month = port_byte_in(cmos_data);
    port_byte_out(cmos_address, 0x04);
    // int year = port_byte_in(cmos_data);               // This is from 0 - 99
    // port_byte_out(cmos_address, 0x04);
    // /*u8*/ int century = port_byte_in(cmos_data);     // This is from 19 - 20?;

    Time time = {second, minute, hour, month};
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