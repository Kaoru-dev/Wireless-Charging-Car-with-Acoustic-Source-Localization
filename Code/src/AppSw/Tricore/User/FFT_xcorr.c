/*
 * FFT_xcorr.c
 *
 *  Created on: 2023��5��23��
 *      Author: Administrator
*/

#include "include.h"
#include "FFT_xcorr.h"


#define pi 3.1415926535898



//2048

#define Sampling_Num 2048//adc�ɼ����ݳ���
#define FFT_Pointer 4096//����������FFT����ĵ�����ö�Ϊ2��N�η�����FFT_Pointer =2*Sampling_Num
#define Sampling_Num_3 6144
#define Filter_Low 40     //Filter_Low = f1(200)*N/Fs
#define Filter_High 410   //Filter_High = f2(2000)*N/Fs


#define none_value 400//�����ж������źŵ���Ч��


volatile uint8  AdcFinishFlag = 0;//adc�ɼ���ɱ�־λ

volatile uint8  AdcBuffIndex = 0;//adc˫buff������  ������ɵ��������
uint32 adcCount = 0;//adc�����±�
short adcCount2 = 0;

uint8 amplitude_flag=0;//������־,����Ϊ1
uint8 last_amplitude_flag=0;
short ampliting_critical=2000;
short sound_all;
volatile float g_Angle;
double wave_out;
double wave_out_last;
//(4*2*2048*2=32k)�ĸ���˷�����
/*
volatile int16_t MIC0_DATA[2][Sampling_Num];
volatile int16_t MIC1_DATA[2][Sampling_Num];
volatile int16_t MIC2_DATA[2][Sampling_Num];
volatile int16_t MIC3_DATA[2][Sampling_Num];
*/

//adcflash

volatile int16_t MIC0_DATA[Sampling_Num];
volatile int16_t MIC1_DATA[Sampling_Num];
volatile int16_t MIC2_DATA[Sampling_Num];
volatile int16_t MIC3_DATA[Sampling_Num];

volatile int16_t DATA0[3072];
volatile int16_t DATA1[3072];
volatile int16_t DATA2[3072];
volatile int16_t DATA3[3072];



cfloat32 xcorr[Sampling_Num_3];//2048

/*��ʱ��*/
short   soundtime=0;            //���ڼ���ÿ����ýǶ���Ҫ��ʱ��
short   soundtime_final=0;      //���ڼ���ÿ����ýǶ���Ҫ��ʱ��,����
uint8   Istart=0;               //������־
short   rotatetime=0;             //��תʱ����ʱ����
//��������������� a����*b(b�Ĺ���)
cfloat32 conj_mul(cfloat32 a, cfloat32 b)
{
    cfloat32 c;
    c.real =  a.real*b.real + a.imag*b.imag;
    c.imag = -a.real*b.imag + a.imag*b.real;
    return c;
}

