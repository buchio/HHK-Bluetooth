// -*- coding:utf-8 -*-
/*
 *  Copyright (c) 2013 Yukio Obuchi
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation files
 *  (the "Software"), to deal in the Software without restriction,
 *  including without limitation the rights to use, copy, modify, merge,
 *  publish, distribute, sublicense, and/or sell copies of the Software,
 *  and to permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 *  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 *  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
// Shell

#include <string.h>
#include <stdio.h>

#include "../uart.h"
#include "../shell.h"

#include "../../Microchip/HardwareProfile.h"

#define SHELL_COMMAND_MAX (64)
struct shellCommandlist_t {
    const char *name;
    int (*command)( int, char** );
} shellCommandlist[ SHELL_COMMAND_MAX ];
    
static int ExecCommand( int argc, char* argv[] )
{
    int i;
    for( i = 0; i < SHELL_COMMAND_MAX; i++ ) {
        if( shellCommandlist[i].command != 0 &&
            !strcmp( argv[0], shellCommandlist[i].name ) ) {
            return (shellCommandlist[i].command)(argc-1, ++argv );
        }
    }
    printf( "  [%s] not found. Available commands are, \r\n", argv[0] );
    for( i = 0; i < SHELL_COMMAND_MAX; i++ ) {
        if( shellCommandlist[i].name != 0 ) {
            printf( "    %s\r\n", shellCommandlist[i].name );
        }
    }
    return -1;
}

static int TestCommand( int argc, char* argv[] )
{
    int i;
    printf( "TestCommand\r\n" );
    for( i = 0; i < argc; i++ ) {
        printf( "  ARG:%d [%s]\r\n", i, argv[i] );
    }
    return 0;
}

#define LINE_LENGTH (128)
#define NUM_OF_ARG  (64)

static const char* uart1ShellPrompt = "SHELL> ";
static char uart1ShellLineBuffer[ LINE_LENGTH ];
static int  uart1ShellArgc = 0;
static char *uart1ShellArgv[ NUM_OF_ARG  ];
static int  uart1ShellLinePointer = 0;

void ShellInit( void )
{
    int i;
    puts( "Starting shell." );
    printf( uart1ShellPrompt );
    for( i = 0; i < SHELL_COMMAND_MAX; i++ ) {
        shellCommandlist[ i ].name = 0;
        shellCommandlist[ i ].command = 0;
    }
    ShellAddCommand( "test", TestCommand );
}

void ShellAddCommand( const char *name, int (*command)( int, char** ) )
{
    int i;
    for( i = 0; i < SHELL_COMMAND_MAX; i++ ) {
        if( shellCommandlist[i].command == 0 ) {
            shellCommandlist[i].name = name;
            shellCommandlist[i].command = command;
            break;
        }
    }
}

void Shell( void )
{
    int c = Uart1GetCh();

    if( c != -1 ) {
        if ( c < ' ' ) {
            if( c == '\n' || c == '\r' ) {
                if( uart1ShellLinePointer > 0 ) {
                    puts( "" );

                    if( uart1ShellLineBuffer[ uart1ShellLinePointer-1 ] == ' ' ) {
                        uart1ShellLinePointer -= 1;
                    }
                    uart1ShellLineBuffer[ uart1ShellLinePointer ] = '\0';
                    
                    uart1ShellArgc = 0;
                    uart1ShellArgv[ uart1ShellArgc++ ] = &(uart1ShellLineBuffer[0]);
                    int i;
                    for( i = 1; i < uart1ShellLinePointer && uart1ShellArgc < NUM_OF_ARG-1; i++ ) {
                        if( uart1ShellLineBuffer[i] <= ' ' ) {
                            uart1ShellLineBuffer[i] = '\0';
                            uart1ShellArgv[ uart1ShellArgc++ ] = &(uart1ShellLineBuffer[i+1]);
                        }
                    }
                
                    ShellExecCommand( uart1ShellArgc, uart1ShellArgv );

                    uart1ShellLinePointer = 0;
                    
                } else {
                    puts( "" );
                }
                printf( uart1ShellPrompt );
            } else {
                printf("[0x%02X]", c);
            }
            
        } else if( c > ' ' ) {
            if( uart1ShellLinePointer < LINE_LENGTH-1 ) {
                putchar( c );
                uart1ShellLineBuffer[ uart1ShellLinePointer++ ] = c;
            }
        } else {
            if( uart1ShellLinePointer > 0 &&
                uart1ShellLinePointer < LINE_LENGTH-1 &&
                uart1ShellLineBuffer[ uart1ShellLinePointer-1 ] != ' ' ) {
                putchar( c );
                uart1ShellLineBuffer[ uart1ShellLinePointer++ ] = c;
            }
            
        }
    }

    if( Uart1SendQueueSize() == 0 && Uart1ReceiveQueueSize() == 0 ) {
        Uart1Flush();
        Idle();
    }
}
