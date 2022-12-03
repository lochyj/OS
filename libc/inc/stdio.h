

#ifndef _STDIO_H

#define _STDIO_H

#include "../src/printf.c"
#include "../src/println.c"

/**
 * Formatted printing to <stdout>
 *
 * @param format
 * 		format string to use
 * @param args
 * 		optional list of arguments
 * @return
 *      if the function succeeds, the total number of characters written is returned.
 */
int printf(const char* format, ...);

/**
 * The same as <printf>, appending a new-line.
 *
 * @return
 * 		EOF if printing the new-line failed,
 * 		otherwise see <vprintf>
 */
int println(const char* format, ...);

#endif