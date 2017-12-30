#include <includes.h>

void BMS_VCU_113_send (void)
{
	static INT8U Counter_num = 0;
    Can_message_struct BMS_Tx;
	VINT32U temp;
	#if 0
	// 测试使用
	INT16U BMSOperMod;
	INT16U BMSOBCOperModReq;
	INT16U BMSPwrUpAllw;
	BMSOperMod = 5;
	BMSOBCOperModReq = 2;
	BMSPwrUpAllw = 1;
//	Myself.Warning_Level = 3;
//    Myself.SOC_ext = 100;
	#endif
	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;
    BMS_Tx.ID  = 0x113;
    BMS_Tx.DLC =8;
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT16U*)(&(BMS_Tx.Data[0])) = Core_data.Cell_Total_voltage << 3;  //精度0.1V
	temp = (Core_data.Current + 6000) & 0x3FFF;  //上海地标需要修改
	*(INT32U*)(&(BMS_Tx.Data[1])) |= (INT32U)(temp << 13);
	*(INT16U*)(&(BMS_Tx.Data[3])) |= Core_data.SOC_int << 3; //上海地标需要修改//Myself.SOC_ext << 3; //上海地标需要修改
	BMS_Tx.Data[4] |= Myself.Warning_Level & 0b111;
	BMS_Tx.Data[5] = BMSOperMod << 5;  				//应用层   BCU工作模式
	BMS_Tx.Data[5] |= BMSOBCOperModReq << 3; 		//应用层 OBC工作模式请求
	BMS_Tx.Data[5] |= (BMSPwrUpAllw & 0x01) << 2; 	// 应用层 允许重新上高压电
	BMS_Tx.Data[5] |= (Myself.HVLOCK_Status & 0x01) << 1;
	BMS_Tx.Data[5] |= (Myself.ST_Break & 0x01);
	BMS_Tx.Data[6] |= (INT8U)((BMSRlyWlddErr & 0x01) << 7); //主继电器常闭故障
	#if 0
	添加报警
	绝缘状态
	电池内部绝缘故障
	高压负载绝缘故障
	#endif
 	BMS_Tx.Data[6] |= Counter_num++;
	if(Counter_num > 0x0E)
	{
		Counter_num = 0;
	}
	
    Can1_Send(&BMS_Tx,OS_POST_OPT_FRONT);
}

void BMS_VCU_213_send (void)
{
	static INT8U Counter_num = 0;
    Can_message_struct BMS_Tx;
#if 0
    //为了做测试
	INT16U BMSMaxChrgU;  //最大充电电压
	INT16U BMSMinDchaIU; //电池最小放电电压
//	Myself.Link_voltage = 350;
	BMSMaxChrgU = 400;
	BMSMinDchaIU = 300;
#endif	
	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;
    BMS_Tx.ID  = 0x213;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT16U*)(&(BMS_Tx.Data[0])) = 374 << 7;  //应用层 电池最大充电电压  374V   范围0~500 精度 1V
	*(INT16U*)(&(BMS_Tx.Data[1])) |= 242 << 6;  // 应用层 电池最小放电电压 242V
	*(INT16U*)(&(BMS_Tx.Data[2])) |= (Myself.Link_voltage & 0xFFF) << 1;

 	BMS_Tx.Data[6] = Counter_num++;
	if(Counter_num > 0x0E)
	{
		Counter_num = 0;
	}
	
    Can1_Send(&BMS_Tx,OS_POST_OPT_FRONT);
}

void BMS_VCU_233_send (void)
{
	static INT8U Counter_num = 0;
    Can_message_struct BMS_Tx;

	#if 0
	//测试使用
	INT16U	BMSMaxDchaPwr;
	INT16U	BMSMaxChrgPwr;
	INT16U	BMSMaxDchaI;
	INT16U	BMSMaxChrgI;
	BMSMaxDchaPwr = 200;
	BMSMaxChrgPwr = 250;
	BMSMaxDchaI = 300;
	BMSMaxChrgI= 350;
	#endif

    BMS_Tx.ID  = 0x233;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;
	
	*(INT16U*)(&(BMS_Tx.Data[0])) = (INT16U)((BMSMaxDchaPwr & 0x1FF) << 7); //应用层 电池最大放电功率    范围0~500 精度 1V
	*(INT16U*)(&(BMS_Tx.Data[1])) |= (BMSMaxChrgPwr & 0x1FF) << 6;  // 应用层   电池最大充电功率
	*(INT16U*)(&(BMS_Tx.Data[2])) |= (BMSMaxDchaI  & 0x1FF) << 4;   // 应用层  电池最大允许放电电流
	*(INT16U*)(&(BMS_Tx.Data[3])) |= (BMSMaxChrgI  & 0x3FF) << 2;	// 应用层  电池最大允许充电电流

 	BMS_Tx.Data[6] = Counter_num++;
	if(Counter_num > 0x0E)
	{
		Counter_num = 0;
	}
	
    Can1_Send(&BMS_Tx,OS_POST_OPT_FRONT);
}

