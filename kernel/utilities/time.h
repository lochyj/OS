#include <stdio.h>
#include <stdint.h>
#include "include/ports.h"
#include "kernel/inc/util.h"

#ifndef TIME_H

#define TIME_H

typedef struct Time {
        u8 second;
        u8 minute;
        u8 hour;
        // int month;
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
    port_byte_out(cmos_address, 0x0B);
    return (port_byte_in(cmos_data) & 0x80);
}

unsigned char get_RTC_register(int reg) {
    port_byte_out(cmos_address, reg);
    return port_byte_in(cmos_data);
}

Time get_kernel_time() {
    // init_time();
    
    while (get_update_in_progress_flag()){};    // Make sure an update isn't in progress

    port_byte_out(cmos_address, 0x00);
    u8 second = port_byte_in(cmos_data);
    port_byte_out(cmos_address, 0x02);
    u8 minute = port_byte_in(cmos_data);
    port_byte_out(cmos_address, 0x04);
    u8 hour = port_byte_in(cmos_data);


    port_byte_out(cmos_address, 0x08);
    u8 month = port_byte_in(cmos_data);

    // if month is betweeen october and april, its daylight savings time
    if (month < 10 || month >= 4) {
        hour -= 0x01;
    }

    Time time = {second, minute, hour};

    u8 registerB = get_RTC_register(0x0B);

    if (!(registerB & 0x04)){
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
        // day = (day & 0x0F) + ((day / 16) * 10);
        // month = (month & 0x0F) + ((month / 16) * 10);
        // year = (year & 0x0F) + ((year / 16) * 10);
        // if(century_register != 0) {
        //         century = (century & 0x0F) + ((century / 16) * 10);
        // }
    }

    if (!(registerB & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    }

    return time;
}

void print_time(Time time) {
    printf("%d:%d:%d\n", time.hour, time.minute, time.second);
}

#endif