/*
 * JHCC_charge.c
 *
 *  Created on: 2023��5��14��
 *      Author: Administrator
 */
/*�ж��Ƿ���Ҫ����뷢��*/
#include "include.h"
#include "FFT_xcorr.h"
// ���޸ı���
#define CHARGE_C                12               // ������������
              // ��繦��
#define CHARGE_V                13.0              // ����ѹ
#define CHARGE_INTERRUPT_TIM    0.02            // �ж�ʱ��
uint8   CHARGE_P_INIT=50;                       // ��繦��
double Value_i1;   //�������ֵ
double Value_v1;   //�����ѹֵ
double Value_v2;   //�����ѹֵ
double last_Value_v2;
double last_Value_i1;
double last_Value_v1;
int time_charge=0;
short charge_power;
double battery;//���ݵ���
short start_judge=1500;//���������ж���
short stop_judge=1000;//
uint8 start_flag=1;
uint8 charg_flag=1;
short charge_filt_1[10],charge_filt_2[10],charge_filt_3[10];//�˲�����
uint8 power_flag=0;
uint8 turn_off_flag=0;
short turn_off_time;//��ƽ�����
void charge_init()
{
    ADC_InitConfig(ADC5, 80000); //��ʼ��adc
    ADC_InitConfig(ADC11, 80000); //��ʼ��adc  rv1,���ݵ�ѹ
    ADC_InitConfig(ADC8, 80000); //��ʼ��adc   rv,������ѹ
    ADC_InitConfig(ADC10, 80000); //��ʼ��adc  ri
  //  ADC_InitConfig(ADC21, 80000); //��ʼ��adc  ri
  //  ATOM_PWM_InitConfig(IfxGtm_ATOM2_6_TOUT92_P13_1_OUT,5000, 80000);
    ATOM_PWM_InitConfig(IfxGtm_ATOM0_1_TOUT11_P00_2_OUT, 0 , 80000);//���a
    PIN_InitConfig(SD, PIN_MODE_OUTPUT, 0);                       //ʹ�ܶ�SD
    PIN_Write(SD, 0);                                //��ʼ�趨Ϊ�͵�ƽ
    PIN_InitConfig(SD, PIN_MODE_OUTPUT, 0);                       //ʹ�ܶ�SD
    PIN_Write(SD, 0);                                //��ʼ�趨Ϊ�͵�ƽ
    Chrage_PID_Init();                              //PID
   // STM_InitConfig(STM1, STM_Channel_1, 20000);
}