//���������ֵ
float Absolute_Value(cfloat32 a)
{

    float value;
    /*float i,j;
    i=a.real*a.real;
    j=a.imag*a.imag;
    value=i+j;
*/
    value = a.real*a.real;
    return value;
}
//��ʼ����˷磬��main.c��ֱ�ӵ�����
void Sound_init(void)
{
    //STM_InitConfig(STM0, STM_Channel_0, 100);//ÿ0.1ms����һ�Σ�Ƶ��10khz,�ű��Ƶ��Ϊ1khz
    ADC_InitConfig(ADC0, 1000000); //��ʼ��adc
    ADC_InitConfig(ADC1, 1000000); //��ʼ��adc
    ADC_InitConfig(ADC2, 1000000); //��ʼ��adc
    ADC_InitConfig(ADC3, 1000000); //��ʼ��adc
    // ANO_DT_send_int16_sound(1,5,1,1);
  //  STM_InitConfig(STM0, STM_Channel_0, 100);//ÿһ�뷢��һ�����ݣ����ڴ������ʲ���������ʵ���������ÿ30s����һ��
}
void VoiceGetSample(void)
{

    //ADCflash
        static short catch;
        static short site = 0;
        //static short past;

        if (adcCount % 1024 == 0)  AdcFinishFlag = 1;
        if (adcCount >= 3072) adcCount = 0;

       if (adcCount2 >= Sampling_Num)
       {
           adcCount2 = 0;

           site += 2048;

           if (site>3072) site = 1024;
           else if(site==3072) site=0;
       }
       /*
       past = adcCount2 - 1024;
       if (past<0)  past += Sampling_Num;
       */

       catch = site+adcCount2;//catchҪ��adcCount���
       if (catch>=3072) catch -= 3072;

       DATA0[adcCount] = ADC_Read(ADC0);
       DATA1[adcCount] = ADC_Read(ADC1);
       DATA2[adcCount] = ADC_Read(ADC2);
       DATA3[adcCount] = ADC_Read(ADC3);
/*
       DATA0[past] /= 2;
       DATA1[past] /= 2;
       DATA2[past] /= 2;
       DATA3[past] /= 2;
*/
       MIC0_DATA[adcCount2] = DATA0[catch];
       MIC1_DATA[adcCount2] = DATA1[catch];
       MIC2_DATA[adcCount2] = DATA2[catch];
       MIC3_DATA[adcCount2] = DATA3[catch];

       adcCount++;
       adcCount2++;

       /*
    static uint8 adcIndex = 0;
    if(adcCount >= Sampling_Num)
    {
        adcCount = 0;
        AdcFinishFlag = 1;
         //�л�buff������
        if(adcIndex == 0)
        {
            adcIndex = 1;
            AdcBuffIndex = 0;
        }
        else
        {
            adcIndex = 0;
            AdcBuffIndex = 1;
        }
    }

    MIC0_DATA[adcIndex][adcCount] = ADC_Read(ADC0)-300;
    MIC1_DATA[adcIndex][adcCount] = ADC_Read(ADC1)-300;
    MIC2_DATA[adcIndex][adcCount] = ADC_Read(ADC2)-300;
    MIC3_DATA[adcIndex][adcCount] = ADC_Read(ADC3)-300;
   //ANO_DT_send_int16_sound( MIC0_DATA[adcIndex][adcCount], MIC1_DATA[adcIndex][adcCount],MIC2_DATA[adcIndex][adcCount],MIC3_DATA[adcIndex][adcCount] );
    adcCount++;
    */

}
void CSP(cfloat32 *m)
{
    float phi;
    phi =m->real*m->real+m->imag*m->imag;
    m->real/=powf(phi,0.375);//0.75,0.5*0.75=0.375
    m->imag/=powf(phi,0.375);
}
//����ؼ���
short MY_Xcorr(short* mic1,short* mic2,double* wave)
{
    short Max_Array_Num=0;            //���������õ������ֵ��Ӧ�������±�
    short i=0;
    float Mic_max=0.0;

     //2048

    cfloat32 *x=xcorr,*y=xcorr+4096,*Fin_Mic_ifftOut=xcorr+2048;

    FFT(x, mic1, Sampling_Num);
    FFT(y, mic2, Sampling_Num);

    for(i=0;i<Sampling_Num;i++)
    {
        if (i>Filter_Low&&i<Filter_High)
        {
            Fin_Mic_ifftOut[2*i]=conj_mul(x[i],y[i]);      //�����ź�Ƶ��ĳ˻�
            CSP(Fin_Mic_ifftOut+2*i);
        }
        else
        {
            Fin_Mic_ifftOut[2*i].real=Fin_Mic_ifftOut[2*i].imag=0;      //�����ź�Ƶ��ĳ˻�
        }
        Fin_Mic_ifftOut[2*i+1].real=0;
        Fin_Mic_ifftOut[2*i+1].imag=0;
    }

    IFFT(Fin_Mic_ifftOut,FFT_Pointer);


    for(i=0;i<FFT_Pointer;i++)
    {
       if(Absolute_Value(Fin_Mic_ifftOut[i])>Mic_max)
        {
            Mic_max=Absolute_Value(Fin_Mic_ifftOut[i]);            //�ҳ���ֵ���ĵ�,�˼�Ϊ�������õĵ�
            Max_Array_Num=i;                   //�������õĵ��Ӧ�������±�
        }
    }

    *wave = Mic_max;


    float all_value=0;

    if(Max_Array_Num<7)//9��һ����ֵ�����Ը���ʵ������޸ģ��������else if(Sampling_Num-Max_Array_Num<9)��һ�ԣ���Max_Array_Num<9˵����Դ����һ����˷磬��Sampling_Num-Max_Array_Num<9˵����Դ��������һ����˷�
        {
            for(i=Max_Array_Num-20+FFT_Pointer;i>Max_Array_Num+FFT_Pointer-340;i--)//������ֵ��������ֵ�󣬽����ֵ���� 20 �����ݿ�ʼ����320��������ƽ����Ȼ���жϷ�ֵ��С�����ƽ��ֵ�ı�ֵ
            {
                all_value=all_value+Fin_Mic_ifftOut[i].real;
            }
            return Max_Array_Num;
            all_value=Fin_Mic_ifftOut[Max_Array_Num].real/(all_value/320);
            if(all_value>5.82)//1.01�Ǳ�ֵ����ֵ�������Ӿ�������޸�
                return Max_Array_Num;
            else
                return none_value;
        }

        //2048

        else if(Sampling_Num-Max_Array_Num<7)//9��һ����ֵ�����Ը���ʵ������޸�
        {
            for(i=Max_Array_Num-20;i>Max_Array_Num-340;i--)
            {
                all_value=all_value+Fin_Mic_ifftOut[i].real;
            }
            return Max_Array_Num-Sampling_Num;
            all_value=Fin_Mic_ifftOut[Max_Array_Num].real/(all_value/320);
            if(all_value>5.82)//�趨1.01Ϊʶ�����������źŵ���ֵ�������Ӿ�������޸�
                return Max_Array_Num-Sampling_Num;
            else
                return none_value;
        }
        else
            return none_value;

}


