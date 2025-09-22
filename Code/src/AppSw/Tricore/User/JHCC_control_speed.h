/*
 * JHCC_control.h
 *
 *  Created on: 2023��3��24��
 *      Author: ������
 */

#ifndef SRC_APPSW_TRICORE_USER_JHCC_CONTROL_SPEED_H_
#define SRC_APPSW_TRICORE_USER_JHCC_CONTROL_SPEED_H_
//-----------------------------�궨����-----------------------------


//-----------------------------������-----------------------------
void    speedRead       (void);
void setSpeedTo(short target_left,short target_right,short target);
void    speedPID        (void);
void    delayMilimeter  (sint32);
void speedPID_left(void) ;
void speedPID_right(void) ;

//-----------------------------����������-----------------------------
extern sint16  speedDuty;
extern sint16   speedActualx2;
extern uint8    dontSpeedPID;               //0�����ٶȻ���1�����ٶȻ�
extern sint16  speedSet_left;              //�����趨�ٶ�
extern sint16  speedSet_right;             //�����趨�ٶ�
extern sint16   speedDuty_left;                  //�ٶȻ����
extern sint16   speedDuty_right;                  //�ٶȻ����
extern sint16   speedActualLeft;            //ʵ�������ٶ�
extern sint16   speedActualRight;           //ʵ�������ٶ�
extern uint8    signleft;                   //����ת������
extern uint8    signRight;                  //����ת������
extern sint16  speedSet;
extern float    speedGiaP;
extern float    speedGiaD;
extern float    speedGiaI;
extern short    speed_forward;//�ٶȻ�׼
extern short    speed_diff;//�ٶȲ���
extern float    speedSmaP;
extern float    speedSmaD;
extern float    speedSmaI;





#endif /* SRC_APPSW_TRICORE_USER_JHCC_CONTROL_SPEED_H_ */
