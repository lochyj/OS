#include <kernel/drivers/vga.h>
#include <kernel/inc/types.h>

void va_start(ap, format) {
    ap->gp_offset = 0;
    ap->fp_offset = 0;
    ap->overflow_arg_area = NULL;
    ap->reg_save_area = NULL;
}

int printf(const char *format, ...) {
    // inject the args into the format string
    va_list args;

    va_start(args, format);

    // format the string
    char* buffer = format;

    for (int i = 0; *buffer != '\0'; buffer++) {
        if (*buffer == '%') {
            buffer++;
            switch (*buffer) {
                case 'c':
                    vga_putchar(va_arg(args, int));
                    break;
                case 's':
                    vga_puts(va_arg(args, char*));
                    break;
                case 'd':
                    vga_putint(va_arg(args, int));
                    break;
                case 'x':
                    vga_puthex(va_arg(args, int));
                    break;
                case '%':
                    vga_putchar('%');
                    break;
            }
        } else {
            vga_putchar(*buffer);
        }
    }

    // print the string
    print_string(buffer);
}