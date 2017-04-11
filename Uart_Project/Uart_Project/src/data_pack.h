#ifndef __DATA_PACK_H__
#define __DATA_PACK_H__

typedef union
{
    unsigned int word;
    unsigned char byte[4];
    struct{
		unsigned MOT1:1; //电机1~8
		unsigned MOT2:1;
		unsigned MOT3:1; 
		unsigned MOT4:1; 
		unsigned MOT5:1;
		unsigned MOT6:1; 
		unsigned MOT7:1; 
		unsigned MOT8:1; 
		unsigned PHOTO:1;  //拍照模块
		unsigned FACE:1;    //人脸识别
		unsigned SW_GET:1;   //超声波人体识别
		unsigned NAV:1;  //导航
		unsigned BTC:1;  //蓝牙遥控
		unsigned LED_DIY:1;  //LED DIY
		unsigned IMU:1;   //IMU
		unsigned SW_RUN:1; //超声波工作
        unsigned LOWPOWER:1; //低电量
		unsigned WiFi:1;  //WiFi连接
		unsigned LKB:1;     //本地知识库
		unsigned LISTEN:1; //语音监听
		unsigned WAIT_A:1; //等待回答
		unsigned VIP_GET:1; //vip识别
		unsigned :1;
		unsigned :1; 
        unsigned :8;
	};		
}SYSTEM_STATUS;

typedef union
{
    unsigned short int word;
    unsigned char byte[2];
    struct
    {
        unsigned BAT:1; //电池故障
		unsigned CHARGE:1;  //充电故障
		unsigned CHASSIS_MOT:1; //底盘电机故障
		unsigned TORSO_MOT:1;   //躯干电机故障
		unsigned NAV:1;        //导航模块故障
		unsigned IMU:1;        //传感器故障
		unsigned SW:1;         //超声波故障
		unsigned CAN4:1;       //底盘can通讯异常
		unsigned CAN3:1;       //胸部can通讯异常
		unsigned CAN8:1;       //LEDcan通讯异常
		unsigned MASTER_POWER:1;   //超声波人体识别
		unsigned PAD_CHARGE:1;  //平板充电电路异常
		unsigned SER_CON:1;       //服务器连接异常
		unsigned CAMERA:1;       //摄像头打开异常
		unsigned FACE:1;         //人脸识别异常
		unsigned LKB:1;         //本地知识库异常
    }；
}SYSTEM_ERROR;

typedef enum
{
    SLEEP=0x40,
    IDLE,
    DANCE,
    SINGING,
    FOLLOW,
    VOICE_CMD,
    VOICE_TALK,
    BT_CONTROL,
    FIRMWARE_UPDATE,
    NAVIGATION,
    CHARGE,
    SHUTDOWN,
    SHAKE_HAND=0x56
}WORK_MODE;

typedef enum
{
   LEFT=0x1,//左转
   RIGHT, //右转
   ROUND, //转圈
   COMEON, //过来
   LTURN,  //再转一点
   JINGLI, //敬礼
   MAIMENG, //卖萌
   SHAKEH, //握手
   FEIWEN, //飞吻
}VOICE_SUBCMD

typedef enum
{
	ACTION_ACCEPT=0x01,
	ACTION_REFUSE=0x02
}CAN_RESP;

typedef enum
{
	NAVIGATION_ONGOING=0x01,
	NAVIGATION_SUCCEED,
	NAVIGATION_FAIL,
}NAVIGATION_STATUS;

/*
第一类：
可以了：{"from":"voice","cmd":IDLE,"subcmd":} //发给canserver
        {"from":"can","cmd":IDLE,"subcmd":,"resp":ACTION_ACCEPT} //发给UI和voice

第二类：
语音指令：{"from":"voice","cmd":VOICE_CMD,"subcmd":LEFT} //发给canserver
		  {"from":"can","cmd":VOICE_CMD,"subcmd":LEFT,"resp":ACTION_ACCEPT} //发给voice
第三类：
导航： {"from":"voice","cmd":NAVIGATION,"subcmd":<goalid>} //语音控制开始导航，发给canserver
       {"from":"can","cmd":NAVIGATION,"subcmd":<goalid>,"resp":ACTION_ACCEPT} //可以导航 或者比可以导航，发给voice
	   {"from":"can","cmd":NAVIGATION,"subcmd":<goalid>,"resp":NAVIGATION_ONGOING} //报告正在导航状态，成功或者导航失败；发给voice
*/
#define SOFTWAER_VERSION 0001
#define HARDWAER_VERSION 01

