#include <stdio.h>
#include <stdarg.h>

#include "../uart.h"


static void print_number (int base, int unsigned_p, long n, int hexcase, int len, int fill);
static void write_char ( char c );
static void write_string ( char *s);

void Uart1Printf (char *fmt, ...)
{
    va_list args;

    va_start (args, fmt);

    while (*fmt) {
        char c, *p;
        long l;
        int *value, len=0, fill=0;
      
        if (*fmt != '%' || *++fmt == '%') {
            if ( *fmt == '\n' ) {
                write_char ('\r');
            }
            write_char (*fmt++);
            continue;
        }

        value = &len;
        while( *fmt ) {
            c = *fmt;
            if ( c >= '0' && c <= '9' ) {
                *value = ((*value) * 10) + (c - '0');
                fmt++;
            }
            else if ( c == '.' ) {
                value = &fill;
                fmt++;
            } else {
                break;
            }
        }
      
        switch (c = *fmt++) {
            case 'c' :
                c = va_arg (args, int);
                write_char (c);
                break;
            case 'p' :
                l = (unsigned int) va_arg (args, char *);
                print_number (16, 1, l, 0, 8, 8);
                break;
            case 'd' :
            case 'u' :
            case 'x' :
            case 'X' :
                l = va_arg (args, int);
                print_number ((c == 'x' || c == 'X') ? 16 : 10, c != 'd', l, c == 'X', len, fill );
                break;
            case 's' :
                p = va_arg (args, char *);
                write_string (p);
                break;
        }
    }

    va_end (args);
}


/* Print X in base BASE.  */

static void print_number (int base, int unsigned_p, long n, int hexcase, int len, int fill)
{
    static const char chars_l[16] = "0123456789abcdef";
    static const char chars_u[16] = "0123456789ABCDEF";
    int p;
    char buf[32];
    unsigned long x;

    if (!unsigned_p && n < 0) {
        write_char ('-');
        x = -n;
    } else {
        x = n;
    }

    p = 31;
    buf[p] = 0;
    if ( n == 0 ) {
        buf[--p] = '0';
        len--;
        fill--;
    }
    
    while( 1 ) {
        if ( x == 0 ) {
            if ( len <= 0 ) {
                break;
            }
            
            if ( fill > 0 ) {
                buf[--p] = '0';
            } else {
                buf[--p] = ' ';
            }
        } else {
            if ( hexcase ) {
                buf[--p] = chars_u[x % base];
            } else {
                buf[--p] = chars_l[x % base];
            }
            x /= base;
        }
        len--;
        fill--;
    }

    write_string(&(buf[p]));
}

static void write_string( char *p )
{
    while( *p ) {
        write_char( *p );
        p++;
    }
}

#ifndef __TEST_ON_PC__
static void write_char( char c )
{
    Uart1Putc( c );
}
#else
#include <stdio.h>

static void write_char( char c )
{
    putc( c, stdout );
}

int main( int argc, char **argv )
{
    int i;
    Uart1Printf ( "Hello\n" );
    for ( i = 0; i < 0x10000; i += 0x1234 ) {
        printf( "Test for [%X]\n", i );
        Uart1Printf ( "Hello %6X, %6x, %8.6x, %8.5X, %8.8x, %8.8X\n", i, i, i, i, i, i );
        Uart1Printf ( "    %8.8X\n", i );
        Uart1Printf ( "    %4.4X\n", i );
        Uart1Printf ( "    %2.2X\n", i );
        Uart1Printf ( "    %2.2X\n", i );
        Uart1Printf ( "    %4.4X\n", i );
        Uart1Printf ( "    %8.8X\n", i );
    }
    Uart1Printf ( "%s\n", "String Test" );
    return  0;
}

#endif


/*
 * Local variables:
 *  compile-command: "gcc -Wall -g  -I . -D__TEST_ON_PC__ uart_printf.c -o uart_printf"
 * End:
 */
