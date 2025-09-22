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
 * JHCC_control_current.c
 *
 *  Created on: 2023��4��21��
 *      Author:  �²�
 */
#include <include.h>

//-----------------------------�궨����-----------------------------
#define CURRENT_INT_SATU1   7000           //�����޷�
#define CURRENT_INT_SATU2   7000           //�����޷�
#define CURRENT_PWM_MAX1    8000          //�������޷�
#define CURRENT_PWM_MIN1    8000          //�������޷�
#define CURRENT_PWM_MAX2    8000          //�������޷�
#define CURRENT_PWM_MIN2    8000          //�������޷�
#define SIZE_TABLE_FDFWD    11             //ǰ�������С

//-----------------------------����������-----------------------------
//������PID����
float   currentKP1          = 3.0;        //������KP
float   currentKD1          = 1;           //������KD
float   currentKI1          = 2.0;         //������KI
float   currentKP2          = 3.0;         //������KP
float   currentKD2          = 1;           //������KD
float   currentKI2          =2.0;         //������KI

//������ȡ
sint16  currentData1        = 0;           //�����ɼ���
sint16  currentData2        = 0;           //�����ɼ���
sint16  currentData3        = 0;           //�����ɼ���
sint16  currentData4        = 0;           //�����ɼ���
sint16  currentActual1      = 0;           //��������λmA��
sint16  currentActual2      = 0;           //��������λmA��
uint16  currentNoload1      = 0;           //���ֿ��ص���
uint16  currentNoload2      = 0;           //���ֿ��ص���

//�������Ʊ���
sint16  currentDesired1     = 0;           //��������
sint16  currentDesired2     = 0;           //��������
sint16  currentErr1         = 0;           //�������
sint16  currentErr2         = 0;           //�������
sint16  currentErrLast1     = 0;           //��һʱ�̵������
sint16  currentErrLast2     = 0;           //��һʱ�̵������
sint16  currentErrLastLast1 = 0;           //����һʱ�̵������
sint16  currentErrLastLast2 = 0;           //����һʱ�̵������
sint16  currentErrDiff1     = 0;           //�������΢��
sint16  currentErrDiff2     = 0;           //�������΢��
sint16  currentErrDiff11    = 0;           //�������΢��
sint16  currentErrDiff22    = 0;           //�������΢��
sint16  currentErrAccm1     = 0;           //����������
sint16  currentErrAccm2     = 0;           //����������
sint16  currentDuty1        = 0;           //���������
sint16  currentDuty2        = 0;           //���������
uint8   dontCurrentPID      = 0;           //1��ֹ��������0����������
uint8   flagRead            = 1;           //������ȡ��־λ
uint8   ReadEnd             = 1;           //0������ȡ������1������ȡ��ʼ

//�˲�����
sint16     value1[5]= {0, 0, 0, 0, 0};
sint16     value2[5]= {0, 0, 0, 0, 0};
sint16     i                      ;

//������ֵ��ȡ
uint32  sum1                = 0;
uint32  sum2                = 0;
uint16  currentMetro        = 0;
uint8   dontCurrentRead     = 0;        //1��ֹ��ȡ������0����������ȡ

//������ǰ��
//ǰ�����ƣ�ռ�ձȶ�Ӧ�ĵ���
sint16 Current_Thr_Fdfwd[SIZE_TABLE_FDFWD] = {0};
//ǰ�����ƣ�������Ӧ��ռ�ձ�
sint16 Current_Out_Fdfwd[SIZE_TABLE_FDFWD] = {0};

//-----------------------------������-----------------------------
//������ȡ
void currentRead(void)
{
    static uint8 i=0;
   // ANO_DT_send_int16_sound(currentData1,currentData2,1,1);
    currentData1 = ADC_Read(ADC6);//����2100
    currentData2 = ADC_Read(ADC7);//����2100
    currentActual1 = filter1(value1, currentData1, sizeof(value1)/sizeof(sint16));
    currentActual2 = filter2(value2, currentData2, sizeof(value2)/sizeof(sint16));
    i++;
}
//ƽ���˲�
sint16 filter1(sint16 value_buf[], sint16 new_value, sint16 num)
{
    static sint16 i;
    sint16 count;
    int sum = 0;

    value_buf[i++] = new_value;
    if (i == num)
        i = 0;
    for (count=0; count<num; count++)
        sum += value_buf[count];

    return (sint16)(sum/num);
}

