/*
 * JHCC_MOTO.c
 *
 *  Created on: 2023年3月22日
 *      Author: 君毋语
 */
#include "include.h"

#define MOTO_Freq           10000//10000 30000 50000 80000 100000  普通电机10k最佳，空心杯需要调
#define MOTOR1_P            IfxGtm_ATOM0_6_TOUT42_P23_1_OUT   //左正
#define MOTOR1_N            IfxGtm_ATOM0_4_TOUT50_P22_3_OUT   //左负
#define MOTOR2_P            IfxGtm_ATOM0_2_TOUT33_P33_11_OUT   //右正
#define MOTOR2_N            IfxGtm_ATOM0_5_TOUT40_P32_4_OUT   //右负

//电机PWM限幅
#define MOTOR1_LIMIT_P      8500        //右负
#define MOTOR1_LIMIT_N      8500        //右正
#define MOTOR2_LIMIT_P      8500        //左负
#define MOTOR2_LIMIT_N      8500        //左正

short   speed_to_set;
int     test_duty=1000;
uint8   run_mode=0;         //0：前驱，1：后驱
uint8   lastrun_mode=0;
uint8   dontup=0;
uint16  Leftduty_p;         //最终左轮输出占空比（正转）
uint16  Leftduty_n;         //最终左轮输出占空比（反转）
uint16  Rightduty_p;        //最终右轮输出占空比（正转）
uint16  Rightduty_n;        //最终右轮输出占空比（反转）
sint16  Leftduty;           //最终左轮
sint16  Rightduty;          //最终右轮


void MOTO_init(void)
{


      ADC_InitConfig(ADC6, 80000); //初始化adc电流采集
      ADC_InitConfig(ADC7, 80000); //初始化adc
      ATOM_PWM_InitConfig(MOTOR1_P, MOTO_Freq-1000 , MOTO_Freq);//电机a  ph2
      ATOM_PWM_InitConfig(MOTOR2_P, MOTO_Freq-1000, MOTO_Freq);//电机b   ph2
      ATOM_PWM_InitConfig(MOTOR1_N, MOTO_Freq , MOTO_Freq);//电机a  ph1
      ATOM_PWM_InitConfig(MOTOR2_N, MOTO_Freq, MOTO_Freq);//电机b   ph1
      /*编码器初始化*/
      ATOM_PWM_SetDuty(MOTOR1_P, MOTO_Freq, MOTO_Freq);//右负
      ATOM_PWM_SetDuty(MOTOR1_N, MOTO_Freq, MOTO_Freq);//右正
      ATOM_PWM_SetDuty(MOTOR2_P, MOTO_Freq, MOTO_Freq); //左正
      ATOM_PWM_SetDuty(MOTOR2_N, MOTO_Freq, MOTO_Freq); //左负
      ENC_InitConfig(ENC6_InPut_P20_3, ENC6_Dir_P20_0);
      ENC_InitConfig(ENC5_InPut_P10_3, ENC5_Dir_P10_1);
      /*定时器STM0, STM_Channel_1初始化*/

      setSpeedTo(0,0,0);//初始速度设定
      //STM_DisableInterrupt(STM1, STM_Channel_1);


}
//前后驱模式选择
void Mode_Select(){
    lastrun_mode=run_mode;//保存状态
    if(!modechange_coolflag&&(g_Angle>=130||g_Angle<=-130)){//开启掉头
        run_mode=!run_mode;//模式反转，更新状态
    }
}


/*
 * 占空比叠加
 */
void updateDuty(void)
{
  //  if(!run_mode){//前

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
//    else//后驱：左右轮反转变换
//    {
//        Leftduty =-(speedDuty+open_bias_duty+biasDuty);
//        Rightduty=-(speedDuty-open_bias_duty-biasDuty);
//        MOTO_Set_duty();
//    }
        if(!run_mode){//前
            Leftduty =speedDuty_left;
            Rightduty=speedDuty_right;

            MOTO_Set_duty();
        }
        else//后驱：左右轮反转变换
        {
            Leftduty =-speedDuty_left;
            Rightduty=-speedDuty_right;
            MOTO_Set_duty();
        }

}


/*pwm设置函数
 *正反转控制
 */
void MOTO_Set_duty( void)
{

        if(run_mode!=lastrun_mode){
            Leftduty=0;
            Rightduty=0;
        }

        //允许叠加
        else
        {
        //左
        //正占空比
        if (Leftduty >= 0) {
            if (Leftduty > MOTOR1_LIMIT_N) Leftduty_p = MOTOR1_LIMIT_N;
            else Leftduty_p = (unsigned short)(Leftduty);
            Leftduty_n = 0;
        }//负占空比
        else {
            Leftduty_p = 0;
            if (-Leftduty > MOTOR1_LIMIT_N) Leftduty_n = MOTOR1_LIMIT_N;
            else Leftduty_n = (unsigned short)(-Leftduty);
        }
        //右
        //正占空比
        if (Rightduty >= 0) {
            if (Rightduty > MOTOR2_LIMIT_N) Rightduty_p = MOTOR2_LIMIT_N;
            else Rightduty_p = (unsigned short)(Rightduty);
            Rightduty_n = 0;
        }//负占空比
        else {
            Rightduty_p = 0;
            if (-Rightduty > MOTOR2_LIMIT_P) Rightduty_n = MOTOR2_LIMIT_N;
            else Rightduty_n = (unsigned short)(-Rightduty);
        }
        }
        ATOM_PWM_SetDuty(MOTOR1_P, MOTO_Freq-Leftduty_p, MOTO_Freq);//右负
        ATOM_PWM_SetDuty(MOTOR1_N, MOTO_Freq-Leftduty_n, MOTO_Freq);//右正
        ATOM_PWM_SetDuty(MOTOR2_P, MOTO_Freq-Rightduty_p, MOTO_Freq); //左正
        ATOM_PWM_SetDuty(MOTOR2_N, MOTO_Freq-Rightduty_n, MOTO_Freq); //左负

}
