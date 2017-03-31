// Uart_Project.cpp : 定义控制台应用程序的入口点。
//

//#include "SL_system.h"
#include "stdio.h"
#include "uart_send.h"
#include "stdafx.h"
#include "stdlib.h"
#include <time.h>
#include <sys/timeb.h>
#include <windows.h>
#include <iostream> 
#include "SerialPort.h"

extern void sendData(unsigned char* buf, unsigned char CANID);
extern void uart_send(unsigned char* buf);
extern void UART0Read(unsigned char UART0ReadData);
bool Flag_uart_read = true;

VOID CALLBACK myTimerProc1(
	HWND hwnd, // handle of window for timer messages
	UINT uMsg, // WM_TIMER message
	UINT idEvent, // timer identifier
	DWORD dwTime // current system time
) {
	
	//printf("In myTimerProc1\n");
	
}
unsigned char SendBuf[10];
//unsigned char buf[10] = {0xBB,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x22 };
int main()
{
	MSG msg;
	CSerialPort mySerialPort;
	bool Flag_Uart_Swtich=true;
	SendBuf[0] = 0xAA;
	SendBuf[2] = 0x56;
	SetTimer(NULL, 0, 50, myTimerProc1);
	if (!mySerialPort.InitPort(2,57600))
	{
		std::cout << "initPort fail !" << std::endl;
	}
	else
	{
		std::cout << "initPort success !" << std::endl;
	}

	if (!mySerialPort.OpenListenThread())
	{
		std::cout << "OpenListenThread fail !" << std::endl;
	}
	else
	{
		std::cout << "OpenListenThread success !" << std::endl;
	}
		
	////SetTimer(NULL, 0, 2000, myTimerProc2);
	BOOL bRet;
	int count = 0;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			if (Flag_Uart_Swtich)
			{
				Flag_Uart_Swtich = false;
				uart_send(SendBuf);
				mySerialPort.WriteData(SendBuf, 10);
			}
			else
			{
				//UINT BytesInQue = mySerialPort.GetBytesInCOM();
				//if (BytesInQue > 0)
				//{
				//	do
				//	{
				//		char cRecved = 0x00;
				//		if (mySerialPort.ReadChar(cRecved) == true)
				//		{
				//			//std::cout << cRecved;  
				//			UART0Read(cRecved);
				//			continue;
				//		}
				//	} while (--BytesInQue);
				//}
				Flag_Uart_Swtich = true;
			}
			//std::cout << "i send the  message " << count++ << std::endl;
			/*uart_send(SendBuf);
			mySerialPort.WriteData(SendBuf, 10);*/
			//UINT BytesInQue = mySerialPort.GetBytesInCOM();
			//if (BytesInQue > 0)
			//{
			//	do
			//	{
			//		char cRecved = 0x00;
			//		if (mySerialPort.ReadChar(cRecved) == true)
			//		{
			//			//std::cout << cRecved;  
			//			UART0Read(cRecved);
			//			continue;
			//		}
			//	} while (--BytesInQue);
			//}
			//std::cout  <<  "i   got   the   message "   <<   std::endl;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			//break;
		}
	}
	system("pause");
    return 0;
}

