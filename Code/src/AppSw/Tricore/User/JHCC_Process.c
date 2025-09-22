/*
 * JHCC_Process.c
 *
 *  Created on: 2023年7月16日
 *      Author: 君毋语
 */

#include "include.h"
//*********************************节拍器变量***********************************//

#define reduce_speed_wave_high 80
#define reduce_speed_wave_low 2

//节拍标志
uint8 modechange_coolflag=0;//掉头冷却标志，0完成，1正在冷却，不允许改变掉头标志
uint8 cooldowm_flag=0;//转向冷却标志位
uint8 rotate_flag=0;//旋转flag
uint8 lastrotate_flag=0;
uint8 reduce_speedflag=0;
uint8 up_speedflag=0;
uint8 dontspeedselect=1;//速度选择
uint8 aimangleflag1;
uint8 aimangleflag2;
uint8 lastaimangleflag1;
uint8 lastaimangleflag2;
uint8 updateflag;
short aimangle1;
short aimangle2;
uint8 allowaimangle;
uint8 buff_flag=0;
//节拍计时变量
short mode_change_cnt=0;
short modechange_cooltime=0;//掉头冷却时间
short cooldowntime=0;
short rotate_time;//允许陀螺仪转向的时间
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

    if(wave_out<reduce_speed_wave_high )     up_speedflag=1;//开启加速节拍器，延时一段时间后加速

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
    if(rotatetime>=100){ //每  100ms计入一次角度
    static short i=0;
    sdata[i]=g_Angle;

    lastaimangleflag1=aimangleflag1;
    lastaimangleflag2=aimangleflag2;

    if(i==0){
        aimangle1=sdata[0];//获得预知转向角，非精确角
        aimangleflag1=1;    //开启预知转向
    }
    i++;
    if(i==frequency){
        aimangle2=sdata[2];//得到较为可信的目标角度
        aimangleflag1=0;//关闭预知转向
        aimangleflag2=1;    //开启叠加转向
        i=0;
    }
    rotatetime=0;
    //ANO_DT_send_int16(speedActualLeft,speedActualRight ,aimangle1,aimangle2,icmErr, aimangleflag2, sdata[0],g_Angle);
    }

}
/*转向控制策略：角度突变
 * 如果角度过大，使用陀螺仪一次性转到目标角度，但是由于角度突然改变会导致一定的不稳定性，所以在改变角度后的1s内不能再使用
 *
 */
void bias_control(){//&&(g_Angle<130||g_Angle>=-130
    if((g_Angle>=45||g_Angle<=-45) &&cooldowm_flag==1)//角度过大，使用陀螺仪一次性转到目标角度
    {
//        rotatetime+=20;//计时，每100ms计入一次角度数据，取第三个数据作为目标角度
//        get_stable_angle();//获取角度并开启转向
        rotate_flag=1;//允许陀螺仪转向

    }
    if(rotate_flag)//取得较为可靠的角度数据后开启
    {
        rotate_time+=20;
        speedbiasPID();
    }
    else  icmDuty=0;

    if(rotate_time>=1000){
        rotate_flag=0;//允许用0.8s进行转向
        rotate_time=0;
        cooldowm_flag=0;
        aimangleflag2=0;
    }
    cooldowntime+=20;//冷却时间加20ms
    if(cooldowntime>=2000){
    cooldowm_flag=1;
    cooldowntime=0;
    }
    lastrotate_flag=rotate_flag;
}

void modechange_speed(){//掉头速度变换
       if(run_mode!=lastrun_mode){
           buff_flag=1;         //开启缓冲
           modechange_coolflag=1;//开启冷却，不允许掉头
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
       else dontspeedselect=0;//完成加速，允许速度选择

       if(buff_time==40){
           buff_flag=0;
           buff_time=0;
       }
       if(modechange_coolflag)
           modechange_cooltime+=20;
       if(modechange_cooltime>=2000)//冷却时间2s
           modechange_coolflag=0;//开启冷却
}