void BMS_OBC_303_send (void)
{
	static INT8U Counter_num = 0;
    Can_message_struct BMS_Tx;
	#if 0
	//测试使用
	INT16U BMSChrgUReq;  //放大10倍
	INT16U 	BMSChrgIReq; //放大10倍
	INT16U 	BMSOBCMaxChrgPwrAvl;//放大10倍
//	INT16U 	BMSMaxChrgI;
	INT8U 	BMSOBCHeatModeCtrl;
	INT8U 	BMSChrgMod;
	INT8U 	BMSChrgErrInfo;
	INT8U 	BMSChrgSts;

	BMSChrgUReq = 50;
	BMSChrgIReq = 60;
	BMSOBCMaxChrgPwrAvl = 70;
//	BMSMaxChrgI = 80;
	BMSOBCHeatModeCtrl = 1;
	BMSChrgMod = 1;
	BMSChrgErrInfo = 1;
	BMSChrgSts = 1;

	#endif
	
    BMS_Tx.ID  = 0x303;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;
	
	*(INT16U*)(&(BMS_Tx.Data[0])) = BMSChrgUReq;  			//应用层 外插充电需求电压
	*(INT16U*)(&(BMS_Tx.Data[2])) |= (INT16U)((BMSChrgIReq & 0x7FF) << 5);  // 应用层   外插充电需求电流
	*(INT16U*)(&(BMS_Tx.Data[3])) |= (INT16U)((BMSOBCMaxChrgPwrAvl  & 0x3FF) << 3);   // 应用层  OBC最大允许输出功率
//	*(INT16U*)(&(BMS_Tx.Data[3])) |= (INT16U)((BMSMaxChrgI  & 0x1FF) << 2);	// 应用层  电池充电模式
//	BMS_Tx.Data[4] |= (INT8U)(BMSOBCHeatModeCtrl & 0x01) << 2;//BMSOBCHeatModeCtrl = //加热模式控制 暂时不使用
	*(INT16U*)(&(BMS_Tx.Data[4])) |= (INT16U)(BMSChrgMod & 0x07) << 7; // 应用层  电池充电模式
//	BMS_Tx.Data[5] |= (INT8U)((BMSChrgErrInfo & 0x0F) << 3);
	BMS_Tx.Data[5] |= (INT8U)(BMSChrgSts & 0x03) << 1;    //电池充电状态
	BMS_Tx.Data[5] |= (INT8U)(Core_data.Relay_DC_Status & 0x01);//BMSDCChrgRlySts 直流充电继电器状态
	//BMSDCChrgRlySts 直流充电继电器状态
	
 	BMS_Tx.Data[6] = Counter_num++;
	if(Counter_num > 0x0E)
	{
		Counter_num = 0;
	}
	
    Can1_Send(&BMS_Tx,OS_POST_OPT_FRONT);
}

void BMS_VCU_313_send (void)
{
	static INT8U Counter_num = 0;
    Can_message_struct BMS_Tx;
	INT32U temp;
	INT16U temp_16;
	#if 0
	//测试使用
	INT16U BMSPredChrgTi;
	INT8U BMSBattIntrHeatReq;
	INT8U BMSBattIntrHeatActv;

	BMSPredChrgTi = 100;
	BMSBattIntrHeatReq = 1;
	BMSBattIntrHeatActv	= 1;
	#endif
    BMS_Tx.ID  = 0x313;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;

	BMS_Tx.Data[0] = (Core_data.Cell_Temp_MAX / 10) + 40; //电池最高温度  	
	BMS_Tx.Data[1] = (Core_data.Cell_Temp_Min / 10) + 40; //电池最低温度
	
	*(INT16U*)(&(BMS_Tx.Data[2])) = (INT16U)((coch_tiChrgTimRem & 0x3FF) << 6);  // 应用层待添加  //充电剩余时间
	BMS_Tx.Data[3] |= (BMSBattIntrHeatReq & 0x03) << 4;   //电池内部加热请求
	BMS_Tx.Data[3] |= (BMSBattIntrHeatActv & 0x03) << 2;  //电池内部加热激活
	
	//这里也可以考虑在计算绝缘电阻时进行3000的比较
	temp = Core_data.Resistor;
	if(temp > 3000)
	{
		temp = 3000;
	}
	
	*(INT32U*)(&(BMS_Tx.Data[3])) |= temp << 14;  //输入绝缘电阻
	temp_16 = Myself.Cell_Temp_Average + 40;
	*(INT16U*)(&(BMS_Tx.Data[5])) |= (temp_16) << 6;  //电池平均温度
	//BMS_Tx.Data[6] |= (BMSMILReq) << 5; //电池MIL请求
		
 	BMS_Tx.Data[6] |= Counter_num++;
	if(Counter_num > 0x0E)
	{
		Counter_num = 0;
	}
	
    Can1_Send(&BMS_Tx,OS_POST_OPT_FRONT);
}

