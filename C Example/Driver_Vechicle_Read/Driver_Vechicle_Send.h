#ifndef __Driver_Vechicle_Send_H
#define __Driver_Vechicle_Send_H

#define BMSChrgMod_DCMode	0x02
#define BMSChrgMod_ACMode   0x01
#define BMSSwVers           11000  //BMS����汾�� 1.0.00.3  20171017
#define BMSHwVers           100    //BMSӲ���汾�� 1.00
#define BMSCalDataVers      0      //BMS�궨���ݰ汾�� ���ޣ�����ʼ��ֵ0
#define BMSManfctCode       222     //�������222

extern void BMS_VCU_113_send (void);
extern void BMS_VCU_213_send (void);
extern void BMS_VCU_233_send (void);
extern void BMS_OBC_303_send (void);
extern void BMS_VCU_313_send (void);
extern void BMS_VCU_323_send (void);
extern void BMS_VCU_413_send (void);
extern void BMS_VCU_513_send (void);
extern void BMS_GW_603_send (void);
extern void BMS_GW_613_send(void);
extern void BMS_GW_623_send(void);
extern void BMS_GW_633_send (void);
extern void BMS_GW_626_send(void);
extern void BMS_GW_627_send(void);
extern void BMS_GW_62B_send(void);
extern void BMS_GW_643_send(void);
extern void BMS_GW_660_678_send(void);
//extern void BMS_GW_683_6B3_send(void);
extern void BMS_GW_683_send (void);
extern void BMS_GW_693_send (void);
extern void BMS_GW_6A3_send (void);    
#endif
