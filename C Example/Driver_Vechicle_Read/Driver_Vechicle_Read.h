#ifndef __Driver_Vechicle_Read_H
#define __Driver_Vechicle_Read_H
//整车状态
#define VCUVehSts_0_Standstill	0//"0x0:静止
#define VCUVehSts_1_EVDrive   	1//0x1:电机驱动
#define VCUVehSts_2_Regeneration 2	//0x2:能量回收（将制动、滑行能量回收合并为能量回收）
#define VCUVehSts_3_Charge		3//0x3: 外插充电
#define VCUVehSts_4_Failure		4//0x4: 故障状态0x5~7: 预留"	S	R
//BMS工作模式请求														
#define VCUBMSOperModReq_0_Initializing	0
#define VCUBMSOperModReq_1_Standby		1
#define VCUBMSOperModReq_2_PreCharge	2
#define VCUBMSOperModReq_3_HVActive		3
#define VCUBMSOperModReq_4_Powerdown 	4
#define VCUBMSOperModReq_5_Failure		5											

//这个结构体是对VCU 发送给 BMS的数据进行缓存
typedef struct
{
	INT8U VCUActHVPwrCnsExt;//外部消耗高压功率
	INT8U VCUSOCDisp;		  //电池SOC快充显示
	INT8U VCUVehSts;//整车状态
	INT8U VCUBMSOperModReq;//BMS工作模式请求
	BOOL VCUChrgAllwd;//整车充电请求
	BOOL VCUBattIntrHeatAllwd;//电池内部加热/冷却许可
	BOOL SRSCrashSts;//SRS碰撞状态
	BOOL VCUBMSSOCEna;//电池剩余SOC触发
	BOOL CHMAsignalSts;//A+信号状态
	INT8U VCUChrgIReqLimFac;//充电需求电流限制系数
	INT8U VCUSRSCrashOutpSts;// 碰撞输出信号 可以更改为bool类型
	INT8U Receied_VCU;//收到VCU的第一帧信息
//VCUSRSCrashOutpStsChkS;
//VCUChrgSspdReq  充电暂停请求信号（预留）
//MilgChrgMode  里程充电模式（预留）
}VCU_BMS_Struct;

typedef struct
{
	//0x311
	INT16U OBCMaxChrgOutpUAvl;//充电机最大允许输出电压   放大10倍
	INT16U OBCMaxChrgOutpIAvl;//充电机最大允许输出电流   放大10倍
	INT16U OBCMaxChrgOutpPwrAvl;//充电机最大允许输出功率
	INT8S OBCVehInletT;//交流充电插座温度
	INT8U OBCChrgSts;//充电机工作状态
	INT8U PlugLockStatus;//充电枪锁止状态
	BOOL OBCS2Stus;//S2状态

	//0x321
	INT8U OBCCPValue;//CP占空比
	BOOL CPSts;//CP状态

	//0x331
	INT16U OBCChrgInpAcU;//充电机交流端实时输入电压
	INT16U OBCChrgInpAcI;//充电机交流端实时输入电流   放大10倍
	INT16U OBCChrgDcU;//充电机实时充电电压    放大10倍
	INT16U OBCChrgDcI;//充电机实时充电电流   放大10倍
	INT8S OBCT;//充电机温度
	BOOL OBCCANErr;//OBC通信故障
	BOOL OBCHwErr;//OBC硬件故障
	BOOL OBCInpUOver;//OBC输入过压
	BOOL OBCOutpUOver;//OBC输出过压
	BOOL OBCInpUUnder;//OBC输入欠压
	BOOL OBCOutpUUnder;//OBC输出欠压
	BOOL OBCInpIOver;//输入过流
	BOOL OBCOutpIOver;//输出过流
	BOOL OBCTOvrErr;//过温故障
}OBC_BMS_Struct;

extern VCU_BMS_Struct VCU_BMS_Data; //VCU向BMS发送的数据解析
extern OBC_BMS_Struct OBC_BMS_Data; //OBC向BMS发送的数据解析
extern void VCU_OBC_BMS_communicate_error(void);

#endif