void BMS_VCU_323_send (void)
{
    Can_message_struct BMS_Tx;
	VINT32U temp;
	
    BMS_Tx.ID  = 0x323;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  
	
	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;

	BMS_Tx.Data[0] = 100; //常量 电池SOC上限
	BMS_Tx.Data[1] = 8;  //常量 电池SOC下限

	*(INT16U*)(&(BMS_Tx.Data[2])) = (INT16U)((1000) << 6);  //SOH 
		
//    temp = 1500;
	temp = (INT32U)(/*Core_data.SOC_int **/ 141);  //超范围  发送电池的容量 是个常量
	temp *= 10; //精度0.1

	*(INT32U*)(&(BMS_Tx.Data[3])) |= temp << 15;  //剩余容量 单位 Ah 精度0.1Ah
    Can1_Send(&BMS_Tx,OS_POST_OPT_FRONT);
}

void BMS_VCU_413_send (void)
{
    Can_message_struct BMS_Tx;
	static INT8U SendToVCU_Num = 0;
	INT32U temp;
	
    #if 0
	//测试使用
	Myself.CC2_value = 2000;
    Myself.current_Heat = 200;
    Myself.BMSErrAmnt = 5;
    Myself.CC1_value = 3000;
	#endif
    BMS_Tx.ID  = 0x413;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;

	if(Myself.SocInit_done < 1)
	{
		*(INT16U*)(&(BMS_Tx.Data[0])) = (INT16U)((0x1FFF) << 3);  //CC2电阻
		*(INT16U*)(&(BMS_Tx.Data[1])) |= (INT16U)((0xFF) << 3); //加热回路电流
	}
	else
	{
		*(INT16U*)(&(BMS_Tx.Data[0])) = (INT16U)((Myself.CC2_value) << 3);  //CC2电阻
		*(INT16U*)(&(BMS_Tx.Data[1])) |= (INT16U)((Myself.current_Heat) << 3); //加热回路电流	
	}
	
	if(Myself.BMSErrAmnt > 0)
	{
		*(INT16U*)(&(BMS_Tx.Data[2])) |= (INT16U)(Myself.BMSErrAmnt) << 7;//最大值是4000
		*(INT16U*)(&(BMS_Tx.Data[3])) |= (INT16U)(Fault_SendtoVCU[SendToVCU_Num]) << 3;//最大值是0x0F 需要根据故障诊断方案填写
	    SendToVCU_Num++;
	    if(SendToVCU_Num > (Myself.BMSErrAmnt - 1))
	    {
	        SendToVCU_Num = 0;
	    }		
	}
	else
	{
		SendToVCU_Num = 0;
	}	

    temp = (INT32U)Myself.CC1_value;
	*(INT32U*)(&(BMS_Tx.Data[4])) |= temp << 14;  //剩余容量 单位 Ah 精度0.1Ah

    Can1_Send(&BMS_Tx,OS_POST_OPT_FRONT);
}

