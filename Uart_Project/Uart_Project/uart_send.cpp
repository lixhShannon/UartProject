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
#include "data_pack.h"

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

SYSTEM_STATUS systemStatus;
SYSTEM_ERROR systemError;
WORK_MODE workMode = SHAKE_HAND;
VOICE_SUBCMD voiceSubCMD;
CAN_RESP CANResp;
NAVIGATION_STATUS navigationStatus;
TORSO_STATUS torsoStatus;
UNDER_CONTROL_STATUS underControlStatus;
NAV_CONTROL_STATUS navControlStatus;
MOTOR_CONTROL_STATUS motorControlStatus;
LED_STATUS LEDStatus;
unsigned char Param = 0;
unsigned char Electricity = 0;
unsigned char SHAKE_HAND_Status = 0;
VERSION underBoard;
VERSION LEDBoard;
VERSION NavBoard;
VERSION TorsoBoard[5];

extern void Uart_Handle(unsigned char* recv_buf, unsigned char* send_buf);
extern void DataProcess(void);

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
		buf[1] = 0x08;		
        uart_phase = 1;
        break;
        case 4:
        uart_phase = 0;
        break;
		case 5:
		uart_phase = 0;
		break;
        default:
        uart_phase = 0;
        break;
    }
	buf[2] = workMode;
	buf[3] = Param;
	if (workMode == SHAKE_HAND)
	{
		buf[4] = SOFTWAER_VERSION & 0xFF;
		buf[5] = SOFTWAER_VERSION >> 8;
		buf[6] = HARDWAER_VERSION;
	}
	else
	{
		buf[4] = systemStatus.byte[0];
		buf[5] = systemStatus.byte[1];
		buf[6] = systemStatus.byte[2];
	}
	buf[7] = systemError.byte[0];
	buf[8] = systemError.byte[1];
	sendDataPack(buf);
}

void UART0Read(unsigned char UART0ReadData)
{
	Uart0RXErrCnt = 0;
	switch (UART0RXPhase)
	{
	case UART0_Idle:
		if (UART0ReadData == 0xBB)
		{
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
			DataProcess();
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
	default:
		if (RecvBuf[0] == 4)
		{
			count4++;
			Electricity = RecvBuf[5];
			underControlStatus.byte[0] = RecvBuf[6];
			underControlStatus.byte[1] = RecvBuf[7];
			//std::cout << "recv to 204 " << count4++ << std::endl;
		}
		else if (RecvBuf[0] == 3)
		{
			count5++;
			torsoStatus.byte[0] = RecvBuf[6];
			torsoStatus.byte[1] = RecvBuf[7];
			//std::cout << "recv to 203 " << count5++ << std::endl;
		}
		else if (RecvBuf[0] == 8)
		{
			count6++;
			LEDStatus.byte[0] = RecvBuf[6];
			LEDStatus.byte[1] = RecvBuf[7];
			//std::cout << "recv to 208 " << count6++ << std::endl;
		}
		else if (RecvBuf[0] == 2)
		{
			navControlStatus.byte[0] = RecvBuf[6];
			navControlStatus.byte[1] = RecvBuf[7];
		}
		switch (cmd)
		{
		case 0x40:
			break;		
		case 0x41:
			break;
		case 0x42:
			break;
		case 0x43:
			break;
		case 0x44:
			break;
		case 0x45:
			break;
		case 0x46:
			break;
		case 0x47:
			break;
		case 0x48:
			break;
		case 0x49:
			break;
		case 0x4A:
			break;
		case 0x4B:
			break;
		case 0x56:		
			if (RecvBuf[0] == 4)
			{
				SHAKE_HAND_Status = SHAKE_HAND_Status | 0x40;
				underBoard.byte[1] = RecvBuf[2];
				underBoard.byte[0] = RecvBuf[3];
				underBoard.hardwareVersion = RecvBuf[4];
			}
			else if (RecvBuf[0] == 3)
			{
				if (RecvBuf[5] < 5)
				{
					SHAKE_HAND_Status = SHAKE_HAND_Status | (1 << RecvBuf[5]);
					TorsoBoard[RecvBuf[5]].byte[1] = RecvBuf[2];
					TorsoBoard[RecvBuf[5]].byte[0] = RecvBuf[3];
					TorsoBoard[RecvBuf[5]].hardwareVersion = RecvBuf[4];
				}
			}
			else if (RecvBuf[0] == 8)
			{
				SHAKE_HAND_Status = SHAKE_HAND_Status | 0x20;
				LEDBoard.byte[1] = RecvBuf[2];
				LEDBoard.byte[0] = RecvBuf[3];
				LEDBoard.hardwareVersion = RecvBuf[4];
			}
			else if (RecvBuf[0] == 2)
			{
				SHAKE_HAND_Status = SHAKE_HAND_Status | 0x80;
				NavBoard.byte[1] = RecvBuf[2];
				NavBoard.byte[0] = RecvBuf[3];
				NavBoard.hardwareVersion = RecvBuf[4];
			}
			break;
		case 0x66:
			break;
		case 0xC5:
			break;
		default:
			break;
		}
		break;
	}
}


void DataProcess(void)
{
	if (workMode == SHAKE_HAND)
	{
		if (SHAKE_HAND_Status == 0xFF)
		{
			workMode = IDLE;
		}
	}
	else
	{
		systemStatus.MOT = torsoStatus.MOT;
		//systemStatus.FACE = 
		//systemStatus.PHOTO =
		systemStatus.SW_GET = underControlStatus.RUN_SW;
		systemStatus.NAV = navControlStatus.NAV;
		systemStatus.BTC = underControlStatus.BAT;
		//systemStatus.LED_DIY = LEDStatus.
		//systemStatus.IMU = underControlStatus.
		systemStatus.SW_RUN = underControlStatus.RUN_SW;
		if (Electricity < 20)
		{
			systemStatus.LOWPOWER = 1;
		}
		else
		{
			systemStatus.LOWPOWER = 0;
		}
		if (0)  //no WiFi
		{
			systemStatus.NOWiFi = 1;
		}
		else
		{
			systemStatus.NOWiFi = 0;
		}
		if (0)  //监听
		{
			systemStatus.LISTEN = 1;
		}
		else
		{
			systemStatus.LISTEN = 0;
		}
		if (0)  //vip
		{
			systemStatus.VIP_GET = 1;
		}
		else
		{
			systemStatus.VIP_GET = 0;
		}
		systemError.BAT = underControlStatus.BAT;
		systemError.CHARGE = underControlStatus.CHARGE;
		if (underControlStatus.MOT1 || underControlStatus.MOT2)
		{
			systemError.CHASSIS_MOT = 1;
		}
		else
		{
			systemError.CHASSIS_MOT = 0;
		}

		if (torsoStatus.byte[0] & 0x0F)
		{
			systemError.TORSO_MOT = 1;
		}
		else
		{
			systemError.TORSO_MOT = 0;
		}
		if (navControlStatus.byte[0])
		{
			systemError.NAV = 1;
		}
		else
		{
			systemError.NAV = 0;
		}

		//systemError.IMU = underControlStatus.IMU;
		//systemError.SW = underControlStatus.
		systemError.CAN4 = underControlStatus.CAN4;
		systemError.CAN3 = torsoStatus.CAN3;
		systemError.CAN8 = LEDStatus.CAN8;
	}
}
