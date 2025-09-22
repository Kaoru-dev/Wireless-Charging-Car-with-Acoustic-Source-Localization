/*
 * JHCC_Process.c
 *
 *  Created on: 2023��7��16��
 *      Author: ������
 */

#include "include.h"
//*********************************����������***********************************//

#define reduce_speed_wave_high 80
#define reduce_speed_wave_low 2

//���ı�־
uint8 modechange_coolflag=0;//��ͷ��ȴ��־��0��ɣ�1������ȴ��������ı��ͷ��־
uint8 cooldowm_flag=0;//ת����ȴ��־λ
uint8 rotate_flag=0;//��תflag
uint8 lastrotate_flag=0;
uint8 reduce_speedflag=0;
uint8 up_speedflag=0;
uint8 dontspeedselect=1;//�ٶ�ѡ��
uint8 aimangleflag1;
uint8 aimangleflag2;
uint8 lastaimangleflag1;
uint8 lastaimangleflag2;
uint8 updateflag;
short aimangle1;
short aimangle2;
uint8 allowaimangle;
uint8 buff_flag=0;
//���ļ�ʱ����
short mode_change_cnt=0;
short modechange_cooltime=0;//��ͷ��ȴʱ��
short cooldowntime=0;
short rotate_time;//����������ת���ʱ��
short changespeed_time=0;
uint8 buff_time=0;
void select_speed()
{
    if(wave_out>=reduce_speed_wave_high )    reduce_speedflag=1;

    if(!buff_flag)
    {
        if(reduce_speedflag||g_Angle>45)
          {
            if(!run_mode)
                speed_forward=18;
            else speed_forward=15;
          }
        else if(!reduce_speedflag&&g_Angle<40)  {
            if(!run_mode)
                speed_forward=40;
            else speed_forward=35;

        }
    }

    if(wave_out<reduce_speed_wave_high )     up_speedflag=1;//�������ٽ���������ʱһ��ʱ������

    if(up_speedflag==1) changespeed_time+=20;

    if(changespeed_time>=800&&g_Angle<30)
    {
        reduce_speedflag=0;
        up_speedflag=0;
        changespeed_time=0;
    }
}

#define frequency  3
short sdata[frequency];

void get_stable_angle(){
    if(rotatetime>=100){ //ÿ  100ms����һ�νǶ�
    static short i=0;
    sdata[i]=g_Angle;

    lastaimangleflag1=aimangleflag1;
    lastaimangleflag2=aimangleflag2;

    if(i==0){
        aimangle1=sdata[0];//���Ԥ֪ת��ǣ��Ǿ�ȷ��
        aimangleflag1=1;    //����Ԥ֪ת��
    }
    i++;
    if(i==frequency){
        aimangle2=sdata[2];//�õ���Ϊ���ŵ�Ŀ��Ƕ�
        aimangleflag1=0;//�ر�Ԥ֪ת��
        aimangleflag2=1;    //��������ת��
        i=0;
    }
    rotatetime=0;
    //ANO_DT_send_int16(speedActualLeft,speedActualRight ,aimangle1,aimangle2,icmErr, aimangleflag2, sdata[0],g_Angle);
    }

}
/*ת����Ʋ��ԣ��Ƕ�ͻ��
 * ����Ƕȹ���ʹ��������һ����ת��Ŀ��Ƕȣ��������ڽǶ�ͻȻ�ı�ᵼ��һ���Ĳ��ȶ��ԣ������ڸı�ǶȺ��1s�ڲ�����ʹ��
 *
 */
void bias_control(){//&&(g_Angle<130||g_Angle>=-130
    if((g_Angle>=45||g_Angle<=-45) &&cooldowm_flag==1)//�Ƕȹ���ʹ��������һ����ת��Ŀ��Ƕ�
    {
//        rotatetime+=20;//��ʱ��ÿ100ms����һ�νǶ����ݣ�ȡ������������ΪĿ��Ƕ�
//        get_stable_angle();//��ȡ�ǶȲ�����ת��
        rotate_flag=1;//����������ת��

    }
    if(rotate_flag)//ȡ�ý�Ϊ�ɿ��ĽǶ����ݺ���
    {
        rotate_time+=20;
        speedbiasPID();
    }
    else  icmDuty=0;

    if(rotate_time>=1000){
        rotate_flag=0;//������0.8s����ת��
        rotate_time=0;
        cooldowm_flag=0;
        aimangleflag2=0;
    }
    cooldowntime+=20;//��ȴʱ���20ms
    if(cooldowntime>=2000){
    cooldowm_flag=1;
    cooldowntime=0;
    }
    lastrotate_flag=rotate_flag;
}

void modechange_speed(){//��ͷ�ٶȱ任
       if(run_mode!=lastrun_mode){
           buff_flag=1;         //��������
           modechange_coolflag=1;//������ȴ���������ͷ
           dontspeedselect=1;
       }
       if(buff_flag){
           if(buff_time<=5)speed_forward=0;
           else if(buff_time>=5&&buff_time<=15)
               speed_forward=20;
           else
           speed_forward=35;
           buff_time++;
       }
       else dontspeedselect=0;//��ɼ��٣������ٶ�ѡ��

       if(buff_time==40){
           buff_flag=0;
           buff_time=0;
       }
       if(modechange_coolflag)
           modechange_cooltime+=20;
       if(modechange_cooltime>=2000)//��ȴʱ��2s
           modechange_coolflag=0;//������ȴ
}
