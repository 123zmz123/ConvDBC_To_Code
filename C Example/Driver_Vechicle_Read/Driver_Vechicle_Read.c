//���������������CAN�ϵ���Ϣ����
#include <includes.h>
#include "SWintegration.h"

VCU_BMS_Struct VCU_BMS_Data; //VCU��BMS���͵����ݽ���
OBC_BMS_Struct OBC_BMS_Data; //OBC��BMS���͵����ݽ���

void VCU_BMS_110_Read(INT8U Data[])
{
	VCU_BMS_Data.VCUActHVPwrCnsExt = Data[0];  //���߾���0.1  �Ŵ�10����ֵ
	VCU_BMS_Data.VCUSOCDisp = (INT8U)(Data[1] >> 1);	  //���SOC�����ʾ
	VCU_BMS_Data.VCUVehSts = (*(INT16U*)(&Data[1]) & 0x01C0) >> 6;//����״̬
	VCU_BMS_Data.VCUBMSOperModReq = (Data[2] & 0x38) >> 3;//BMS����ģʽ����
	
	if((Data[3] & 0x80) > 0)
	{
		VCU_BMS_Data.VCUChrgAllwd = True;//�����������
	}
	else
	{
		VCU_BMS_Data.VCUChrgAllwd = False;
	}

	if((Data[3] & 0x40) > 0)
	{
		VCU_BMS_Data.VCUBattIntrHeatAllwd = True;//����ڲ�����/��ȴ���
	}
	else
	{
		VCU_BMS_Data.VCUBattIntrHeatAllwd = False;
	}

	if((Data[3] & 0x20) > 0)
	{
		VCU_BMS_Data.SRSCrashSts = True;//SRS��ײ״̬
	}
	else
	{
		VCU_BMS_Data.SRSCrashSts = False;		
	}

	if((Data[3] & 0x10) > 0)
	{
		VCU_BMS_Data.VCUBMSSOCEna = True;//���ʣ��SOC����
	}
	else
	{
		VCU_BMS_Data.VCUBMSSOCEna = False;		
	}

	if((Data[3] & 0x08) > 0)
	{
		VCU_BMS_Data.CHMAsignalSts = True;//A+�ź�״̬
	}
	else
	{
		VCU_BMS_Data.CHMAsignalSts = False;		
	}

	VCU_BMS_Data.VCUChrgIReqLimFac = (Data[3] & 0x06) >> 1;//��������������ϵ��
	VCU_BMS_Data.VCUSRSCrashOutpSts = (Data[4] & 0xF0) >> 4;// ��ײ����ź�	

	/*
	if((VCU_BMS_Data.VCUSRSCrashOutpSts > 0) || (VCU_BMS_Data.SRSCrashSts > 0))
	{
		Myself.ST_Break = 1;
		Enable_warring(DTC_001_CrashHappen);
		OSSemPost(Module_sem);
	}
	*/
	Myself.VCU_BMS_110_Timeout = 0;
	//Data[4]�ĺ���λ����ײУ���ź� ��Ҫ����
}

void OBC_BMS_311_Read(INT8U Data[])
{
	OBC_BMS_Data.OBCMaxChrgOutpUAvl = (*(INT16U*)(&Data[0]) & 0xFFF8) >> 3;//����������������ѹ  �Ŵ�10��
	OBC_BMS_Data.OBCMaxChrgOutpIAvl = (*(INT16U*)(&Data[1]) & 0x07FE) >> 1;//������������������  �Ŵ�10��
	OBC_BMS_Data.OBCMaxChrgOutpPwrAvl = ((*(INT32U*)(&Data[2]) & 0x01FF8000) >> 15) * 10;//������������������
	OBC_BMS_Data.OBCVehInletT = ((Data[4] & 0x7F) << 1) + ((Data[5] & 0x80) >> 7) - 40;//�����������¶�
	OBC_BMS_Data.OBCChrgSts = (Data[6] & 0x3C) >> 2;//��������״̬
	OBC_BMS_Data.PlugLockStatus = (Data[6] & 0x03);//���ǹ��ֹ״̬
	OBC_BMS_Data.OBCS2Stus = (Data[7] & 0x80) >> 7;//S2״̬
	Myself.OBC_BMS_311_Timeout = 0;
}

void OBC_BMS_321_Read(INT8U Data[])
{
	OBC_BMS_Data.OBCCPValue = Data[0];//CPռ�ձ�
	OBC_BMS_Data.CPSts = (BOOL)((Data[1] & 0x80) >> 7);//CP״̬
	Myself.OBC_BMS_321_Timeout = 0;
}

