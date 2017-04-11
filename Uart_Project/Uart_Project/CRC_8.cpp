/********************************************************************
文件名称          ：CRC_8.cpp
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
//#include "system.h"
#include "CRC_8.h"
#include "stdio.h"
#include "uart_send.h"
#include "stdafx.h"
#include "stdlib.h"
#include <time.h>
#include <sys/timeb.h>
#include <windows.h>
#include <iostream> 
#include "SerialPort.h"
//半字节的CRC余式表
 const static unsigned char CrcTab[16]={0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,
                                0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D};
                                
/*===================================================================
函数功能          :对单字节进行8位CRC校验，校验多項式为X8＋X2＋X1＋1   
 
输入              :data_in，待校验字节  
                   crc，上一字节的校验结果  
                   
输出              :函数返回对本字节的CRC校验结果
                   
创建人            :Shannon Lee    
创建日期          :2011年2月28日     
历史修改记录      :     
                   Shannon Lee  2011.02.28 模块创建
其它说明          :     
===================================================================*/
unsigned char GetRecCRC(unsigned char data_in,unsigned char crc)
{
	unsigned char harf_data;
	
	//对上半字节校验
	harf_data=crc>>4;
	crc<<=4;
	crc^=CrcTab[harf_data^(data_in>>4)];
	
	//对下半字节校验
    harf_data=crc>>4;
	crc<<=4;
    crc^=CrcTab[harf_data^(data_in&0x0f)]; 
    
	return crc;                   //返回本字节的CRC码
}

bool CRC_Check(unsigned char* recvbuf, unsigned char* sendbuf)
{
	unsigned char crc=0;
	unsigned char i = 3;
	if (recvbuf[0] == 0x5A)
	{
		sendbuf[0] = 0x5A;
		sendbuf[1] = 0x33;
		sendbuf[2] = 0x00;
		for(i=3;i<8;i++)
		{ 
			crc = GetRecCRC(crc, recvbuf[i]);
			sendbuf[i] = crc;
		}
		return true;
	}
	else
	{
		return false;
	}
}


