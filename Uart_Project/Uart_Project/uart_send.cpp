/********************************************************************
文件名称          ：uart_send
项目代号或名称     ：
功能描述          ：
目标芯片          ：
创建人            ：Shannon Li
创建日期          ：
历史修改记录      ：
                    SL    2017-03-24    v1.0   模块创建
版本信息          ：v1.0
其他说明          ：
********************************************************************/
#include "stdio.h"
#include "uart_send.h"
#include "stdafx.h"
#include "stdlib.h"
#include <time.h>
#include <sys/timeb.h>
#include <windows.h>
#include <iostream> 
#include "SerialPort.h"
#include "CRC_8.h"

#define UART0_Idle     0			
#define UART0_Data_Rcv 1			
#define UART0_CRC      2	

unsigned char uart_phase=1;  //开关量
unsigned char Uart0RXErrCnt = 0;
unsigned char UART0RXPhase = 0;
unsigned char UART0RXCnt = 0;
unsigned char UART0RXCRC = 0;
unsigned int count1 = 0;
unsigned int count2 = 0;
unsigned int count3 = 0;
unsigned int count4 = 0;
unsigned int count5 = 0;
unsigned int count6 = 0;
unsigned char RecvBuf[8];
unsigned char SendBuf[8];
unsigned char Uart_Recv_Buf[10];
unsigned char Uart_Send_Buf[10];
bool Flag_Uart_PassWord_Send=false;
extern CSerialPort mySerialPort;
extern void Uart_Handle(unsigned char* recv_buf, unsigned char* send_buf);

void sendDataPack(unsigned char* buf)
{
    unsigned char i;
    unsigned char crc=0xAA;
    buf[0] = 0xAA;
    for(i=1;i<9;i++)
    {
        crc ^= buf[i];
    }
    buf[9] = crc;
}

void uart_send(unsigned char* buf)
{
    switch(uart_phase)
    {
        case 0:  //给工控机
	    buf[1] = 0x02;
		sendDataPack(buf);
        uart_phase = 1;
        break;
        case 1:  //给底盘板
			count1++;
			if ((count1 % 100)==0)
			{
				std::cout << "send to 204 " << count1 << std::endl;
				std::cout << "recv to 204 " << count4 << std::endl;
			}
	    //
		buf[1] = 0x04;
		sendDataPack(buf);
        uart_phase = 2;
        break;
        case 2:  //胸部板
			count2++;
			if ((count2 % 100)==0)
			{
				std::cout << "send to 203 " << count2 << std::endl;
				std::cout << "recv to 203 " << count5 << std::endl;
			}
		//std::cout << "send to 203 " << count2 << std::endl;
		buf[1] = 0x03;
		sendDataPack(buf);
        uart_phase = 3;
        break;
        case 3:  //头部板
			count3++;
			if ((count3 % 100)==0)
			{
				std::cout << "send to 208 " << count3 << std::endl;
				std::cout << "recv to 208 " << count6 << std::endl;
			}
		//std::cout << "send to 208 " << count3 << std::endl;
		sendDataPack(buf);
        uart_phase = 1;
        break;
        case 4:
		buf[1] = 0x08;
		sendDataPack(buf);
        uart_phase = 0;
        break;
		case 5:
		uart_phase = 0;
		break;
        default:
        uart_phase = 0;
        break;
    }
}

void UART0Read(unsigned char UART0ReadData)
{
	Uart0RXErrCnt = 0;
	switch (UART0RXPhase)
	{
	case UART0_Idle:
		if (UART0ReadData == 0xBB)
		{
			//SL_Flag_Uart0RxErr = 1;
			UART0RXPhase = UART0_Data_Rcv;
			Uart_Recv_Buf[0] = UART0ReadData;
			UART0RXCnt = 1;
			UART0RXCRC = 0xBB;
		}
		break;
	case UART0_Data_Rcv:
		if (UART0RXCnt<8)
		{
			Uart_Recv_Buf[UART0RXCnt] = UART0ReadData;
			UART0RXCRC ^= UART0ReadData;
			UART0RXCnt++;
		}
		else if (UART0RXCnt == 8)
		{
			Uart_Recv_Buf[UART0RXCnt] = UART0ReadData;
			UART0RXCRC ^= UART0ReadData;
			UART0RXCnt = 0;
			UART0RXPhase = UART0_CRC;
		}
		else
		{
			UART0RXCnt = 0;
			UART0RXPhase = UART0_Idle;
		}
		break;
	case UART0_CRC:
		if (UART0ReadData == UART0RXCRC)
		{
			Uart_Recv_Buf[9] = UART0ReadData;
			for (int i = 0; i < 8; i++)
			{
				RecvBuf[i] = Uart_Recv_Buf[i + 1];
			}
			Uart_Handle(RecvBuf, SendBuf);
			if (RecvBuf[0] == 4)
			{
				count4++;
				//std::cout << "recv to 204 " << count4++ << std::endl;
			}
			else if (RecvBuf[0] == 3)
			{
				count5++;
				//std::cout << "recv to 203 " << count5++ << std::endl;
			}
			else if (RecvBuf[0] == 8)
			{
				count6++;
				//std::cout << "recv to 208 " << count6++ << std::endl;
			}
		}
		else
		{
			UART0RXCnt = 0;
		}
		UART0RXPhase = UART0_Idle;
		break;
	default:
		UART0RXPhase = UART0_Idle;
		break;
	}
}

void Uart_Handle(unsigned char* recv_buf, unsigned char* send_buf)
{
	unsigned char cmd;
	cmd = recv_buf[1];
	unsigned char crc = 0;
	switch (cmd)
	{
	case 0x33:
		if (CRC_Check(recv_buf, send_buf))
		{
			Flag_Uart_PassWord_Send = true;
		}
		break;
	case 0x40:
		break;
	case 0x56:
		break;
	default:
		break;
	}
}