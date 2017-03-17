#ifndef _xprintF_H_
#define _xprintF_H_

#define printf_format_idt(x) \
    _Generic((x), \
             char: "%c", \
             signed char: "%hhd", \
             unsigned char: "%hhu", \
             short: "%hd", \
             unsigned short: "%hd", \
             int: "%d", \
             unsigned int: "%u", \
             long int: "%ld", \
             unsigned long int: "%lu", \
             long long int: "%lld", \
             unsigned long long int: "%llu", \
             float: "%f", \
             double: "%lf", \
             long double: "%Lf", \
             char *: "%s", \
             void *: "%p")

#define xprint(x) \
    printf(printf_format_idt(x), (x))
#define xprintln(x) \
    xprint(x); printf("\n")

#endif