//BMSCC2SngR
void BMS_VCU_513_send (void)
{//暂时空缺 还没有清楚软件的版本信息
    Can_message_struct BMS_Tx;
	INT16U temp;
	
    BMS_Tx.ID  = 0x513;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

    *(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;
    
	*(INT16U*)(&(BMS_Tx.Data[0])) = BMSSwVers;
	*(INT8U*)(&(BMS_Tx.Data[2])) = BMSHwVers;
    *(INT16U*)(&(BMS_Tx.Data[3])) = BMSCalDataVers;
	*(INT16U*)(&(BMS_Tx.Data[5])) = (INT16U)(((INT16U)BMSManfctCode) << 6);
//    BMS_Tx.Data[5] = (BMSManfctCode );
//    BMS_Tx.Data[6] = BMSManfctCode & 0x3;
	
    Can1_Send(&BMS_Tx,OS_POST_OPT_FRONT);
}

void BMS_GW_603_send (void)
{//这里只是暂时填写
    Can_message_struct BMS_Tx;

    BMS_Tx.ID  = 0x603;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;
	BMS_Tx.Data[0] = 1; //动力蓄电池包总数
	BMS_Tx.Data[1] = 88; //单体蓄电池总数
	BMS_Tx.Data[2] = 22; //动力电池包动力蓄电池温度探针总数
	BMS_Tx.Data[3] = (1 << 4) | (88 >> 4); //动力蓄电池包总数
	BMS_Tx.Data[4] = ((88 & 0x0F) << 4) | (22 >> 4); //动力蓄电池包总数
	BMS_Tx.Data[5] = (22 & 0x0F) << 4;
	
    Can1_Send(&BMS_Tx,OS_POST_OPT_FRONT);
}

void BMS_GW_613_send (void) //1000ms
{//这里只是暂时填写
    Can_message_struct BMS_Tx;
    INT8U bit0_flg;
    INT8U bit1_flg;
    INT8U bit2_flg;
    INT8U bit3_flg;
    INT8U bit4_flg;
    INT8U bit5_flg;
    INT8U bit6_flg;
    INT8U bit7_flg;

    BMS_Tx.ID  = 0x613;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;
	//添加故障信息
	#if 0
    Fault_save[12] = 0x03;
    Fault_save[10] = 0x21;
    
    #endif
	bit0_flg = (Fault_save[12] >> 2) & 0x01;
    bit1_flg = (Fault_save[12] >> 1) & 0x01;
    bit2_flg = 0;
    if(((Fault_save[10] >> 5) & 0x01 == 1) || ((Fault_save[10] >> 6) & 0x01 == 1))
    {
        bit3_flg = 1;
    }
    else
    {
        bit3_flg = 0;
    }
    if(((Fault_save[10] >> 7) & 0x01 == 1) || ((Fault_save[11] >> 0) & 0x01 == 1))
    {
        bit4_flg = 1;
    }
    else
    {
        bit4_flg = 0;
    }
    bit5_flg = (Fault_save[11] >> 4) & 0x01;
    bit6_flg = (Fault_save[11] >> 1) & 0x01;
    bit7_flg = (Fault_save[11] >> 2) & 0x01;
	BMS_Tx.Data[0] = bit0_flg | (bit1_flg << 1) | (bit2_flg << 2) | (bit3_flg << 3) | (bit4_flg << 4) | (bit5_flg << 5) 
                    | (bit6_flg << 6) | (bit7_flg << 7);
    bit0_flg = 0;
    bit1_flg = 0;
    if(((Fault_save[11] >> 7) & 0x01 == 1) || ((Fault_save[12] >> 0) & 0x01 == 1))
    {
        bit2_flg = 1;
    }
    else
    {
        bit2_flg = 0;
    }
    if(((Fault_save[11] >> 5) & 0x01 == 1) || ((Fault_save[11] >> 6) & 0x01 == 1))
    {
        bit3_flg = 1;
    }
    else
    {
        bit3_flg = 0;
    }
    bit4_flg = 0;
    bit5_flg = 0;
    bit6_flg = 0;
    bit7_flg = (Fault_save[7] >> 0) & 0x01;
	BMS_Tx.Data[1] = bit0_flg | (bit1_flg << 1) | (bit2_flg << 2) | (bit3_flg << 3) | (bit4_flg << 4) | (bit5_flg << 5) 
                    | (bit6_flg << 6) | (bit7_flg << 7);
    if(((Fault_save[10] >> 5) & 0x01 == 1) || ((Fault_save[10] >> 6) & 0x01 == 1) ||
        ((Fault_save[10] >> 7) & 0x01 == 1) || ((Fault_save[11] >> 0) & 0x01 == 1))
    {
        bit7_flg = 1;
    }
    else
    {
        bit7_flg = 0;
    }
    bit6_flg = (Fault_save[12] >> 1) & 0x01;
    BMS_Tx.Data[2] = ((bit7_flg << 7) | (bit6_flg << 6)) & 0xC0;
    Can1_Send(&BMS_Tx,OS_POST_OPT_NONE);
}

void BMS_GW_623_send (void)
{//这里只是暂时填写
    Can_message_struct BMS_Tx;

    BMS_Tx.ID  = 0x623;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;
	if(Myself.SocInit_done < 1)
	{
    BMS_Tx.Data[0] = 0xFF; 
	BMS_Tx.Data[1] = 0xFF;
	}
	else
	{
	    BMS_Tx.Data[0] = Core_data.Cell_Temp_Min / 10 + 40; 
		BMS_Tx.Data[1] = Core_data.Cell_Temp_MAX / 10 + 40;	
	}
    Can1_Send(&BMS_Tx,OS_POST_OPT_NONE);
}

void BMS_GW_633_send (void) //100mS
{//这里只是暂时填写
    Can_message_struct BMS_Tx;

    BMS_Tx.ID  = 0x633;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;

	if(Myself.SocInit_done < 1)
	{
		/*BMS_Tx.Data[0] = Core_data.Cell_Voltage_Min >> 8; 
		BMS_Tx.Data[1] = Core_data.Cell_Voltage_Min & 0xFF;*/

		*(INT16U*)(&(BMS_Tx.Data[0])) = 0xFFFF;		
	}
	else
	{
	    /*BMS_Tx.Data[0] = Core_data.Cell_Voltage_Min >> 8; 
		BMS_Tx.Data[1] = Core_data.Cell_Voltage_Min & 0xFF;	*/
		*(INT16U*)(&(BMS_Tx.Data[0])) = Core_data.Cell_Voltage_Min;
	}
    BMS_Tx.Data[2] = Core_data.Cell_Voltage_Min_number;   
    BMS_Tx.Data[3] = 1;
	/*
    BMS_Tx.Data[4] = Core_data.Cell_Voltage_Max >> 8; 
	BMS_Tx.Data[5] = Core_data.Cell_Voltage_Max & 0xFF;*/
	if(Myself.SocInit_done < 1)
	{
		*(INT16U*)(&(BMS_Tx.Data[4])) = 0xFFFF;		
	}
	else
	{
		*(INT16U*)(&(BMS_Tx.Data[4])) = Core_data.Cell_Voltage_Max;
	}
    BMS_Tx.Data[6] = Core_data.Cell_Voltage_Max_number;   
    BMS_Tx.Data[7] = 1;
    Can1_Send(&BMS_Tx,OS_POST_OPT_FRONT);
}

void BMS_GW_643_send (void)//100mS
{//这里只是暂时填写
    Can_message_struct BMS_Tx;

    BMS_Tx.ID  = 0x643;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;

    BMS_Tx.Data[0] = Core_data.Cell_Temp_Min / 10 + 40;  
	BMS_Tx.Data[1] = Core_data.Cell_Temp_Min_Num;
    BMS_Tx.Data[2] = 1;
    BMS_Tx.Data[3] = Core_data.Cell_Temp_MAX / 10 + 40; 
    BMS_Tx.Data[4] = Core_data.Cell_Temp_MAX_number; 
	BMS_Tx.Data[5] = 1; 

    Can1_Send(&BMS_Tx,OS_POST_OPT_FRONT);
}

void BMS_GW_653_send (void)
{//这里只是暂时填写
    Can_message_struct BMS_Tx;

    BMS_Tx.ID  = 0x653;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;

	*(INT16U*)(&(BMS_Tx.Data[0])) = 141 * 5;
	*(INT16U*)(&(BMS_Tx.Data[2])) = 1410 - (1410 * Core_data.SOC_int / 1000); //重点测试
	//BMSSOCUproMod 电压切换模式 解释不清楚
	//BLC_CellNr	均衡数据循环单体号

    Can1_Send(&BMS_Tx,OS_POST_OPT_NONE);
}

void BMS_GW_626_send (void)
{//这里只是暂时填写
    Can_message_struct BMS_Tx;

    BMS_Tx.ID  = 0x626;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;

    //均衡状态 先不做
    Can1_Send(&BMS_Tx,OS_POST_OPT_NONE);
}

void BMS_GW_627_send (void)
{
    Can_message_struct BMS_Tx;

    BMS_Tx.ID  = 0x627;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;

    //均衡状态 先不做
    Can1_Send(&BMS_Tx,OS_POST_OPT_NONE);
}

void BMS_GW_62B_send (void)
{
    Can_message_struct BMS_Tx;

    BMS_Tx.ID  = 0x62B;
    BMS_Tx.DLC =8; 
    BMS_Tx.IDTYPE = Standard_ID;  

	*(INT32U*)(&(BMS_Tx.Data[0])) = 0;
	*(INT32U*)(&(BMS_Tx.Data[4])) = 0;

    //先不做
    Can1_Send(&BMS_Tx,OS_POST_OPT_NONE);
}

void BMS_GW_660_678_send (void)
{
    Can_message_struct BMS_Send;
    INT16U i,j;//i:10块6804，t:每个ID采4个电压

	BMS_Send.ID = 0x660 ;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	for(i = 0;i < Slave_Board_Num;i++)
	{
		for(j = 0;j < 6;j++)
	{
			 BMS_Send.Data[0] = (Slave[i].Battery_Info[j*4].CellVoltage & 0xFF00 ) >> 8; 
             BMS_Send.Data[1] = Slave[i].Battery_Info[j*4].CellVoltage & 0x00FF; 
			 BMS_Send.Data[2] = (Slave[i].Battery_Info[j*4 + 1].CellVoltage & 0xFF00 ) >> 8; 
             BMS_Send.Data[3] = Slave[i].Battery_Info[j*4 + 1].CellVoltage & 0x00FF; 
			 BMS_Send.Data[4] = (Slave[i].Battery_Info[j*4 + 2].CellVoltage & 0xFF00 ) >> 8; 
             BMS_Send.Data[5] = Slave[i].Battery_Info[j*4 + 2].CellVoltage & 0x00FF; 
			 BMS_Send.Data[6] = (Slave[i].Battery_Info[j*4 + 3].CellVoltage & 0xFF00 ) >> 8; 
             BMS_Send.Data[7] = Slave[i].Battery_Info[j*4 + 3].CellVoltage & 0x00FF;
             if(BMS_Send.ID == 0x674)
		{
                 BMS_Send.Data[0] = (Slave[3].Battery_Info[0].CellVoltage & 0xFF00 ) >> 8; 
                 BMS_Send.Data[1] = Slave[3].Battery_Info[0].CellVoltage & 0x00FF; 
    			 BMS_Send.Data[2] = (Slave[3].Battery_Info[1].CellVoltage & 0xFF00 ) >> 8; 
                 BMS_Send.Data[3] = Slave[3].Battery_Info[1].CellVoltage & 0x00FF; 
    			 BMS_Send.Data[4] = (Slave[3].Battery_Info[2].CellVoltage & 0xFF00 ) >> 8; 
                 BMS_Send.Data[5] = Slave[3].Battery_Info[2].CellVoltage & 0x00FF; 
    			 BMS_Send.Data[6] = (Slave[3].Battery_Info[3].CellVoltage & 0xFF00 ) >> 8; 
                 BMS_Send.Data[7] = Slave[3].Battery_Info[3].CellVoltage & 0x00FF;
		}
             if(BMS_Send.ID == 0x675)
		{
                 BMS_Send.Data[0] = (Slave[3].Battery_Info[4].CellVoltage & 0xFF00 ) >> 8; 
                 BMS_Send.Data[1] = Slave[3].Battery_Info[4].CellVoltage & 0x00FF; 
    			 BMS_Send.Data[2] = (Slave[3].Battery_Info[5].CellVoltage & 0xFF00 ) >> 8; 
                 BMS_Send.Data[3] = Slave[3].Battery_Info[5].CellVoltage & 0x00FF; 
    			 BMS_Send.Data[4] = (Slave[3].Battery_Info[6].CellVoltage & 0xFF00 ) >> 8; 
                 BMS_Send.Data[5] = Slave[3].Battery_Info[6].CellVoltage & 0x00FF; 
    			 BMS_Send.Data[6] = (Slave[3].Battery_Info[7].CellVoltage & 0xFF00 ) >> 8; 
                 BMS_Send.Data[7] = Slave[3].Battery_Info[7].CellVoltage & 0x00FF;
		}
             Can1_Send(&BMS_Send,0);
             BMS_Send.ID++;
		//	 OSTimeDly(3);
	}
}

	memset(&BMS_Send.Data[0],0,8);
    BMS_Send.DLC =8; 
    BMS_Send.IDTYPE = Standard_ID; 
    BMS_Send.ID = 0x676;
    Can1_Send(&BMS_Send,0);
    OSTimeDly(3);
    BMS_Send.ID = 0x677;
    Can1_Send(&BMS_Send,0);
    OSTimeDly(3);
    BMS_Send.ID = 0x678;
    Can1_Send(&BMS_Send,0);
//    Can1_Send(&BMS_Send,OS_POST_OPT_FRONT);
}

void BMS_GW_683_send (void)
{
    Can_message_struct BMS_Send;

    BMS_Send.ID = 0x683;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID; 

	BMS_Send.Data[0] = Slave[0].LTC6804[0].CellTemperature[2].CellTemp / 10 + 40;
	BMS_Send.Data[1] = Slave[0].LTC6804[0].CellTemperature[3].CellTemp / 10 + 40;
	BMS_Send.Data[2] = Slave[0].LTC6804[0].CellTemperature[0].CellTemp / 10 + 40;
	BMS_Send.Data[3] = Slave[0].LTC6804[0].CellTemperature[1].CellTemp / 10 + 40;
//    BMS_Send.Data[3] = Slave[0].LTC6804[1].CellTemperature[0].CellTemp / 10 + 40;
	BMS_Send.Data[4] = Slave[0].LTC6804[1].CellTemperature[1].CellTemp / 10 + 40;
	BMS_Send.Data[5] = Slave[0].LTC6804[1].CellTemperature[2].CellTemp / 10 + 40;
	BMS_Send.Data[6] = Slave[1].LTC6804[0].CellTemperature[0].CellTemp / 10 + 40;
	BMS_Send.Data[7] = Slave[1].LTC6804[0].CellTemperature[1].CellTemp / 10 + 40;		
	Can1_Send(&BMS_Send,OS_POST_OPT_NONE);
}

void BMS_GW_693_send (void)
{
    Can_message_struct BMS_Send;

    BMS_Send.ID = 0x693;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID; 

	BMS_Send.Data[0] = Slave[1].LTC6804[0].CellTemperature[2].CellTemp / 10 + 40;
	BMS_Send.Data[1] = Slave[1].LTC6804[1].CellTemperature[0].CellTemp / 10 + 40;
	BMS_Send.Data[2] = Slave[1].LTC6804[1].CellTemperature[1].CellTemp / 10 + 40;
	BMS_Send.Data[3] = Slave[1].LTC6804[1].CellTemperature[2].CellTemp / 10 + 40;
	BMS_Send.Data[4] = Slave[2].LTC6804[0].CellTemperature[0].CellTemp / 10 + 40;
	BMS_Send.Data[5] = Slave[2].LTC6804[0].CellTemperature[1].CellTemp / 10 + 40;
	BMS_Send.Data[6] = Slave[2].LTC6804[0].CellTemperature[2].CellTemp / 10 + 40;
	BMS_Send.Data[7] = Slave[2].LTC6804[1].CellTemperature[0].CellTemp / 10 + 40;		
	Can1_Send(&BMS_Send,OS_POST_OPT_NONE);
}

void BMS_GW_6A3_send (void)
{
    Can_message_struct BMS_Send;

    BMS_Send.ID = 0x6A3;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID; 

	BMS_Send.Data[0] = Slave[2].LTC6804[1].CellTemperature[1].CellTemp / 10 + 40;
	BMS_Send.Data[1] = Slave[2].LTC6804[1].CellTemperature[2].CellTemp / 10 + 40;
	BMS_Send.Data[2] = Slave[3].LTC6804[0].CellTemperature[0].CellTemp / 10 + 40;
	BMS_Send.Data[3] = Slave[3].LTC6804[0].CellTemperature[1].CellTemp / 10 + 40;
	BMS_Send.Data[4] = Slave[3].LTC6804[1].CellTemperature[0].CellTemp / 10 + 40;
	BMS_Send.Data[5] = Slave[3].LTC6804[1].CellTemperature[1].CellTemp / 10 + 40;
	BMS_Send.Data[6] = 0xFF;
	BMS_Send.Data[7] = 0xFF;
	Can1_Send(&BMS_Send,OS_POST_OPT_NONE);
}

void BMS_GW_660_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x660;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 0;
	Cell_number = 0;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);

}

