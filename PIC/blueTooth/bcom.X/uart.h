
#ifndef __UART_H__
#define __UART_H__

void config_uart();
void SendChar(char c);
void SendString(char *string);
char ReadChar(void);
void ReadString(char *string, int length);

#endif