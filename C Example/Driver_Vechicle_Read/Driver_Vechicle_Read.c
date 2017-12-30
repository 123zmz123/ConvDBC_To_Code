//这个函数进行整车CAN上的信息解析
#include <includes.h>
#include "SWintegration.h"

VCU_BMS_Struct VCU_BMS_Data; //VCU向BMS发送的数据解析
OBC_BMS_Struct OBC_BMS_Data; //OBC向BMS发送的数据解析

void VCU_BMS_110_Read(INT8U Data[])
{
	VCU_BMS_Data.VCUActHVPwrCnsExt = Data[0];  //总线精度0.1  放大10倍的值
	VCU_BMS_Data.VCUSOCDisp = (INT8U)(Data[1] >> 1);	  //电池SOC快充显示
	VCU_BMS_Data.VCUVehSts = (*(INT16U*)(&Data[1]) & 0x01C0) >> 6;//整车状态
	VCU_BMS_Data.VCUBMSOperModReq = (Data[2] & 0x38) >> 3;//BMS工作模式请求
	
	if((Data[3] & 0x80) > 0)
	{
		VCU_BMS_Data.VCUChrgAllwd = True;//整车充电请求
	}
	else
	{
		VCU_BMS_Data.VCUChrgAllwd = False;
	}

	if((Data[3] & 0x40) > 0)
	{
		VCU_BMS_Data.VCUBattIntrHeatAllwd = True;//电池内部加热/冷却许可
	}
	else
	{
		VCU_BMS_Data.VCUBattIntrHeatAllwd = False;
	}

	if((Data[3] & 0x20) > 0)
	{
		VCU_BMS_Data.SRSCrashSts = True;//SRS碰撞状态
	}
	else
	{
		VCU_BMS_Data.SRSCrashSts = False;		
	}

	if((Data[3] & 0x10) > 0)
	{
		VCU_BMS_Data.VCUBMSSOCEna = True;//电池剩余SOC触发
	}
	else
	{
		VCU_BMS_Data.VCUBMSSOCEna = False;		
	}

	if((Data[3] & 0x08) > 0)
	{
		VCU_BMS_Data.CHMAsignalSts = True;//A+信号状态
	}
	else
	{
		VCU_BMS_Data.CHMAsignalSts = False;		
	}

	VCU_BMS_Data.VCUChrgIReqLimFac = (Data[3] & 0x06) >> 1;//充电需求电流限制系数
	VCU_BMS_Data.VCUSRSCrashOutpSts = (Data[4] & 0xF0) >> 4;// 碰撞输出信号	

	/*
	if((VCU_BMS_Data.VCUSRSCrashOutpSts > 0) || (VCU_BMS_Data.SRSCrashSts > 0))
	{
		Myself.ST_Break = 1;
		Enable_warring(DTC_001_CrashHappen);
		OSSemPost(Module_sem);
	}
	*/
	Myself.VCU_BMS_110_Timeout = 0;
	//Data[4]的后四位是碰撞校验信号 需要补充
}

void OBC_BMS_311_Read(INT8U Data[])
{
	OBC_BMS_Data.OBCMaxChrgOutpUAvl = (*(INT16U*)(&Data[0]) & 0xFFF8) >> 3;//充电机最大允许输出电压  放大10倍
	OBC_BMS_Data.OBCMaxChrgOutpIAvl = (*(INT16U*)(&Data[1]) & 0x07FE) >> 1;//充电机最大允许输出电流  放大10倍
	OBC_BMS_Data.OBCMaxChrgOutpPwrAvl = ((*(INT32U*)(&Data[2]) & 0x01FF8000) >> 15) * 10;//充电机最大允许输出功率
	OBC_BMS_Data.OBCVehInletT = ((Data[4] & 0x7F) << 1) + ((Data[5] & 0x80) >> 7) - 40;//交流充电插座温度
	OBC_BMS_Data.OBCChrgSts = (Data[6] & 0x3C) >> 2;//充电机工作状态
	OBC_BMS_Data.PlugLockStatus = (Data[6] & 0x03);//充电枪锁止状态
	OBC_BMS_Data.OBCS2Stus = (Data[7] & 0x80) >> 7;//S2状态
	Myself.OBC_BMS_311_Timeout = 0;
}

void OBC_BMS_321_Read(INT8U Data[])
{
	OBC_BMS_Data.OBCCPValue = Data[0];//CP占空比
	OBC_BMS_Data.CPSts = (BOOL)((Data[1] & 0x80) >> 7);//CP状态
	Myself.OBC_BMS_321_Timeout = 0;
}

