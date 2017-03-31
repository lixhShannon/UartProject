#ifndef __UART_SEND_H__
#define __UART_SEND_H__

//extern char uart_check(char *buf);
extern void sendData(unsigned char* buf, unsigned char CANID);
extern void uart_send(unsigned char* buf);



#endif