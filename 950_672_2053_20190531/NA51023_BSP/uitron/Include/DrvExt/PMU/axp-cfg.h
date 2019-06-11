#ifndef __LINUX_AXP_CFG_H_
#define __LINUX_AXP_CFG_H_

/*
添加平台相关的头文件
			*/

#if defined(__ECOS)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Type.h"
#include "i2c.h"//#include "stm32f10x.h"
#include "axp-mfd-216.h"


/*设备地址*/
/*
	一般不改变：
	AXP216: 写地址 ：0x68	读地址：0x69
*/
#define	AXP_DEVICES_ADDR	0x68



/*AXP216 读函数	*/
uint8_t AXP216_read( uint8_t readAddr, uint8_t byteNum , uint8_t *readData);
/*AXP216 写函数	*/
uint8_t AXP216_write(const uint8_t writeAddr, const uint8_t writeData);
/*AXP216 延时函数*/
void delay_ms(uint16_t nms);
/*AXP216 位置1函数*/
void axp_set_bits(int reg, uint8_t bit_mask);
/*AXP216 位清0函数*/
void  axp_clr_bits(int reg, uint8_t bit_mask);
/*AXP216 跟*/
void  axp_update(int reg, uint8_t val, uint8_t mask);



/*debug功能开关：1:打开 0 ：关闭*/
#define axp_debug		1//0



/*电池容量，mAh：根据实际电池容量来定义，对库仑计方法来说
这个参数很重要，必须配置*/
#define BATCAP				380//460//549//470//3000

/*初始化电池内阻，mΩ：一般在100~200之间，不过最好根据实际
测试出来的确定，方法是打开打印信息，不接电池烧好固件后，
上电池，不接充电器，开机，开机1分钟后，接上充电器，充
1~2分钟，看打印信息中的rdc值，填入这里*/
#define BATRDC				257//220//203//100

/*开路电压方法中的电池电压的缓存*/
//#define AXP_VOL_MAX			1
/*
	充电功能使能：
        AXP:0-关闭，1-打开
*/
#define CHGEN       1

/*
	充电电流设置，uA，0为关闭：
		AXP:300~2550,100/step
*/
/*充电电流，uA*/
#define STACHGCUR			300*1000//900*1000


/*目标充电电压，mV*/
/*
	AXP:4100000/4200000/4240000/4350000
*/
#define CHGVOL				4100000//4200000

/*充电电流小于设置电流的ENDCHGRATE%时，停止充电，%*/
/*
	AXP:10\15
*/
#define ENDCHGRATE			10
/*关机电压，mV*/
/*
	系统设计的关机过后的电池端电压，需要与关机百分比、
	开路电压对应百分比表及低电警告电压相互配合才会有作用
*/
#define SHUTDOWNVOL			3300

/*adc采样率设置，Hz*/
/*
	AXP:100\200\400\800
*/
#define ADCFREQ				100

/*预充电超时时间，min*/
/*
	AXP:40\50\60\70
*/
#define CHGPRETIME			50
/*恒流充电超时时间，min*/
/*
	AXP:360\480\600\720
*/
#define CHGCSTTIME			480


/*pek开机时间，ms*/
/*
	按power键硬件开机时间：
		AXP:128/1000/2000/3000
*/
#define PEKOPEN				1000
/*pek长按时间，ms*/
/*
	按power键发长按中断的时间，短于此时间是短按，发短按键irq，
	长于此时间是长按，发长按键irq：
		AXP:1000/1500/2000/2500
*/
#define PEKLONG				1500
/*pek长按关机使能*/
/*
	按power键超过关机时长硬件关机功能使能：
		AXP:0-不关，1-关机
*/
#define PEKOFFEN			1
/*pek长按关机使能后开机选择*/
/*
	按power键超过关机时长硬件关机还是重启选择:
		AXP:0-只关机不重启，1-关机后重启
*/
#define PEKOFFRESTART			0
/*pekpwr延迟时间，ms*/
/*
	开机后powerok信号的延迟时间：
		AXP20:8/16/32/64
*/
#define PEKDELAY			32
/*pek长按关机时间，ms*/
/*
	按power键的关机时长：
		AXP:4000/6000/8000/10000
*/
#define PEKOFF				6000
/*过温关机使能*/
/*
	AXP内部温度过高硬件关机功能使能：
		AXP:0-不关，1-关机
*/
#define OTPOFFEN			1//0
/* 充电电压限制使能*/
/*
	AXP:0-关闭，1-打开
*/
#define USBVOLLIMEN		0// 1
/*  充电限压，mV，0为不限制*/
/*
	AXP:4000~4700，100/step
*/
#define USBVOLLIM			4500
/*  USB充电限压，mV，0为不限制*/
/*
	AXP:4000~4700，100/step
*/
//#define USBVOLLIMPC			4700

