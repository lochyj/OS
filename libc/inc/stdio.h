
#ifndef _STDIO_H

#define _STDIO_H

#include "../src/stdio.c"

/**
 * @brief       Prints a formatted string to the standard output.
 * 
 * @param c     The format string.
 */
void putc(char c);

/**
 * @brief       Prints a formatted string to the standard output.
 * 
 * @param str   The formatted string.
 */
void puts(const char* str);

/**
 * @brief           Prints a formatted string to the screen.
 * 
 * @param format    The format string.
 * @param ...       The arguments to print into the format string.
 */
void printf(const char* format, ...);

/**
 * The same as <printf>, appending a new-line.
 *
 * @return
 * 		EOF if printing the new-line failed,
 * 		otherwise see <vprintf>
 */
void println(const char* format, ...);




#endif