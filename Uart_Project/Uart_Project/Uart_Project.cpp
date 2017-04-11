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

extern void sendDataPack(unsigned char* buf);
extern void uart_send(unsigned char* buf);
extern void UART0Read(unsigned char UART0ReadData);

extern unsigned char Uart_Recv_Buf[10];
extern unsigned char Uart_Send_Buf[10];
extern bool Flag_Uart_PassWord_Send;
extern unsigned char RecvBuf[8];
extern unsigned char SendBuf[8];


VOID CALLBACK myTimerProc1(
	HWND hwnd, // handle of window for timer messages
	UINT uMsg, // WM_TIMER message
	UINT idEvent, // timer identifier
	DWORD dwTime // current system time
) {
	
	//printf("In myTimerProc1\n");
	
}

int main()
{
	MSG msg;
	CSerialPort mySerialPort;
	bool Flag_Uart_Swtich=true;
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
				if (Flag_Uart_PassWord_Send)
				{
					Flag_Uart_PassWord_Send = false;
					for (int i = 0; i < 8; i++)
					{
						Uart_Send_Buf[i + 1] = SendBuf[i];
					}
					sendDataPack(Uart_Send_Buf);
					mySerialPort.WriteData(Uart_Send_Buf, 10);

				}
				else
				{
					Uart_Send_Buf[2] = 0x56;
					uart_send(Uart_Send_Buf);
					mySerialPort.WriteData(Uart_Send_Buf, 10);
				}
				
				//mySerialPort.WriteData(Uart_Send_Buf, 10);
			}
			else
			{
				Flag_Uart_Swtich = true;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	system("pause");
    return 0;
}

