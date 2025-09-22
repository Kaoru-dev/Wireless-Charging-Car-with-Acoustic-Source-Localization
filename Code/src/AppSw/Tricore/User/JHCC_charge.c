/*
 * JHCC_charge.c
 *
 *  Created on: 2023年5月14日
 *      Author: Administrator
 */
/*判断是否需要充电与发车*/
#include "include.h"
#include "FFT_xcorr.h"
// 可修改变量
#define CHARGE_C                12               // 超级电容容量
              // 充电功率
#define CHARGE_V                13.0              // 充电电压
#define CHARGE_INTERRUPT_TIM    0.02            // 中断时间
uint8   CHARGE_P_INIT=50;                       // 充电功率
double Value_i1;   //输入电流值
double Value_v1;   //输入电压值
double Value_v2;   //输出电压值
double last_Value_v2;
double last_Value_i1;
double last_Value_v1;
int time_charge=0;
short charge_power;
double battery;//电容电量
short start_judge=1500;//发车电量判断量
short stop_judge=1000;//
uint8 start_flag=1;
uint8 charg_flag=1;
short charge_filt_1[10],charge_filt_2[10],charge_filt_3[10];//滤波数组
uint8 power_flag=0;
uint8 turn_off_flag=0;
short turn_off_time;//灭灯节拍器
void charge_init()
{
    ADC_InitConfig(ADC5, 80000); //初始化adc
    ADC_InitConfig(ADC11, 80000); //初始化adc  rv1,电容电压
    ADC_InitConfig(ADC8, 80000); //初始化adc   rv,整流电压
    ADC_InitConfig(ADC10, 80000); //初始化adc  ri
  //  ADC_InitConfig(ADC21, 80000); //初始化adc  ri
  //  ATOM_PWM_InitConfig(IfxGtm_ATOM2_6_TOUT92_P13_1_OUT,5000, 80000);
    ATOM_PWM_InitConfig(IfxGtm_ATOM0_1_TOUT11_P00_2_OUT, 0 , 80000);//电机a
    PIN_InitConfig(SD, PIN_MODE_OUTPUT, 0);                       //使能端SD
    PIN_Write(SD, 0);                                //初始设定为低电平
    PIN_InitConfig(SD, PIN_MODE_OUTPUT, 0);                       //使能端SD
    PIN_Write(SD, 0);                                //初始设定为低电平
    Chrage_PID_Init();                              //PID
   // STM_InitConfig(STM1, STM_Channel_1, 20000);
}

void charge_read()
{
       // static uint8 i=0;
       // ANO_DT_send_int16_sound(currentData1,currentData2,1,1);
//        charge_filt_1[i] = ADC_Read(ADC11);//rv1电容电压
//        charge_filt_2[i] = ADC_Read(ADC8);//rv整流电压
//        charge_filt_3[i] = ADC_Read(ADC10);//电流
//        Value_v2 =ADC_Read(ADC11);//rv1电容电压
//        Value_v1 = ADC_Read(ADC8);//rv整流电压
//        Value_i1 =ADC_Read(ADC10);//电流
        Value_v2 = ADC_ReadAverage(ADC11,10);//rv1电容电压
        Value_v1 = ADC_ReadAverage(ADC8,10);//rv整流电压
        Value_i1 = ADC_ReadAverage(ADC10,10);//电流
    //  charge_power=ADC_ReadAverage(ADC5,5);
 //  battery=ADC_ReadAverage(ADC11,10);
 //  ANO_DT_send_int16(charge_power,battery,start_flag,0,0,0,0,0);
//       i++;
//       if(i==2){
//           for (int i=0;i<2;i++){
//               Value_v2 += charge_filt_1[i];//rv1电容电压
//               Value_v1 += charge_filt_2[i];//rv整流电压
//               Value_i1 += charge_filt_3[i];//电流
//           }
          Value_v2=Value_v2* 20.2*3.3/4096;
          Value_v1=Value_v1 * 20.2*3.3/4096;
          Value_i1=Value_i1* 2.5*3.3/4096*0.54;
          Value_v2=0.7*Value_v2+last_Value_v2*0.3;
          last_Value_v2=Value_v2;
          Value_v1=0.7*Value_v1+last_Value_v1*0.3;
          last_Value_v1=Value_v1;
          Value_i1=0.7*Value_i1+last_Value_i1*0.3;
          last_Value_i1=Value_i1;
//           i=0;
//       }
}
//发车

void setup(){
    while(start_flag)
    {
        charge_read();
        if(battery>1700)
        start_flag=0;//跳出while循环
       ANO_DT_send_int16(start_flag,battery,0,0,0,0,0,0);
    }
}

