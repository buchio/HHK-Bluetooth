#ifndef H_UART_H_130929061643_
#define H_UART_H_130929061643_

void Uart1Init();
void Uart1Putc( const char c );
void Uart1Puts( const char *str );
void Uart1Printf (char *fmt, ...);
int Uart1Write( char *dat, int szbyte );
int Uart1GetCh( void );
int Uart1QueueSize( void );


#endif //H_UART_H_130929061643_
