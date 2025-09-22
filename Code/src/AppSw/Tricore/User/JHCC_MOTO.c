/*
 * JHCC_MOTO.c
 *
 *  Created on: 2023��3��22��
 *      Author: ������
 */
#include "include.h"

#define MOTO_Freq           10000//10000 30000 50000 80000 100000  ��ͨ���10k��ѣ����ı���Ҫ��
#define MOTOR1_P            IfxGtm_ATOM0_6_TOUT42_P23_1_OUT   //����
#define MOTOR1_N            IfxGtm_ATOM0_4_TOUT50_P22_3_OUT   //��
#define MOTOR2_P            IfxGtm_ATOM0_2_TOUT33_P33_11_OUT   //����
#define MOTOR2_N            IfxGtm_ATOM0_5_TOUT40_P32_4_OUT   //�Ҹ�

//���PWM�޷�
#define MOTOR1_LIMIT_P      8500        //�Ҹ�
#define MOTOR1_LIMIT_N      8500        //����
#define MOTOR2_LIMIT_P      8500        //��
#define MOTOR2_LIMIT_N      8500        //����

short   speed_to_set;
int     test_duty=1000;
uint8   run_mode=0;         //0��ǰ����1������
uint8   lastrun_mode=0;
uint8   dontup=0;
uint16  Leftduty_p;         //�����������ռ�ձȣ���ת��
uint16  Leftduty_n;         //�����������ռ�ձȣ���ת��
uint16  Rightduty_p;        //�����������ռ�ձȣ���ת��
uint16  Rightduty_n;        //�����������ռ�ձȣ���ת��
sint16  Leftduty;           //��������
sint16  Rightduty;          //��������


void MOTO_init(void)
{


      ADC_InitConfig(ADC6, 80000); //��ʼ��adc�����ɼ�
      ADC_InitConfig(ADC7, 80000); //��ʼ��adc
      ATOM_PWM_InitConfig(MOTOR1_P, MOTO_Freq-1000 , MOTO_Freq);//���a  ph2
      ATOM_PWM_InitConfig(MOTOR2_P, MOTO_Freq-1000, MOTO_Freq);//���b   ph2
      ATOM_PWM_InitConfig(MOTOR1_N, MOTO_Freq , MOTO_Freq);//���a  ph1
      ATOM_PWM_InitConfig(MOTOR2_N, MOTO_Freq, MOTO_Freq);//���b   ph1
      /*��������ʼ��*/
      ATOM_PWM_SetDuty(MOTOR1_P, MOTO_Freq, MOTO_Freq);//�Ҹ�
      ATOM_PWM_SetDuty(MOTOR1_N, MOTO_Freq, MOTO_Freq);//����
      ATOM_PWM_SetDuty(MOTOR2_P, MOTO_Freq, MOTO_Freq); //����
      ATOM_PWM_SetDuty(MOTOR2_N, MOTO_Freq, MOTO_Freq); //��
      ENC_InitConfig(ENC6_InPut_P20_3, ENC6_Dir_P20_0);
      ENC_InitConfig(ENC5_InPut_P10_3, ENC5_Dir_P10_1);
      /*��ʱ��STM0, STM_Channel_1��ʼ��*/

      setSpeedTo(0,0,0);//��ʼ�ٶ��趨
      //STM_DisableInterrupt(STM1, STM_Channel_1);


}
//ǰ����ģʽѡ��
void Mode_Select(){
    lastrun_mode=run_mode;//����״̬
    if(!modechange_coolflag&&(g_Angle>=130||g_Angle<=-130)){//������ͷ
        run_mode=!run_mode;//ģʽ��ת������״̬
    }
}


/*
 * ռ�ձȵ���
 */
void updateDuty(void)
{
  //  if(!run_mode){//ǰ

//        Leftduty =speedDuty-biasDuty;
//        Rightduty=speedDuty+biasDuty;
//        if(Leftduty>9000){
//            Leftduty=9000;
//        }
//        if(Rightduty>9000){
//            Rightduty=9000;
//        }
//        MOTO_Set_duty();
//    }
//    else//�����������ַ�ת�任
//    {
//        Leftduty =-(speedDuty+open_bias_duty+biasDuty);
//        Rightduty=-(speedDuty-open_bias_duty-biasDuty);
//        MOTO_Set_duty();
//    }
        if(!run_mode){//ǰ
            Leftduty =speedDuty_left;
            Rightduty=speedDuty_right;

            MOTO_Set_duty();
        }
        else//�����������ַ�ת�任
        {
            Leftduty =-speedDuty_left;
            Rightduty=-speedDuty_right;
            MOTO_Set_duty();
        }

}


/*pwm���ú���
 *����ת����
 */
void MOTO_Set_duty( void)
{

        if(run_mode!=lastrun_mode){
            Leftduty=0;
            Rightduty=0;
        }

        //�������
        else
        {
        //��
        //��ռ�ձ�
        if (Leftduty >= 0) {
            if (Leftduty > MOTOR1_LIMIT_N) Leftduty_p = MOTOR1_LIMIT_N;
            else Leftduty_p = (unsigned short)(Leftduty);
            Leftduty_n = 0;
        }//��ռ�ձ�
        else {
            Leftduty_p = 0;
            if (-Leftduty > MOTOR1_LIMIT_N) Leftduty_n = MOTOR1_LIMIT_N;
            else Leftduty_n = (unsigned short)(-Leftduty);
        }
        //��
        //��ռ�ձ�
        if (Rightduty >= 0) {
            if (Rightduty > MOTOR2_LIMIT_N) Rightduty_p = MOTOR2_LIMIT_N;
            else Rightduty_p = (unsigned short)(Rightduty);
            Rightduty_n = 0;
        }//��ռ�ձ�
        else {
            Rightduty_p = 0;
            if (-Rightduty > MOTOR2_LIMIT_P) Rightduty_n = MOTOR2_LIMIT_N;
            else Rightduty_n = (unsigned short)(-Rightduty);
        }
        }
        ATOM_PWM_SetDuty(MOTOR1_P, MOTO_Freq-Leftduty_p, MOTO_Freq);//�Ҹ�
        ATOM_PWM_SetDuty(MOTOR1_N, MOTO_Freq-Leftduty_n, MOTO_Freq);//����
        ATOM_PWM_SetDuty(MOTOR2_P, MOTO_Freq-Rightduty_p, MOTO_Freq); //����
        ATOM_PWM_SetDuty(MOTOR2_N, MOTO_Freq-Rightduty_n, MOTO_Freq); //��

}