//ƽ���˲�
sint16 filter2(sint16 value_buf[], sint16 new_value, sint16 num)
{
    static sint16 i;
    sint16 count;
    int sum = 0;

    value_buf[i++] = new_value;
    if (i == num)
        i = 0;
    for (count=0; count<num; count++)
        sum += value_buf[count];

    return (sint16)(sum/num);
}

//������������
void setCurrentTo(sint16 target1, sint16 target2)
{
    if(target1>1000)target1=1000;
    if(target2>1000)target2=1000;
    if(target1<0)target1=0;
    if(target2<0)target2=0;
    currentDesired1 = target1;
    currentDesired2 = target2;

}

//������������ʽPID
/*
void currentPID(void)
{
    //���������
    currentErr1 = currentDesired1 - currentActual1;
    currentErrDiff1 = currentErr1 - currentErrLast1;
    currentErrDiff11 = currentErr1 - 2 * currentErrLast1 + currentErrLastLast1;

    currentErr2 = currentDesired2 - currentActual2;
    currentErrDiff2 = currentErr2 - currentErrLast2;
    currentErrDiff22 = currentErr2 - 2 * currentErrLast2 + currentErrLastLast2;

    currentErrAccm1 += currentErr1 * currentKI1;
    if (currentErrAccm1 > 3000)currentErrAccm1 = 3000;
    currentDuty1 = currentErrAccm1 + currentErrDiff1*currentKP1 + currentErrDiff11*currentKD1;
    //�޷�
    if (currentDuty1 > 4000)currentDuty1 = 4000;//////////////////////////////////
    else if (currentDuty1 < 0)currentDuty1 = 0;//////////////////////////////////
    currentErrLastLast1 = currentErrLast1;
    currentErrLast1 = currentErr1;

}

*/
//λ��ʽpid
void currentPID(void)
{
    //���������
      currentErr1 = currentDesired1 - currentActual1;
      currentErrDiff1 = currentErr1 - currentErrLast1;
      currentErrDiff11 = currentErr1 - 2 * currentErrLast1 + currentErrLastLast1;

      currentErr2 = currentDesired2 - currentActual2;
      currentErrDiff2 = currentErr2 - currentErrLast2;
      currentErrDiff22 = currentErr2 - 2 * currentErrLast2 + currentErrLastLast2;

      if (dontCurrentPID == 0) {
          if(currentErr1 > 10 || currentErr1 < -10)
          {
              currentErrAccm1 += currentErr1 * currentKI1;
              if (currentErrAccm1 > CURRENT_INT_SATU1) currentErrAccm1 = CURRENT_INT_SATU1;
              else if(currentErrAccm1 < -CURRENT_INT_SATU1) currentErrAccm1 = -CURRENT_INT_SATU1;
              currentDuty1 = currentErrAccm1 + currentErrDiff1*currentKP1 + currentErrDiff11*currentKD1;
          }
          if(currentErr2 > 10 || currentErr2 < -10)
          {
              currentErrAccm2 += currentErr2 * currentKI2;
              if (currentErrAccm2 > CURRENT_INT_SATU2) currentErrAccm2 = CURRENT_INT_SATU2;
              else if(currentErrAccm2 < -CURRENT_INT_SATU2) currentErrAccm2 = -CURRENT_INT_SATU2;
              currentDuty2 = currentErrAccm2 + currentErrDiff2*currentKP2 + currentErrDiff22*currentKD2;
          }
      }

      //�޷�
      if (currentDuty1 > CURRENT_PWM_MAX1)currentDuty1 = CURRENT_PWM_MAX1;//////////////////////////////////
      else if (currentDuty1 < -CURRENT_PWM_MIN1)currentDuty1 = -CURRENT_PWM_MIN1;//////////////////////////////////

      if (currentDuty2 > CURRENT_PWM_MAX2)currentDuty2 = CURRENT_PWM_MAX2;//////////////////////////////////
      else if (currentDuty2 < -CURRENT_PWM_MIN2)currentDuty2 = -CURRENT_PWM_MIN2;//////////////////////////////////

      currentErrLastLast1 = currentErrLast1;
      currentErrLast1 = currentErr1;

      currentErrLastLast2 = currentErrLast2;
      currentErrLast2 = currentErr2;

}