float new_atan1(float a,float b)
{
    float c;

    if ((a==0.0&&b==-0.0) || (a==-0.0&&b==0.0))
    {
        a =b;
    }

    //c = atan2(a,b)*180/pi+45;//У��

    /*
     //��Ҫ�����汾
        c = atan2(a,b)*180/pi+90;
        if (c>180) c=c-360;
     */
     c = atan2(a,b)*180/pi;//���õ����汾

     return c;

}

short angle_dif1(float a,float b)
{
    if(fabs(a-b)>30.0) return 1;
    else return 0;
}

void over_cope(float *a,float *b)
{
    if (*a>=150.0&&*a<=180.0&&*b>=-180.0&&*b<=*a-330) *b+=360;//*a-330 = *a+30-360
    if (*b>=150.0&&*b<=180.0&&*a>=-180.0&&*a<=*b-330) *a+=360;
}

void VoiceProcess1()
{
    Istart=1;//��������ʼ��ʱ
    float angle1,angle2,angle3,angle4;
    short angle_flag;
    static float last_angle;
    double wave1,wave2,wave3,wave0;
    //static double last_wave;
    short a;


    //ANO_DT_send_int16(AdcFinishFlag,angle_flag,adcCount,0,0,0,0,0);
    if(AdcFinishFlag)
    {
        angle_flag=1;


        //���õ����汾

        //2 3
        //1 0

        short Delay32 , Delay01 , Delay03 , Delay12;

        /*
        Delay32 = MY_Xcorr((short*)&MIC3_DATA[AdcBuffIndex],(short*)&MIC2_DATA[AdcBuffIndex],&wave0);

        Delay01 = MY_Xcorr((short*)&MIC0_DATA[AdcBuffIndex],(short*)&MIC1_DATA[AdcBuffIndex],&wave1);

        Delay03 = MY_Xcorr((short*)&MIC0_DATA[AdcBuffIndex],(short*)&MIC3_DATA[AdcBuffIndex],&wave2);

        Delay12 = MY_Xcorr((short*)&MIC1_DATA[AdcBuffIndex],(short*)&MIC2_DATA[AdcBuffIndex],&wave3);
*/

        Delay32 = MY_Xcorr((short*)MIC3_DATA,(short*)MIC2_DATA,&wave0);

        Delay01 = MY_Xcorr((short*)MIC0_DATA,(short*)MIC1_DATA,&wave1);

        Delay03 = MY_Xcorr((short*)MIC0_DATA,(short*)MIC3_DATA,&wave2);

        Delay12 = MY_Xcorr((short*)MIC1_DATA,(short*)MIC2_DATA,&wave3);


        /*
         *
        0 2
       * * *
       *3*1*
         */

        wave_out_last=wave_out;
        wave_out = (wave2+wave0)/2;//ǰ

        if(Delay32!=none_value&&Delay01!=Delay03!=none_value&&Delay12!=none_value)
        {

            angle1=new_atan1(Delay32,Delay03);
            angle2=new_atan1(Delay32,Delay12);
            angle3=new_atan1(Delay01,Delay03);
            angle4=new_atan1(Delay01,Delay12);

            over_cope(&angle1,&angle2);
            over_cope(&angle1,&angle3);
            over_cope(&angle1,&angle4);
            over_cope(&angle2,&angle3);
            over_cope(&angle2,&angle4);
            over_cope(&angle3,&angle4);


            if(angle_dif1(angle1,angle2) ||angle_dif1(angle1,angle3) ||angle_dif1(angle1,angle4) ||angle_dif1(angle2,angle3) ||angle_dif1(angle2,angle4) || angle_dif1(angle3,angle4))
            {
                angle_flag=0;
            } //дһ���Ƕ����30�ȵĳ���

            if (angle_flag)
            {
                g_Angle =(angle1+angle2+angle3+angle4)/4.0 +45;

                while (g_Angle>180) g_Angle=g_Angle-360;

                g_Angle = ((int)g_Angle)/5*5.0;

                last_angle=g_Angle;
                a=1;

                //last_wave = wave_out;
            }
            else
            {
                g_Angle = last_angle;
                a=0;

                //wave_out = last_wave;
            }

        }
        else
        {
             g_Angle = last_angle;
             a=0;

             //wave_out = last_wave;
        }


        if(run_mode)
        {//����
            if(g_Angle>0)
                g_Angle-=180;
            else
                g_Angle+=180;
        }
      //  ANO_DT_send_int16(g_Angle,AdcFinishFlag ,speedSet,speedActualLeft,speedActualRight, speedDuty, biasDuty,Value_v2*100);
//        Mode_Select();//ģʽѡ��
//           soundtime_final=soundtime;//��ȡ����ʱ��
//           soundtime=0;
        //  ANO_DT_send_int16(0,0,0,0,0, 0, Value_v1*100,g_Angle);
//          ANO_DT_send_int16(a,g_Angle,last_angle,angle_flag,angle1,angle2,angle3,wave_out);
          AdcFinishFlag = 0;

    }
}


