#include <stdint.h>

#ifndef UTILITY_H

#define UTILITY_H

#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

int int_length(int n) {
    int i = 0;
    while (n != 0) {
        n /= 10;
        ++i;
    }
    return i;
}

int string_length(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void reverse(char string[]) {
    int c, i, j;
    for (i = 0, j = string_length(string)-1; i < j; i++, j--) {
        c = string[i];
        string[i] = string[j];
        string[j] = c;
    }
}

void int_to_string(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

void memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void append(char s[], char n) {
    int len = string_length(s);
    s[len] = n;
    s[len+1] = '\0';
}

int append_int_to_int(int n, int m) {
    int length = int_length(m);
    for (int i = 0; i < length; i++) {
        int digit = m % 10;
        n = n * 10 + digit;
        m /= 10;
    }
    return n;
}

int hex_to_int(u8 hex) {
    int n = (int) hex;
    return n;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_space(char c) {
    return c == ' ';
}

bool backspace(char s[]) {
    int len = string_length(s);
    if (len > 0) {
        s[len - 1] = '\0';
        return true;
    } else {
        return false;
    }
}

bool is_upper(char c) {
    return c >= 'A' && c <= 'Z';
}

bool is_lower(char c) {
    return c >= 'a' && c <= 'z';
}

char to_upper(char c) {
    if (is_lower(c)) {
        return c - 32;
    } else {
        return c;
    }
}

char to_lower(char c) {
    if (is_upper(c)) {
        return c + 32;
    } else {
        return c;
    }
}

char* to_upper_string(char s[]) {
    int len = string_length(s);
    for (int i = 0; i < len; i++) {
        s[i] = to_upper(s[i]);
    }
    return s;
}

char* to_lower_string(char s[]) {
    int len = string_length(s);
    for (int i = 0; i < len; i++) {
        s[i] = to_lower(s[i]);
    }
    return s;
}

int char_to_int(char c) {
    return c - '0';
}

int string_to_int(char string[]) {
    int i, n, sign;
    for (i = 0; is_space(string[i]); i++);
    sign = (string[i] == '-') ? -1 : 1;
    if (string[i] == '+' || string[i] == '-') i++;
    for (n = 0; is_digit(string[i]); i++) {
        n = 10 * n + (string[i] - '0');
    }
    return sign * n;
}

/* K&R
 * Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 
 */
int compare_string(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

int compare_char(char c1, char c2) {
    return c1 - c2;
}

#endif