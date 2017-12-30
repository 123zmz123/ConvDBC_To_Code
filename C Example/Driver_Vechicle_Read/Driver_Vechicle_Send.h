#ifndef __Driver_Vechicle_Send_H
#define __Driver_Vechicle_Send_H

#define BMSChrgMod_DCMode	0x02
#define BMSChrgMod_ACMode   0x01
#define BMSSwVers           11000  //BMS软件版本号 1.0.00.3  20171017
#define BMSHwVers           100    //BMS硬件版本号 1.00
#define BMSCalDataVers      0      //BMS标定数据版本号 暂无，发初始化值0
#define BMSManfctCode       222     //制造代码222

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