void charge_read()
{
       // static uint8 i=0;
       // ANO_DT_send_int16_sound(currentData1,currentData2,1,1);
//        charge_filt_1[i] = ADC_Read(ADC11);//rv1���ݵ�ѹ
//        charge_filt_2[i] = ADC_Read(ADC8);//rv������ѹ
//        charge_filt_3[i] = ADC_Read(ADC10);//����
//        Value_v2 =ADC_Read(ADC11);//rv1���ݵ�ѹ
//        Value_v1 = ADC_Read(ADC8);//rv������ѹ
//        Value_i1 =ADC_Read(ADC10);//����
        Value_v2 = ADC_ReadAverage(ADC11,10);//rv1���ݵ�ѹ
        Value_v1 = ADC_ReadAverage(ADC8,10);//rv������ѹ
        Value_i1 = ADC_ReadAverage(ADC10,10);//����
    //  charge_power=ADC_ReadAverage(ADC5,5);
 //  battery=ADC_ReadAverage(ADC11,10);
 //  ANO_DT_send_int16(charge_power,battery,start_flag,0,0,0,0,0);
//       i++;
//       if(i==2){
//           for (int i=0;i<2;i++){
//               Value_v2 += charge_filt_1[i];//rv1���ݵ�ѹ
//               Value_v1 += charge_filt_2[i];//rv������ѹ
//               Value_i1 += charge_filt_3[i];//����
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
//����

void setup(){
    while(start_flag)
    {
        charge_read();
        if(battery>1700)
        start_flag=0;//����whileѭ��
       ANO_DT_send_int16(start_flag,battery,0,0,0,0,0,0);
    }
}

/*����ж�
 * �ú������ж����ж�
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
        setSpeedTo(0);//ͣ��
        power_flag=1;
    }

    if(power_flag){//����������
        time_charge++;
        if(time_charge==500){//��ʱ10s
            if(battery<1300){//�ڵ�2,3���ִ�����λ�ò���
                charg_flag=1;
                power_flag=0;
                time_charge=0;
            }
            else{//λ�ڵ�1�ű�ƻ���λ�ýϼ�
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
//  @brief      ���ַ��� PID ����ʵ��
//              ��ϵͳ�����������������������趨�Ĺ�����ʱ����ʱ����ϵͳ����кܴ��ƫ���˱���ڶ�ʱ���ڲ����ϴ�Ļ����ۼƣ�
//              ���¿���������ִ�л�������������������Χ��Ӧ�ļ��޿���������������ϵͳ�ϴ󳬵����������ϵͳ���𵴡�
//              ������ַ����Ŀ�����ڽ���������⣬�����˼���ǣ����������ӽ�����ֵʱ��������ֿ��ƣ�������̬��
//              �������������ֵ���ϴ�ʱ��ȡ���������ã���������ۼӺ͹�����ɵ�ϵͳ���ȶ��������ӡ�
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
float SeqIntPIDErrADD = 0.0;
float SeqIntErrBack = 0.0;//last
float SeqIntPID(float Kp, float Ki, float Kd, float GiveValue, float ActualValue)
{

    float result;
    float Err,KpWork, KiWork, KdWork;
    Err = GiveValue - ActualValue;
    KpWork = Kp*Err;                   //PID����
    KiWork = Ki*SeqIntPIDErrADD;
    KdWork = Kd*(Err-SeqIntErrBack);
    if(fabs(Err) >= GiveValue * 0.7)    // ����������ʱ�����ֹ����³����������� CHARGE_P * 0.8ʱIΪ0������50wʱ��50 * 0.8=40������40���ϣ�I��������
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
//  @brief      �㹦�ʳ�����
//  @param      GiveValue       Ԥ��ֵ
//  @param      ActualValue     ʵ��ֵ
//  @return     void
//  @message                    ����һ������ֵ��ʵ�ʹ��ʣ��õ�ѹ��ʾ���Ƚϣ��ó�PID����ֵ�����Կ��ƺ㹦�ʳ��ģ��
//  @message                    ����ʵ�ʹ���ֻ��������ѹֵ����ռ�ձȳ����ȣ������ڣ�������һ����������P*t=1/2*C*U*U
//  @message                    �õ�P��U�Ƿ����ԡ���t�仯��Сʱ��������Ϊ�����ԣ����Եõ�����->��ѹ->ռ�ձ����Թ�ϵ��
//  Sample usage:               Charge_Ctrl(CHARGE_P, Value_v1*1.0/1000*Value_i1/1000); //��������ΪCHARGE_P��ʵ�ʹ���ΪValue_v1*1.0/1000*Value_i1/1000
//  -------------------------------------------------------------------------------------------------------------------
Charge_InitTypedef Charge;      //����PID�ṹ��
float Charge_Time = 0;          //������ʱ��
float CHARGE_TIME = 0;          //��ʼ��ʱ��
uint8 CHARGE_P;                 //�趨����
void Charge_Ctrl(float GiveValue, float ActualValue)
{
    float Charge_Err, Charge_Value;     //���ٶ�
    Charge_Err = SeqIntPID(Charge.Charge_Kp, Charge.Charge_Ki, Charge.Charge_Kd, GiveValue, ActualValue);    //PID������
    Charge_Value = Charge_Err + ActualValue;

    Charge_Value =  sqrt(2 * Charge_Value * (CHARGE_TIME + Charge_Time) / CHARGE_C);//ǰ������P*t=1/2*C*U*U,�õ�Ŀ���ѹ
    Charge_Time += CHARGE_INTERRUPT_TIM;   //ʱ���ۼ�

    Charge_Interface(Charge_Value);             //����������ֵ
}


//��籣������
uint8 Charge_protect = 0;
void Charge_Protect(void)
{
    if(Charge_PWM >= 90)            //����������ռ�ձ�
    {
        Charge_PWM = 90;
        if(Charge_protect >= 50)    //��ʼ�����������ִﲻ������ʱ�����͹����Ա�����·
        {
            Charge_protect = 0;
            CHARGE_P_INIT -= 5;     //ԭ��CHARGE_P���ģ�CHARGE_P_INIT��Ϊuint8
        }
        else
            Charge_protect++;
    }
    else
        Charge_protect = 0;

    if(Charge_PWM <= 0)        //������Сռ�ձ�
        Charge_PWM = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pwm��������ӿ�
//  @param      Charge_Value    ������ѹ
//  @return     void
//  @message                    ������ѹ,ת��Ϊռ�ձȿ���
//  Sample usage:               Charge_Interface(40) //��ѹΪ40
//-------------------------------------------------------------------------------------------------------------------
float Charge_PWM = 0;

void Charge_Interface(float Charge_Value)
{
    Charge_PWM =  3 * Charge_Value + 10 ;                  //��ѹתPWMռ�ձȣ���8������������ܵ�ͨ�����޸�

    Charge_Protect();

   ATOM_PWM_SetDuty(IfxGtm_ATOM0_1_TOUT11_P00_2_OUT,Charge_PWM*100 ,80000);
   ANO_DT_send_int16(Value_v1*100,Value_i1*100,Value_v2*100,Value_v1*1.0*Value_i1,CHARGE_P,Charge_PWM*100,AdcFinishFlag,g_Angle);
 //   ATOM_PWM_SetDuty(IfxGtm_ATOM0_1_TOUT11_P00_2_OUT,3000,50000);
   // pwm_duty(ATOM1_CH0_P21_2, Charge_PWM * 100);        //����ռ�ձ�Ϊ�ٷ�֮Charge_PWM
}

//ռ�ձȳ�ʼ������
void Chrage_PID_Init()
{
    //PIDֵ
    Charge.Charge_Kp = 1.5;
    Charge.Charge_Ki = 0.15;
    Charge.Charge_Kd = 0.00;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ������
//  @param
//  @return     void
//  @message                    ��ʼ��ʹ��3��ADC��1��ʹ�ܶˣ�һ��PWM����ˣ�һ��PID������ʼ����
//  Sample usage:               Charge_Init(); //����ʼ��
//-------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��翪ʼ������⺯��
//  @param
//  @return     void
//  @message                    ��ʹ�ܶ�SD=1,��ʼ��磬��SD = 0,�����
//  @message
//  Sample usage:               Charge_Check();
//-------------------------------------------------------------------------------------------------------------------
uint8 Charge_Flag = 0;              //��翪ʼ��־
uint8 Charge_Flag_start = 1;        //��ʵ��翪ʼ��־
uint8 Charge_Full = 0;              //������־
uint8 j;
void Charge_Check(void)
{
  //  uint8 temp_str[200];
    //��������㷨�����ڱȽϣ��¶���
    if(Value_v2 < CHARGE_V - 2)//��ѹС��10v
        Charge_Full = 0;
    else if(Value_v2 > CHARGE_V)//����12�� Charge_Full = 1;
    {

        Charge_Full = 0;
        Charge_Flag=0;
    }

    //��ʼ����ж��㷨
    if(Charge_Flag && !Charge_Full)     //����־λ��������δ����ʱ
    {
        //�ж��Ƿ������룬���ޣ���Ͽ�50�Σ�����3�Σ�����Ƿ�������
        if(Value_v1*1.0*Value_i1 < 5) //�жϹ����Ƿ�С��һ����ֵ��С������
        {
            j++;
            if(j >= 40)     //�Ͽ�50��
            {
                if(j >= 70) //����3��
                    j = 0;
                Charge_Flag_start = 1;
            }
            else
                Charge_Flag_start = 0;
        }
        else                    //���㹦��Ҫ�󣬿�ʼ���
        {
            Charge_Flag_start = 1;
        }
        }
                else                        //�رճ��
        Charge_Flag_start = 0;

    //��ʼ����������ʼ���
    if(Charge_Flag_start)       //��ʼ���
    {
        if(Value_v2 < CHARGE_P_INIT / 9 - CHARGE_C/2)    //���ٸտ�ʼ������
            CHARGE_P = CHARGE_P_INIT * 0.8;
        else
            CHARGE_P = CHARGE_P_INIT;
        PIN_Write(SD, 1);
        Charge_Ctrl(CHARGE_P, Value_v1*1.0*Value_i1); //������
    }
    else                        //���δ��ʼ����ʼ������
    {
        PIN_Write(SD, 1);
        Charge_Ctrl(0,0);
        Charge_Time = 0;
        CHARGE_TIME = Value_v2*Value_v2*CHARGE_C/100;   //P*t=1/2*C*U*U���ɼ���ѹ�������ŵ�ǰʱ��.��һ������ϵ��

}
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��纯��
//  @param
//  @return     void
//  @messagev
//  @message                    �����ô˺��������ж��Ｔ��
//  Sample usage:               Charge_Start();  //���
//-------------------------------------------------------------------------------------------------------------------
void Charge_Start(void)
{
    charge_read();      //��ȡ2����ѹֵ��һ������ֵ
    Charge_Check();
}