void OBC_BMS_331_Read(INT8U Data[])
{
	INT16U temp;
	OBC_BMS_Data.OBCChrgInpAcU = (INT16U)((*(INT16U*)(&Data[0]) & 0xFF80) >> 7);//����������ʵʱ�����ѹ
	OBC_BMS_Data.OBCChrgInpAcI = (INT16U)((*(INT16U*)(&Data[1]) & 0x7FE0) >> 5);//����������ʵʱ�������    �Ŵ�10��
	OBC_BMS_Data.OBCChrgDcU = (INT16U)(*(INT16U*)(&Data[2]) & 0x1FFF);//����ʵʱ����ѹ   �Ŵ�10��
	OBC_BMS_Data.OBCChrgDcI = (INT16U)((*(INT16U*)(&Data[4]) & 0xFFC0) >> 6);//����ʵʱ������  �Ŵ�10��

	temp = (INT16U)((*(INT16U*)(&Data[5]) & 0x3FC0) >> 6);
	OBC_BMS_Data.OBCT = (INT8S)(temp - 40);
	
	//C_BMS_Data.OBCT = ((Data[5] & 0x3F) << 2) + ((Data[6] & 0xC0) >> 6) - 40;//�����¶�
	
	OBC_BMS_Data.OBCCANErr = (BOOL)((Data[6] & 0x20) >> 5);//OBCͨ�Ź���
	OBC_BMS_Data.OBCHwErr = (BOOL)((Data[6] & 0x10) >> 4);//OBCӲ������
	OBC_BMS_Data.OBCInpUOver = (BOOL)((Data[6] & 0x08) >> 3);//OBC�����ѹ
	OBC_BMS_Data.OBCOutpUOver = (BOOL)((Data[6] & 0x04) >> 2);//OBC�����ѹ
	OBC_BMS_Data.OBCInpUUnder = (BOOL)((Data[6] & 0x02) >> 1);//OBC����Ƿѹ
	OBC_BMS_Data.OBCOutpUUnder = (BOOL)(Data[6] & 0x01);//OBC���Ƿѹ
	OBC_BMS_Data.OBCInpIOver = (BOOL)((Data[7] & 0x80) >> 7);//�������
	OBC_BMS_Data.OBCOutpIOver = (BOOL)((Data[7] & 0x40) >> 6);//�������
	OBC_BMS_Data.OBCTOvrErr = (BOOL)((Data[7] & 0x20) >> 5);//���¹���	
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
          	break; //������Ҫ�������can���մ���ļ�������
      	}
    }
}

void VCU_OBC_BMS_communicate_error(void)
{// 20ms ����һ��
	if(VCU_BMS_Data.Receied_VCU > 0)
	{
		Myself.VCU_BMS_110_Timeout++;// ��ʱΪ�����10������ 10*10ms 
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
			OBC_BMS_Data.OBCMaxChrgOutpUAvl = 0;//����������������ѹ   �Ŵ�10��
			OBC_BMS_Data.OBCMaxChrgOutpIAvl = 0;//������������������   �Ŵ�10��
			OBC_BMS_Data.OBCMaxChrgOutpPwrAvl = 0;//������������������
			OBC_BMS_Data.OBCVehInletT = 0;//�����������¶�
			OBC_BMS_Data.OBCChrgSts = 0;//��������״̬
			OBC_BMS_Data.PlugLockStatus = 0;//���ǹ��ֹ״̬
			OBC_BMS_Data.OBCS2Stus = 0;//S2״̬
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
			OBC_BMS_Data.OBCChrgInpAcU = 0;//����������ʵʱ�����ѹ
			OBC_BMS_Data.OBCChrgInpAcI = 0;//����������ʵʱ�������   �Ŵ�10��
			OBC_BMS_Data.OBCChrgDcU = 0;//����ʵʱ����ѹ    �Ŵ�10��
			OBC_BMS_Data.OBCChrgDcI = 0;//����ʵʱ������   �Ŵ�10��
			OBC_BMS_Data.OBCT = 0;//�����¶�
			OBC_BMS_Data.OBCCANErr = 0;//OBCͨ�Ź���
			OBC_BMS_Data.OBCHwErr = 0;//OBCӲ������
			OBC_BMS_Data.OBCInpUOver = 0;//OBC�����ѹ
			OBC_BMS_Data.OBCOutpUOver = 0;//OBC�����ѹ
			OBC_BMS_Data.OBCInpUUnder = 0;//OBC����Ƿѹ
			OBC_BMS_Data.OBCOutpUUnder = 0;//OBC���Ƿѹ
			OBC_BMS_Data.OBCInpIOver = 0;//�������
			OBC_BMS_Data.OBCOutpIOver = 0;//�������
			OBC_BMS_Data.OBCTOvrErr = 0;//���¹���
			Enable_warring(DTC_066_CntrCANErr4);
		}
	}
}