void BMS_GW_661_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x661;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 0;
	Cell_number = 4;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_662_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x662;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 0;
	Cell_number = 8;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_663_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x663;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 0;
	Cell_number = 12;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_664_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x664;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 0;
	Cell_number = 16;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_665_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x665;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 0;
	Cell_number = 20;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_666_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x666;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 1;
	Cell_number = 0;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);

}

void BMS_GW_667_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x667;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 1;
	Cell_number = 4;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_668_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x668;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 1;
	Cell_number = 8;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_669_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x669;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 1;
	Cell_number = 12;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_66A_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x66A;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 1;
	Cell_number = 16;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_66B_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x66B;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 1;
	Cell_number = 20;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}


void BMS_GW_66C_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x66C ;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 2;
	Cell_number = 0;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);

}

void BMS_GW_66D_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x66D;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 2;
	Cell_number = 4;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_66E_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x66E;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 2;
	Cell_number = 8;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_66F_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x66F ;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 2;
	Cell_number = 12;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_670_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x670;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 2;
	Cell_number = 16;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_671_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x671;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 2;
	Cell_number = 20;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}


void BMS_GW_672_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x672 ;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 3;
	Cell_number = 0;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);

}

void BMS_GW_673_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x673 ;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 3;
	Cell_number = 4;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_674_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x674 ;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 3;
	Cell_number = 12;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

