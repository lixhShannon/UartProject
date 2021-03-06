/*========================================================================================

單位名稱:    

項目名稱:    

功能描述:    CRC校驗頭文件

目標芯片:    

創 建 人:    Shannon Lee

創建日期:    2007.11.20

修改紀錄:
            

版本信息:    v1.0

其他說明:

========================================================================================*/
//对传输数据包进行CRC校验函數   
#ifndef __CRC_8_H__
#define __CRC_8_H__

extern unsigned char GetRecCRC(unsigned char data_in,unsigned char crc);
extern bool CRC_Check(unsigned char* recvbuf, unsigned char* sendbuf);

#endif
