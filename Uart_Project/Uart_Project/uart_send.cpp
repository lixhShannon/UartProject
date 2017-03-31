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
unsigned char RecvBuf[10];

void sendData(unsigned char* buf,unsigned char CANID)
{
    unsigned char i;
    unsigned char crc=0xAA;
    buf[0] = 0xAA;
    if(CANID<0x0B)
    {
        buf[1] = CANID;
    }
    else
    {
        printf("error CAN ID");
        return;
    }
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
        sendData(buf,0x02);
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
        sendData(buf, 0x04);
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
        sendData(buf, 0x03);
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
        sendData(buf, 0x08);
        uart_phase = 1;
        break;
        case 4:
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
			RecvBuf[0] = UART0ReadData;
			UART0RXCnt = 1;
			UART0RXCRC = 0xBB;
		}
		break;
	case UART0_Data_Rcv:
		if (UART0RXCnt<8)
		{
			RecvBuf[UART0RXCnt] = UART0ReadData;
			UART0RXCRC ^= UART0ReadData;
			UART0RXCnt++;
		}
		else if (UART0RXCnt == 8)
		{
			RecvBuf[UART0RXCnt] = UART0ReadData;
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
			RecvBuf[9] = UART0ReadData;
			if (RecvBuf[1] == 4)
			{
				count4++;
				//std::cout << "recv to 204 " << count4++ << std::endl;
			}
			else if (RecvBuf[1] == 3)
			{
				count5++;
				//std::cout << "recv to 203 " << count5++ << std::endl;
			}
			else if (RecvBuf[1] == 8)
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