/* 充电电流限制使能*/
/*
	AXP:0-关闭，1-打开
*/
#define USBCURLIMEN		1
/* 充电限流，mA，0为不限制*/
/*
	AXP:500/900
*/
#define USBCURLIM			0
/* usb 充电限流，mA，0为不限制*/
/*
	AXP:500/900
*/
#define USBCURLIMPC			500
/* PMU 中断触发唤醒使能*/
/*
	AXP:0-不唤醒，1-唤醒
*/
#define IRQWAKEUP			1//0
/* N_VBUSEN PIN 功能控制*/
/*
	AXP:0-输出，驱动OTG升压模块，1-输入，控制VBUS通路
*/
#define VBUSEN			1
/* ACIN/VBUS In-short 功能设置*/
/*
	AXP:0-AC VBUS分开，1-使用VBUS当AC,无单独AC
*/
#define VBUSACINSHORT			0
/* CHGLED 管脚控制设置*/
/*
	AXP:0-驱动马达，1-由充电功能控制
*/
#define CHGLEDFUN			0
/* CHGLED LED 类型设置*/
/*
	AXP:0-充电时led长亮，1-充电时led闪烁
*/
#define CHGLEDTYPE			0
/* 电池总容量校正使能*/
/*
	AXP:0-关闭，1-打开
*/
#define BATCAPCORRENT			1
/* 充电完成后，充电输出使能*/
/*
	AXP:0-关闭，1-打开
*/
#define BATREGUEN			0
/* 电池检测功能使能*/
/*
	AXP:0-关闭，1-打开
*/
#define BATDET		1
/* PMU重置使能*/
/*
	AXP:0-关闭，1-打开按电源键16秒重置PMU功能
*/
#define PMURESET		0
/*低电警告电压1，%*/
/*
	根据系统设计来定：
	AXP:5%~20%
*/
#define BATLOWLV1    15
/*低电警告电压2，%*/
/*
	根据系统设计来定：
	AXP:0%~15%
*/
#define BATLOWLV2    0

#define ABS(x)				((x) >0 ? (x) : -(x) )


/*初始化开路电压对应百分比表*/
/*
	可以使用默认值，但是最好根据实际测试的电池来确定每级
	对应的剩余百分比，特别需要注意，关机电压SHUTDOWNVOL和电池
	容量开始校准剩余容量百分比BATCAPCORRATE这两级的准确性
	AXP适用
*/
#define OCVREG0				0		 //3.13V
#define OCVREG1				0		 //3.27V
#define OCVREG2				0		 //3.34V
#define OCVREG3				0		 //3.41V
#define OCVREG4				0		 //3.48V
#define OCVREG5				0		 //3.52V
#define OCVREG6				0		 //3.55V
#define OCVREG7				0// 1		 //3.57V
#define OCVREG8				0// 1		 //3.59V
#define OCVREG9				0// 2		 //3.61V
#define OCVREGA				0// 3		 //3.63V
#define OCVREGB				0// 4		 //3.64V
#define OCVREGC				0//5		 //3.66V
#define OCVREGD				5//14		 //3.7V
#define OCVREGE				11//24		 //3.73V
#define OCVREGF				24//41		 //3.77V
#define OCVREG10		 	28//45                //3.78V
#define OCVREG11		 	32//51                //3.8V
#define OCVREG12		 	38//55                //3.82V
#define OCVREG13		 	42//59                //3.84V
#define OCVREG14		 	48//61                //3.85V
#define OCVREG15		 	55//65                //3.87V
#define OCVREG16		 	65//71                //3.91V
#define OCVREG17		 	78                //3.94V
#define OCVREG18		 	86                //3.98V
#define OCVREG19		 	94                //4.01V
#define OCVREG1A		 	100                //4.05V
#define OCVREG1B		 	100                //4.08V
#define OCVREG1C		 	100//94                //4.1V
#define OCVREG1D		 	100//96                //4.12V
#define OCVREG1E		 	100//97                //4.14V
#define OCVREG1F		 	100                //4.15V



/*选择需要打开的中断使能*/
static const uint64_t AXP_NOTIFIER_ON = (AXP_IRQ_USBIN |AXP_IRQ_USBRE |AXP_IRQ_USBOV|
				       		             AXP_IRQ_ACIN  |AXP_IRQ_ACRE  |AXP_IRQ_ACOV	|
				       		             AXP_IRQ_BATIN |AXP_IRQ_BATRE |
				       		             AXP_IRQ_CHAST |AXP_IRQ_CHAOV |AXP_IRQ_POKSH|
										 AXP_IRQ_POKLO | AXP_IRQ_POKSH|
						                (uint64_t)AXP_IRQ_PEKFE |(uint64_t)AXP_IRQ_PEKRE);

/* 需要做插入火牛、usb关机重启进boot时power_start设置为1，否则为0*/
//#define POWER_START 0

typedef enum
{
    POWERIC_I2C_REGISTER_1BYTE = 1,
    POWERIC_I2C_REGISTER_2BYTE = 2,
    ENUM_DUMMY4WORD(POWERIC_I2C_REGISTER_BYTES)
} POWERIC_I2C_REGISTER_BYTES, *PPOWERIC_I2C_REGISTER_BYTES;

typedef struct
{
	I2C_SESSION I2C_Channel;
	POWERIC_I2C_REGISTER_BYTES	I2C_RegBytes;
	I2C_PINMUX		I2C_PinMux;
	I2C_BUS_CLOCK	I2C_BusClock;
	UINT32			I2C_Slave_WAddr;
	UINT32			I2C_Slave_RAddr;
} POWERIC_INFO,*PPOWERIC_INFO;

extern BOOL PowerIC_I2C_Init(POWERIC_INFO PowerICInfo);
extern void PowerIC_I2C_Unlock(void);
#endif

#endif