void OBC_BMS_331_Read(INT8U Data[])
{
	INT16U temp;
	OBC_BMS_Data.OBCChrgInpAcU = (INT16U)((*(INT16U*)(&Data[0]) & 0xFF80) >> 7);//充电机交流端实时输入电压
	OBC_BMS_Data.OBCChrgInpAcI = (INT16U)((*(INT16U*)(&Data[1]) & 0x7FE0) >> 5);//充电机交流端实时输入电流    放大10倍
	OBC_BMS_Data.OBCChrgDcU = (INT16U)(*(INT16U*)(&Data[2]) & 0x1FFF);//充电机实时充电电压   放大10倍
	OBC_BMS_Data.OBCChrgDcI = (INT16U)((*(INT16U*)(&Data[4]) & 0xFFC0) >> 6);//充电机实时充电电流  放大10倍

	temp = (INT16U)((*(INT16U*)(&Data[5]) & 0x3FC0) >> 6);
	OBC_BMS_Data.OBCT = (INT8S)(temp - 40);
	
	//C_BMS_Data.OBCT = ((Data[5] & 0x3F) << 2) + ((Data[6] & 0xC0) >> 6) - 40;//充电机温度
	
	OBC_BMS_Data.OBCCANErr = (BOOL)((Data[6] & 0x20) >> 5);//OBC通信故障
	OBC_BMS_Data.OBCHwErr = (BOOL)((Data[6] & 0x10) >> 4);//OBC硬件故障
	OBC_BMS_Data.OBCInpUOver = (BOOL)((Data[6] & 0x08) >> 3);//OBC输入过压
	OBC_BMS_Data.OBCOutpUOver = (BOOL)((Data[6] & 0x04) >> 2);//OBC输出过压
	OBC_BMS_Data.OBCInpUUnder = (BOOL)((Data[6] & 0x02) >> 1);//OBC输入欠压
	OBC_BMS_Data.OBCOutpUUnder = (BOOL)(Data[6] & 0x01);//OBC输出欠压
	OBC_BMS_Data.OBCInpIOver = (BOOL)((Data[7] & 0x80) >> 7);//输入过流
	OBC_BMS_Data.OBCOutpIOver = (BOOL)((Data[7] & 0x40) >> 6);//输出过流
	OBC_BMS_Data.OBCTOvrErr = (BOOL)((Data[7] & 0x20) >> 5);//过温故障	
	Myself.OBC_BMS_331_Timeout = 0;
}

void Receive_Vechicle(Can_message_struct*  PCAN_Store)
{        	
    switch(PCAN_Store->ID)
    {
        case 0x110: 
      	{
          	VCU_BMS_110_Read(PCAN_Store->Data);
          	break;
      	}
		
        case 0x311: 
      	{
          	OBC_BMS_311_Read(PCAN_Store->Data);
         	break;
      	}
        case 0x321: 
      	{
          	OBC_BMS_321_Read(PCAN_Store->Data);
          	break;
      	}
		
        case 0x331: 
      	{
          	OBC_BMS_331_Read(PCAN_Store->Data);
         	break;
      	}
        
        default:
      	{
          	break; //这里需要添加整车can接收错误的计数处理
      	}
    }
}

void VCU_OBC_BMS_communicate_error(void)
{// 20ms 调用一次
	if(VCU_BMS_Data.Receied_VCU > 0)
	{
		Myself.VCU_BMS_110_Timeout++;// 超时为传输的10个周期 10*10ms 
		Myself.OBC_BMS_311_Timeout++;// 10*100ms
		Myself.OBC_BMS_321_Timeout++;// 10*10ms
		Myself.OBC_BMS_331_Timeout++;// 10*100ms
		if(Myself.VCU_BMS_110_Timeout > 4)
		{
			memset(&VCU_BMS_Data,0,sizeof(VCU_BMS_Struct));
			Enable_warring(DTC_063_CntrCANErr1);
		}
		
		if(Myself.OBC_BMS_311_Timeout > 49)
		{		
			OBC_BMS_Data.OBCMaxChrgOutpUAvl = 0;//充电机最大允许输出电压   放大10倍
			OBC_BMS_Data.OBCMaxChrgOutpIAvl = 0;//充电机最大允许输出电流   放大10倍
			OBC_BMS_Data.OBCMaxChrgOutpPwrAvl = 0;//充电机最大允许输出功率
			OBC_BMS_Data.OBCVehInletT = 0;//交流充电插座温度
			OBC_BMS_Data.OBCChrgSts = 0;//充电机工作状态
			OBC_BMS_Data.PlugLockStatus = 0;//充电枪锁止状态
			OBC_BMS_Data.OBCS2Stus = 0;//S2状态
			Enable_warring(DTC_064_CntrCANErr2);
		}
		
		if(Myself.OBC_BMS_321_Timeout > 4)
		{
			OBC_BMS_Data.OBCCPValue = 0;
			OBC_BMS_Data.CPSts = 0;
			Enable_warring(DTC_065_CntrCANErr3);
		}
		
		if(Myself.OBC_BMS_331_Timeout > 49)
		{
			OBC_BMS_Data.OBCChrgInpAcU = 0;
			OBC_BMS_Data.OBCChrgInpAcU = 0;//充电机交流端实时输入电压
			OBC_BMS_Data.OBCChrgInpAcI = 0;//充电机交流端实时输入电流   放大10倍
			OBC_BMS_Data.OBCChrgDcU = 0;//充电机实时充电电压    放大10倍
			OBC_BMS_Data.OBCChrgDcI = 0;//充电机实时充电电流   放大10倍
			OBC_BMS_Data.OBCT = 0;//充电机温度
			OBC_BMS_Data.OBCCANErr = 0;//OBC通信故障
			OBC_BMS_Data.OBCHwErr = 0;//OBC硬件故障
			OBC_BMS_Data.OBCInpUOver = 0;//OBC输入过压
			OBC_BMS_Data.OBCOutpUOver = 0;//OBC输出过压
			OBC_BMS_Data.OBCInpUUnder = 0;//OBC输入欠压
			OBC_BMS_Data.OBCOutpUUnder = 0;//OBC输出欠压
			OBC_BMS_Data.OBCInpIOver = 0;//输入过流
			OBC_BMS_Data.OBCOutpIOver = 0;//输出过流
			OBC_BMS_Data.OBCTOvrErr = 0;//过温故障
			Enable_warring(DTC_066_CntrCANErr4);
		}
	}
}

