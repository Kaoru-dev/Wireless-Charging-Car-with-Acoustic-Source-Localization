/*
 * JHCC_control_dir.h
 *
 *  Created on: 2023��3��26��
 *      Author: ������
 */

#ifndef SRC_APPSW_TRICORE_USER_JHCC_CONTROL_DIR_H_
#define SRC_APPSW_TRICORE_USER_JHCC_CONTROL_DIR_H_
extern sint16 biasIndBigFastP ;
extern sint16 biasIndBigFastD  ;
extern float  biasIndBigFastPD  ;
extern sint16  open_bias_duty;
//���򻷱���
extern uint8  dontBiasPID ;      //0�������򻷣�1���η���
extern uint8  dontBiasPID1 ;      //0�������򻷣�1���η���
extern float biasSoundErr ;      //����ͷƫ�ƶ�ƫ����(����Ǹ����ҹ�����)
extern  float  biasSoundDifferential;   //������ֵ�Ĳ�֣�����΢��
extern float  biasSoundErrLast ;      //ǰһ������ƫ����
extern sint16 biasDuty ;      //���򻷵�ռ�ձ�
extern float icmDuty;
extern  float   icmErr;
void biasPID_low(void);
void biasPID(void);
void speedbiasPID(void);
void open_bias(void);
void open_speed(void);
#endif /* SRC_APPSW_TRICORE_USER_JHCC_CONTROL_DIR_H_ */