typedef union
{
    unsigned short int word;
    unsigned char byte[2];
    struct
    {
        unsigned MOT1_CAN:1; //1号电机CAN通讯故障
		unsigned MOT2_CAN:1;  //2号电机CAN通讯故障
		unsigned MOT3_CAN:1; //3号电机CAN通讯故障
		unsigned MOT4_CAN:1;  //4号电机CAN通讯故障
		unsigned TMP:1;       //温度异常
		unsigned POWER_5V:1;  //5v电源异常
		unsigned POWER_12V:1; //12v电源异常
		unsigned CAN3:1;       //胸部can通讯异常
		unsigned KEY:1;       //触摸开关
		unsigned :1;       
		unsigned :1;  
		unsigned :1;  
		unsigned :1;       
		unsigned :1;       
		unsigned :1;         
		unsigned :1;         
    }；
}TORSO_STATUS;

typedef union
{
    unsigned short int word;
    unsigned char byte[2];
    struct
    {
        unsigned MOT_CAN:1;        //电机通讯故障
		unsigned MOT2:1;  //电机2故障
		unsigned MOT1:1;           //电机2故障
		unsigned BAT:1;  //电池故障
		unsigned ELEC:1;       //电量检测故障
		unsigned CHARGE:1;  //充电检测
		unsigned CAN4:1; //主线CAN通讯故障
		unsigned TMP:1;       //温度异常
		unsigned POWER_5V:1;       //5v电源异常
		unsigned POWER_12V:1;       //12v电源异常
		unsigned RUN_SW:1;       //运动时超声波检测到信号
		unsigned SLEEP_SW:1; //休眠时超声波检测到信号 
		unsigned WALK_SW:1;       //前进时超声波检测到信号 
		unsigned DANCN_SW:1;         //跳舞时超声波检测到信号
		unsigned LEFT_LIGHT:1;           //左灯通讯故障
		unsigned RIGHT_LIGHT:1;         //右灯通讯故障
    }；
}UNDER_CONTROL_STATUS;

typedef union
{
    unsigned short int word;
    unsigned char byte[2];
    struct
    {
        unsigned RADER_COM:1;        //雷达通讯异常
		unsigned NAV:1;            //导航规划失败
		unsigned FACE:1;           //人脸识别异常
		unsigned UART:1;  //导航和底盘uart通讯异常
		unsigned WiFi:1;       //WiFi信号异常
		unsigned :1;  //
		unsigned :1; //
		unsigned CAN2:1;       //主线CAN通讯异常
		unsigned :1;       
		unsigned :1;       
		unsigned :1;       
		unsigned :1; 
		unsigned :1;       
		unsigned :1;         
		unsigned :1;           
		unsigned :1;         
    }；
}NAV_CONTROL_STATUS;

typedef union
{
    unsigned short int word;
    unsigned char byte[2];
    struct
    {
        unsigned OC:1;        //硬件过流
		unsigned POWER_DOWN:1;  //掉电
		unsigned UV:1;           //欠压
		unsigned STOP_OV:1;  //停止过流
		unsigned RUN_OV:1;       //运行过流
		unsigned EM_ERR:1;  //电磁铁故障
		unsigned OC_ERR:1; //电路回路故障
		unsigned STALL:1;       //电机堵转
		unsigned BC:1;       //制动回路故障
		unsigned CAN:1;       //通讯故障
		unsigned KEY:1;       //光电开关故障
		unsigned INIT_ANGLE:1; //初始化角度故障 
		unsigned HILL:1;       //霍尔故障 
		unsigned OS:1;         //超速故障
		unsigned :1;           //反转故障
		unsigned OL:1;         //过载故障
    }；
}MOTOR_CONTROL_STATUS;




#endif
