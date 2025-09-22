/*
 * JHCC_control.c
 *
 *  Created on: 2023��3��24��
 *      Author: ������
 */
//-----------------------------ͷ�ļ���-----------------------------
#include <include.h>

//-----------------------------�궨����-----------------------------
#define INT_SATURATION      500     //���ٶ����Ļ��ֱ���
//-----------------------------����������-----------------------------
//�ٶȻ�PID����50,95,2��
float   speedGiaP           = 100;
float   speedGiaD           = 4;
float   speedGiaI           = 30;

float   speedSmaP           = 40;
float   speedSmaD           = 0;
float   speedSmaI           = 0;

//�ٶȻ�����
uint8   dontSpeedPID        = 0;    //0�����ٶȻ���1�����ٶȻ�
sint16      speedErr;
sint16  speedDifferential;
sint16 speedErrLast;
sint16 speedDifferential2;
sint16 speedErrLastLast;
sint16  speedDutyAccI;
sint16 speedDutyAccPD;
sint16  speedDuty;

sint16   speedDuty_left;
sint16   speedDuty_right;
sint16  speedErr_left       = 0;    //��ƫ����
sint16  speedErr_right       = 0;   //��ƫ����
sint16  speedDifferential_left   = 0;    //ƫ����������
sint16  speedDifferential2_left  = 0;    //ƫ�������׺�����
sint16  speedDifferential_right   = 0;    //ƫ����������
sint16  speedDifferential2_right  = 0;    //ƫ�������׺�����
sint16  speedDuty_left           = 0;    //�ٶȻ���ռ�ձ�
sint16  speedDuty_right           = 0;    //�ٶȻ���ռ�ձ�
sint16  speedDutyAccI_left       = 0;    //�ٶȻ�I�ۼ�
sint16  speedDutyAccPD_left      = 0;    //�ٶȻ�P��D�ۼ�
sint16  speedDutyAccI_right      = 0;    //�ٶȻ�I�ۼ�
sint16  speedDutyAccPD_right      = 0;    //�ٶȻ�P��D�ۼ�
sint16  speedErrLast_left        = 0;    //ǰһ��ƫ����
sint16  speedErrLast_right        = 0;    //ǰһ��ƫ����
sint16  speedErrLastLast_left    = 0;    //����һ��ƫ����
sint16  speedErrLastLast_right    = 0;    //����һ��ƫ����

//�ٶȶ�ȡ
sint16  speedSet=40;                   //�趨�ٶ�
sint16  speedSet_left;              //�����趨�ٶ�
sint16  speedSet_right;             //�����趨�ٶ�
sint16  speedActualLeft;            //ʵ�������ٶ�
sint16  speedActualRight;           //ʵ�������ٶ�
sint16  speedActualx2;              //ʵ���ٶ�
sint32  speedSumx2;                 //�ٶ��ۼӣ�������
uint8   signleft = 1;               //����ת������
uint8   signRight = 1;              //����ת������


short speed_forward=25;
short speed_diff=0;
//-----------------------------������-----------------------------

//�ٶ�����ֵ����
void setSpeedTo(short target_left,short target_right,short target)
{
   // speedSet_left = target_left;
    speedSet_left = target_left;
    speedSet_right=target_right;
    speedSet=target;
}

//��ȡ�ٶ�
void speedRead(void)
{
    if(!run_mode){
    //��ñ���������ENC6_InPut_P20_3
    speedActualRight  = ENC_GetCounter(ENC5_InPut_P10_3);
    speedActualLeft = -ENC_GetCounter(ENC6_InPut_P20_3);
    //��ӵõ�С�����ĵ��ٶȵ�����
    speedActualx2 = speedActualLeft + speedActualRight;
    }
    else{
    speedActualRight  = -ENC_GetCounter(ENC5_InPut_P10_3);
    speedActualLeft = +ENC_GetCounter(ENC6_InPut_P20_3);
           //��ӵõ�С�����ĵ��ٶȵ�����
    speedActualx2 = speedActualLeft + speedActualRight;
    }
}

//�ٶȻ�������ʽPID

void speedPID(void) {
    //���������
    speedErr = speedSet - speedActualx2;
    speedDifferential = speedErr - speedErrLast;
    speedDifferential2 = speedErr - 2 * speedErrLast + speedErrLastLast;
        speedDutyAccI += speedGiaI * speedErr;
        if (speedDutyAccI > 7000) speedDutyAccI = 7000;
        else if(speedDutyAccI < -7000) speedDutyAccI = -7000;
        //if(speedActualx2<1)speedDutyAccI=0;

        speedDutyAccPD += speedGiaP * speedDifferential + speedGiaD * speedDifferential2;

        speedDuty = speedDutyAccI + speedDutyAccPD;


    //�޷�
    if (speedDuty > 8700)speedDuty = 8700;
    else if (speedDuty < -8700)speedDuty = -8700;
    speedErrLastLast = speedErrLast;
    speedErrLast = speedErr;
//    if(buff_flag){//����任���ֵ���
//        speedErr=0;
//        speedDifferential=0;
//        speedDifferential2=0;
//        speedDutyAccI=0;
//    }
}

////�����ٶȻ�pid
void speedPID_left(void) {
    //���������
    speedErr_left = speedSet_left - speedActualLeft;
    speedDifferential_left = speedErr_left - speedErrLast_left;
    speedDifferential2_left = speedErr_left - 2 * speedErrLast_left + speedErrLastLast_left;
        speedDutyAccI_left += speedGiaI * speedErr_left;
        if (speedDutyAccI_left > 8700) speedDutyAccI_left = 8700;
        else if(speedDutyAccI_left < -8700) speedDutyAccI_left = -8700;
        //if(speedActualx2<1)speedDutyAccI=0;
        speedDutyAccPD_left += speedGiaP * speedDifferential_left + speedGiaD * speedDifferential2_left;
        speedDuty_left = speedDutyAccI_left + speedDutyAccPD_left;
    //�޷�
    if (speedDuty_left > 8700)speedDuty_left = 8700;
    else if (speedDuty_left < -8700)speedDuty_left = -8700;
    speedErrLastLast_left = speedErrLast_left;
    speedErrLast_left = speedErr_left;
        if(buff_flag){//����任���ֵ���
          speedErr_left=0;
          speedDifferential_left=0;
          speedDifferential2_left=0;
          speedDutyAccI_left=0;
        }
}
//�����ٶȻ�
void speedPID_right(void) {
    //���������
    speedErr_right = speedSet_right - speedActualRight;
    speedDifferential_right = speedErr_right - speedErrLast_right;
    speedDifferential2_right = speedErr_right - 2 * speedErrLast_right + speedErrLastLast_right;
        speedDutyAccI_right += speedGiaI * speedErr_right;
        if (speedDutyAccI_right > 8700) speedDutyAccI_right = 8700;
        else if(speedDutyAccI_right < -8700) speedDutyAccI_right = -8700;
        //if(speedActualx2<1)speedDutyAccI=0;
        speedDutyAccPD_right += speedGiaP * speedDifferential_right + speedGiaD * speedDifferential2_right;
        speedDuty_right = speedDutyAccI_right+ speedDutyAccPD_right;
    //�޷�
    if (speedDuty_right > 8700)speedDuty_right = 8700;
    else if (speedDuty_right < -8700)speedDuty_right = -8700;
    speedErrLastLast_right = speedErrLast_right;
    speedErrLast_right = speedErr_right;
    if(buff_flag){//����任���ֵ���
        speedErr_right=0;
        speedDifferential_right=0;
        speedDifferential2_right=0;
        speedDutyAccI_right=0;
    }
}