void sound_amplitude (){
    static int r=0;
    static  int sm[4],dat[4];

    for(short i=0;i<2048;i++)
    {
        sm[0]+= abs(MIC0_DATA[i]);
        sm[1]+= abs(MIC1_DATA[i]);
        sm[2]+= abs(MIC2_DATA[i]);
        sm[3]+= abs(MIC3_DATA[i]);
    }
    sm[0]=sm[0]/2048;
    sm[1]=sm[1]/2048;
    sm[2]=sm[2]/2048;
    sm[3]=sm[3]/2048;
    dat[0]+=sm[0];
    dat[1]+=sm[1];
    dat[2]+=sm[2];
    dat[3]+=sm[3];
    if(r==2)
    {
        sound_all=dat[0]/2+dat[1]/2+dat[2]/2+dat[3]/2;
        if(sound_all>ampliting_critical)//����flagΪ1

            amplitude_flag=1;
        else
            amplitude_flag=0;
//ANO_DT_send_int16(dat[0]/2,dat[1]/2,dat[2]/2,dat[3]/2,dat[0]/2+dat[1]/2+dat[2]/2+dat[3]/2,0,0,0);
    dat[0]=0;
    dat[1]=0;
    dat[2]=0;
    dat[3]=0;
        r=0;
    }
    sm[0]=0;
    sm[1]=0;
    sm[2]=0;
    sm[3]=0;
    r++;

}
