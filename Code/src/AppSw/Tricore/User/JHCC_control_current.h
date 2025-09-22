/*������������������������������������������������������������������������
�汾��
    2022.4.22
���빦�ܣ�
        ������
��Ȩ��Ϣ��
        �²�������ƽ�����㺭
        �Լ����и���֧�ֵĻ����˾��ֲ���ǰ���ǣ�����
������������������������������������������������������������������������*/
/*
 * JHCC_control_current.h
 *
 *  Created on: 2023��4��21��
 *      Author: �²�
 */

#ifndef SRC_APPSW_TRICORE_USER_JHCC_CONTROL_CURRENT_H_
#define SRC_APPSW_TRICORE_USER_JHCC_CONTROL_CURRENT_H_


//-----------------------------ͷ�ļ���-----------------------------
#include <include.h>

//-----------------------------�궨����-----------------------------


//-----------------------------������-----------------------------
void    currentRead     (void);
void    setCurrentTo    (sint16, sint16);
void    currentPID      (void);
sint16  filter1         (sint16 value_buf[], sint16 new_value, sint16 num);
sint16  filter2         (sint16 value_buf[], sint16 new_value, sint16 num);

//-----------------------------����������-----------------------------
extern sint16   currentDuty1;
extern sint16   currentDuty2;
extern sint16  currentDesired1;
extern sint16  currentDesired2;
extern sint16  currentData1;       //�����ɼ���
extern sint16  currentData2;       //�����ɼ���
extern sint16   currentActual1;     //��������λmA��
extern sint16   currentActual2;     //��������λmA��
extern float    currentKP1;         //������KP
extern float    currentKD1;         //������KD
extern float    currentKI1;         //������KI
extern float    currentKP2;         //������KP
extern float    currentKD2;         //������KD
extern float    currentKI2;         //������KI
extern uint16   currentMetro;
extern sint16   currentErr1;
extern sint16   currentErr2;
extern uint8    dontCurrentPID;     //1��ֹ��������0����������
extern sint16  currentErrAccm1;
extern sint16  currentErrDiff1  ;

#endif /* SRC_APPSW_TRICORE_USER_JHCC_CONTROL_CURRENT_H_ */