/*充电判断
 * 该函数在中断中判断
 */
/*
void charge_judge()
{
   if(battery<stop_judge)
   {
       charg_flag=0;
   }
    if(charg_flag==0&&charge_power>40){
        biasDuty=0;
        setSpeedTo(0);//停车
        power_flag=1;
    }

    if(power_flag){//开启充电节拍
        time_charge++;
        if(time_charge==500){//定时10s
            if(battery<1300){//在第2,3灯罩处或者位置不对
                charg_flag=1;
                power_flag=0;
                time_charge=0;
            }
            else{//位于第1信标灯或充电位置较佳
                power_flag=0;
                time_charge=0;
            }

        }
    }

   if(battery>start_judge)
   {
       charg_flag=1;
       //setSpeedTo(50);
       //power_flag=0;
   }

}
*/

//-------------------------------------------------------------------------------------------------------------------
//  @brief      积分分离 PID 控制实现
//              当系统处于启动、结束或大幅增减设定的过程中时，短时间内系统输出有很大的偏差，如此便会在短时间内产生较大的积分累计，
//              导致控制量超过执行机构可能允许的最大动作范围对应的极限控制量，进而引起系统较大超调，甚至造成系统的震荡。
//              引入积分分离的目的在于解决上述问题，其基本思想是，当控制量接近给定值时，引入积分控制，消除静态误差；
//              当控制量与给定值相差较大时，取消积分作用，避免积分累加和过大造成的系统不稳定因素增加。
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
float SeqIntPIDErrADD = 0.0;
float SeqIntErrBack = 0.0;//last
float SeqIntPID(float Kp, float Ki, float Kd, float GiveValue, float ActualValue)
{

    float result;
    float Err,KpWork, KiWork, KdWork;
    Err = GiveValue - ActualValue;
    KpWork = Kp*Err;                   //PID计算
    KiWork = Ki*SeqIntPIDErrADD;
    KdWork = Kd*(Err-SeqIntErrBack);
    if(fabs(Err) >= GiveValue * 0.7)    // 消除在启动时，积分过大导致超调，误差大于 CHARGE_P * 0.8时I为0。例如50w时，50 * 0.8=40，超过40以上，I不起作用
    {
        result = KpWork+KdWork;
        SeqIntPIDErrADD = 0;
    }
    else
    {
        result = KpWork+KiWork+KdWork;
        SeqIntPIDErrADD = SeqIntPIDErrADD + Err;
    }
    SeqIntErrBack = Err;
    return result;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      恒功率充电控制
//  @param      GiveValue       预定值
//  @param      ActualValue     实际值
//  @return     void
//  @message                    给定一个功率值与实际功率（用电压表示）比较，得出PID控制值，用以控制恒功率充电模块
//  @message                    由于实际功率只能依靠电压值（与占空比成正比）来调节，当给出一个功率误差后，P*t=1/2*C*U*U
//  @message                    得到P与U是非线性。当t变化很小时，近视认为是线性，可以得到功率->电压->占空比线性关系。
//  Sample usage:               Charge_Ctrl(CHARGE_P, Value_v1*1.0/1000*Value_i1/1000); //给定功率为CHARGE_P，实际功率为Value_v1*1.0/1000*Value_i1/1000
//  -------------------------------------------------------------------------------------------------------------------
Charge_InitTypedef Charge;      //定义PID结构体
float Charge_Time = 0;          //充电持续时间
float CHARGE_TIME = 0;          //初始化时间
uint8 CHARGE_P;                 //设定功率
void Charge_Ctrl(float GiveValue, float ActualValue)
{
    float Charge_Err, Charge_Value;     //误差、速度
    Charge_Err = SeqIntPID(Charge.Charge_Kp, Charge.Charge_Ki, Charge.Charge_Kd, GiveValue, ActualValue);    //PID输出误差
    Charge_Value = Charge_Err + ActualValue;

    Charge_Value =  sqrt(2 * Charge_Value * (CHARGE_TIME + Charge_Time) / CHARGE_C);//前馈控制P*t=1/2*C*U*U,得到目标电压
    Charge_Time += CHARGE_INTERRUPT_TIM;   //时间累加

    Charge_Interface(Charge_Value);             //给驱动功率值
}


//充电保护函数
uint8 Charge_protect = 0;
void Charge_Protect(void)
{
    if(Charge_PWM >= 90)            //限制输出最大占空比
    {
        Charge_PWM = 90;
        if(Charge_protect >= 50)    //开始计数，当发现达不到功率时，降低功率以保护电路
        {
            Charge_protect = 0;
            CHARGE_P_INIT -= 5;     //原：CHARGE_P；改：CHARGE_P_INIT宏为uint8
        }
        else
            Charge_protect++;
    }
    else
        Charge_protect = 0;

    if(Charge_PWM <= 0)        //限制最小占空比
        Charge_PWM = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pwm控制输出接口
//  @param      Charge_Value    给定电压
//  @return     void
//  @message                    给定电压,转换为占空比控制
//  Sample usage:               Charge_Interface(40) //电压为40
//-------------------------------------------------------------------------------------------------------------------
float Charge_PWM = 0;

void Charge_Interface(float Charge_Value)
{
    Charge_PWM =  3 * Charge_Value + 10 ;                  //电压转PWM占空比，加8跳过理想二极管导通，可修改

    Charge_Protect();

   ATOM_PWM_SetDuty(IfxGtm_ATOM0_1_TOUT11_P00_2_OUT,Charge_PWM*100 ,80000);
   ANO_DT_send_int16(Value_v1*100,Value_i1*100,Value_v2*100,Value_v1*1.0*Value_i1,CHARGE_P,Charge_PWM*100,AdcFinishFlag,g_Angle);
 //   ATOM_PWM_SetDuty(IfxGtm_ATOM0_1_TOUT11_P00_2_OUT,3000,50000);
   // pwm_duty(ATOM1_CH0_P21_2, Charge_PWM * 100);        //设置占空比为百分之Charge_PWM
}

//占空比初始化函数
void Chrage_PID_Init()
{
    //PID值
    Charge.Charge_Kp = 1.5;
    Charge.Charge_Ki = 0.15;
    Charge.Charge_Kd = 0.00;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化函数
//  @param
//  @return     void
//  @message                    初始化使能3个ADC，1个使能端，一个PWM输出端，一个PID参数初始化端
//  Sample usage:               Charge_Init(); //充电初始化
//-------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------
//  @brief      充电开始结束检测函数
//  @param
//  @return     void
//  @message                    当使能端SD=1,开始充电，当SD = 0,不充电
//  @message
//  Sample usage:               Charge_Check();
//-------------------------------------------------------------------------------------------------------------------
uint8 Charge_Flag = 0;              //充电开始标志
uint8 Charge_Flag_start = 1;        //真实充电开始标志
uint8 Charge_Full = 0;              //充满标志
uint8 j;
void Charge_Check(void)
{
  //  uint8 temp_str[200];
    //充满检测算法。窗口比较，仿抖动
    if(Value_v2 < CHARGE_V - 2)//电压小于10v
        Charge_Full = 0;
    else if(Value_v2 > CHARGE_V)//大于12， Charge_Full = 1;
    {

        Charge_Full = 0;
        Charge_Flag=0;
    }

    //开始充电判断算法
    if(Charge_Flag && !Charge_Full)     //当标志位开启，且未充满时
    {
        //判断是否有输入，若无，则断开50次，开启3次，检测是否有输入
        if(Value_v1*1.0*Value_i1 < 5) //判断功率是否小于一个阈值，小于则检测
        {
            j++;
            if(j >= 40)     //断开50次
            {
                if(j >= 70) //开启3次
                    j = 0;
                Charge_Flag_start = 1;
            }
            else
                Charge_Flag_start = 0;
        }
        else                    //满足功率要求，开始充电
        {
            Charge_Flag_start = 1;
        }
        }
                else                        //关闭充电
        Charge_Flag_start = 0;

    //初始化参数，开始充电
    if(Charge_Flag_start)       //开始充电
    {
        if(Value_v2 < CHARGE_P_INIT / 9 - CHARGE_C/2)    //减少刚开始充电过冲
            CHARGE_P = CHARGE_P_INIT * 0.8;
        else
            CHARGE_P = CHARGE_P_INIT;
        PIN_Write(SD, 1);
        Charge_Ctrl(CHARGE_P, Value_v1*1.0*Value_i1); //充电控制
    }
    else                        //充电未开始，初始化参数
    {
        PIN_Write(SD, 1);
        Charge_Ctrl(0,0);
        Charge_Time = 0;
        CHARGE_TIME = Value_v2*Value_v2*CHARGE_C/100;   //P*t=1/2*C*U*U，采集电压，计算大概当前时间.乘一个比例系数

}
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      充电函数
//  @param
//  @return     void
//  @messagev
//  @message                    充电调用此函数放在中断里即可
//  Sample usage:               Charge_Start();  //充电
//-------------------------------------------------------------------------------------------------------------------
void Charge_Start(void)
{
    charge_read();      //获取2个电压值，一个电流值
    Charge_Check();
}