void BMS_GW_675_send (void)
{
    Can_message_struct BMS_Send;
	INT8U Slave_number;
	INT8U Cell_number;
	BMS_Send.ID = 0x675 ;
    BMS_Send.DLC = 8; 
    BMS_Send.IDTYPE = Standard_ID;
	Slave_number = 3;
	Cell_number = 16;

	 BMS_Send.Data[0] = (Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[1] = Slave[Slave_number].Battery_Info[Cell_number].CellVoltage & 0x00FF; 
	 BMS_Send.Data[2] = (Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[3] = Slave[Slave_number].Battery_Info[Cell_number + 1].CellVoltage & 0x00FF; 
	 BMS_Send.Data[4] = (Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[5] = Slave[Slave_number].Battery_Info[Cell_number + 2].CellVoltage & 0x00FF; 
	 BMS_Send.Data[6] = (Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0xFF00 ) >> 8; 
     BMS_Send.Data[7] = Slave[Slave_number].Battery_Info[Cell_number + 3].CellVoltage & 0x00FF;	
     Can1_Send(&BMS_Send,OS_POST_OPT_NONE);	 
}

#if 0
void BMS_GW_683_6B3_send (INT8U number)
{
    Can_message_struct BMS_Send;
	
	if(number < 4)
	{
	    BMS_Send.ID = 0x683 + number * 0x10;
	    BMS_Send.DLC = 8; 
	    BMS_Send.IDTYPE = Standard_ID; 

		BMS_Send.Data[0] = Slave[number].LTC6804[0].CellTemperature[0].CellTemp / 10 + 40;
		BMS_Send.Data[1] = Slave[number].LTC6804[0].CellTemperature[1].CellTemp / 10 + 40;
		BMS_Send.Data[2] = Slave[number].LTC6804[0].CellTemperature[2].CellTemp / 10 + 40;
		BMS_Send.Data[3] = Slave[number].LTC6804[0].CellTemperature[3].CellTemp / 10 + 40;
		BMS_Send.Data[4] = Slave[number].LTC6804[1].CellTemperature[0].CellTemp / 10 + 40;
		BMS_Send.Data[5] = Slave[number].LTC6804[1].CellTemperature[1].CellTemp / 10 + 40;
		BMS_Send.Data[6] = Slave[number].LTC6804[1].CellTemperature[2].CellTemp / 10 + 40;
		BMS_Send.Data[7] = Slave[number].LTC6804[1].CellTemperature[3].CellTemp / 10 + 40;		
		Can1_Send(&BMS_Send,0);
	}
}
#endif

#if 0
void BMS_GW_683_6B3_send (void)
{
    Can_message_struct BMS_Send;

    BMS_Send.ID = 0x683;
    BMS_Send.DLC =8; 
    BMS_Send.IDTYPE = Standard_ID; 

	BMS_Send.Data[0] = Slave[0].LTC6804[0].CellTemperature[0].CellTemp / 10 + 40;
	BMS_Send.Data[1] = Slave[0].LTC6804[0].CellTemperature[1].CellTemp / 10 + 40;
	BMS_Send.Data[2] = Slave[0].LTC6804[0].CellTemperature[2].CellTemp / 10 + 40;
	BMS_Send.Data[3] = Slave[0].LTC6804[0].CellTemperature[3].CellTemp / 10 + 40;
	BMS_Send.Data[4] = Slave[0].LTC6804[1].CellTemperature[0].CellTemp / 10 + 40;
	BMS_Send.Data[5] = Slave[0].LTC6804[1].CellTemperature[1].CellTemp / 10 + 40;
	BMS_Send.Data[6] = Slave[0].LTC6804[1].CellTemperature[2].CellTemp / 10 + 40;
	BMS_Send.Data[7] = Slave[0].LTC6804[1].CellTemperature[3].CellTemp / 10 + 40;
			
	Can1_Send(&BMS_Send,0);
	BMS_Send.ID += 0x10; //0x693
	OSTimeDlyHMSM(0, 0, 0,10);
			
  	BMS_Send.Data[0] = Slave[1].LTC6804[0].CellTemperature[0].CellTemp / 10 + 40;
  	BMS_Send.Data[1] = Slave[1].LTC6804[0].CellTemperature[1].CellTemp / 10 + 40;
  	BMS_Send.Data[2] = Slave[1].LTC6804[0].CellTemperature[2].CellTemp / 10 + 40;
  	BMS_Send.Data[3] = Slave[1].LTC6804[0].CellTemperature[3].CellTemp / 10 + 40;
	BMS_Send.Data[4] = Slave[1].LTC6804[1].CellTemperature[0].CellTemp / 10 + 40;
  	BMS_Send.Data[5] = Slave[1].LTC6804[1].CellTemperature[1].CellTemp / 10 + 40;
  	BMS_Send.Data[6] = Slave[1].LTC6804[1].CellTemperature[2].CellTemp / 10 + 40;
  	BMS_Send.Data[7] = Slave[1].LTC6804[1].CellTemperature[3].CellTemp / 10 + 40;

	Can1_Send(&BMS_Send,0);
	BMS_Send.ID += 0x10;//0x6A3
	OSTimeDlyHMSM(0, 0, 0,10);			

	BMS_Send.Data[0] = Slave[2].LTC6804[0].CellTemperature[0].CellTemp / 10 + 40;
  	BMS_Send.Data[1] = Slave[2].LTC6804[0].CellTemperature[1].CellTemp / 10 + 40;
  	BMS_Send.Data[2] = Slave[2].LTC6804[0].CellTemperature[2].CellTemp / 10 + 40;
  	BMS_Send.Data[3] = Slave[2].LTC6804[0].CellTemperature[3].CellTemp / 10 + 40;
	BMS_Send.Data[4] = Slave[2].LTC6804[1].CellTemperature[0].CellTemp / 10 + 40;
  	BMS_Send.Data[5] = Slave[2].LTC6804[1].CellTemperature[1].CellTemp / 10 + 40;
  	BMS_Send.Data[6] = Slave[2].LTC6804[1].CellTemperature[2].CellTemp / 10 + 40;
  	BMS_Send.Data[7] = Slave[2].LTC6804[1].CellTemperature[3].CellTemp / 10 + 40;

	Can1_Send(&BMS_Send,0);
	BMS_Send.ID += 0x10; //0x6B3
	OSTimeDlyHMSM(0, 0, 0,10);			

	BMS_Send.Data[0] = Slave[3].LTC6804[0].CellTemperature[0].CellTemp / 10 + 40;
  	BMS_Send.Data[1] = Slave[3].LTC6804[0].CellTemperature[1].CellTemp / 10 + 40;
  	BMS_Send.Data[2] = Slave[3].LTC6804[0].CellTemperature[2].CellTemp / 10 + 40;
  	BMS_Send.Data[3] = Slave[3].LTC6804[0].CellTemperature[3].CellTemp / 10 + 40;
	BMS_Send.Data[4] = Slave[3].LTC6804[1].CellTemperature[0].CellTemp / 10 + 40;
  	BMS_Send.Data[5] = Slave[3].LTC6804[1].CellTemperature[1].CellTemp / 10 + 40;
  	BMS_Send.Data[6] = Slave[3].LTC6804[1].CellTemperature[2].CellTemp / 10 + 40;
  	BMS_Send.Data[7] = Slave[3].LTC6804[1].CellTemperature[3].CellTemp / 10 + 40;

	Can1_Send(&BMS_Send,0);
	BMS_Send.ID += 0x10;
	OSTimeDlyHMSM(0, 0, 0,10);			
}
#endif

