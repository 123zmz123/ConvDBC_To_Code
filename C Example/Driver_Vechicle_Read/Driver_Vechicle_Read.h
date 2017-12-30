#ifndef __Driver_Vechicle_Read_H
#define __Driver_Vechicle_Read_H
//����״̬
#define VCUVehSts_0_Standstill	0//"0x0:��ֹ
#define VCUVehSts_1_EVDrive   	1//0x1:�������
#define VCUVehSts_2_Regeneration 2	//0x2:�������գ����ƶ��������������պϲ�Ϊ�������գ�
#define VCUVehSts_3_Charge		3//0x3: �����
#define VCUVehSts_4_Failure		4//0x4: ����״̬0x5~7: Ԥ��"	S	R
//BMS����ģʽ����														
#define VCUBMSOperModReq_0_Initializing	0
#define VCUBMSOperModReq_1_Standby		1
#define VCUBMSOperModReq_2_PreCharge	2
#define VCUBMSOperModReq_3_HVActive		3
#define VCUBMSOperModReq_4_Powerdown 	4
#define VCUBMSOperModReq_5_Failure		5											

//����ṹ���Ƕ�VCU ���͸� BMS�����ݽ��л���
typedef struct
{
	INT8U VCUActHVPwrCnsExt;//�ⲿ���ĸ�ѹ����
	INT8U VCUSOCDisp;		  //���SOC�����ʾ
	INT8U VCUVehSts;//����״̬
	INT8U VCUBMSOperModReq;//BMS����ģʽ����
	BOOL VCUChrgAllwd;//�����������
	BOOL VCUBattIntrHeatAllwd;//����ڲ�����/��ȴ���
	BOOL SRSCrashSts;//SRS��ײ״̬
	BOOL VCUBMSSOCEna;//���ʣ��SOC����
	BOOL CHMAsignalSts;//A+�ź�״̬
	INT8U VCUChrgIReqLimFac;//��������������ϵ��
	INT8U VCUSRSCrashOutpSts;// ��ײ����ź� ���Ը���Ϊbool����
	INT8U Receied_VCU;//�յ�VCU�ĵ�һ֡��Ϣ
//VCUSRSCrashOutpStsChkS;
//VCUChrgSspdReq  �����ͣ�����źţ�Ԥ����
//MilgChrgMode  ��̳��ģʽ��Ԥ����
}VCU_BMS_Struct;

typedef struct
{
	//0x311
	INT16U OBCMaxChrgOutpUAvl;//����������������ѹ   �Ŵ�10��
	INT16U OBCMaxChrgOutpIAvl;//������������������   �Ŵ�10��
	INT16U OBCMaxChrgOutpPwrAvl;//������������������
	INT8S OBCVehInletT;//�����������¶�
	INT8U OBCChrgSts;//��������״̬
	INT8U PlugLockStatus;//���ǹ��ֹ״̬
	BOOL OBCS2Stus;//S2״̬

	//0x321
	INT8U OBCCPValue;//CPռ�ձ�
	BOOL CPSts;//CP״̬

	//0x331
	INT16U OBCChrgInpAcU;//����������ʵʱ�����ѹ
	INT16U OBCChrgInpAcI;//����������ʵʱ�������   �Ŵ�10��
	INT16U OBCChrgDcU;//����ʵʱ����ѹ    �Ŵ�10��
	INT16U OBCChrgDcI;//����ʵʱ������   �Ŵ�10��
	INT8S OBCT;//�����¶�
	BOOL OBCCANErr;//OBCͨ�Ź���
	BOOL OBCHwErr;//OBCӲ������
	BOOL OBCInpUOver;//OBC�����ѹ
	BOOL OBCOutpUOver;//OBC�����ѹ
	BOOL OBCInpUUnder;//OBC����Ƿѹ
	BOOL OBCOutpUUnder;//OBC���Ƿѹ
	BOOL OBCInpIOver;//�������
	BOOL OBCOutpIOver;//�������
	BOOL OBCTOvrErr;//���¹���
}OBC_BMS_Struct;

extern VCU_BMS_Struct VCU_BMS_Data; //VCU��BMS���͵����ݽ���
extern OBC_BMS_Struct OBC_BMS_Data; //OBC��BMS���͵����ݽ���
extern void VCU_OBC_BMS_communicate_error(void);

#endif
