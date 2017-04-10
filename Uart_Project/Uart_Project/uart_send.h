#ifndef __UART_SEND_H__
#define __UART_SEND_H__

//extern char uart_check(char *buf);
extern void sendDataPack(unsigned char* buf);
extern void uart_send(unsigned char* buf);
extern void Uart_Handle(unsigned char* recv_buf, unsigned char* send_buf);
typedef union
{
	unsigned char byte[10];
	struct
	{
		unsigned char Begin;     //头
		unsigned char Data[8];   //数据
		unsigned char CRC;       //校验
	};
}UART_BUF;


#define PASSWORD 0x33
#define UNLOCK   0x5B
#define MODE_SLEEP 0x40
#define MODE_HANDSHAKE 0x56


#endif