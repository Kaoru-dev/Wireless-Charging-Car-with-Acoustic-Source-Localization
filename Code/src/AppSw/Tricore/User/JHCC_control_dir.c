/*
 * JHCC_control_dir.c
 *
 *  Created on: 2023年3月26日
 *      Author: 君毋语
 */
#include "include.h"


//-----------------------------宏定义区-----------------------------

//-----------------------------变量定义区-----------------------------
//方向环PID参数
//高转向变量
sint16 biasIndBigFastP     = 50;
float biasIndBigFastI     =0.00;
sint16 biasIndBigFastD     = 50;
float biasDutyAccFastI;           //方向环积分
float  biasIndBigFastPD    = 0;

//低转向变量
sint16 biasIndBigLowP     = 30;
float biasIndBigLowI     = 0;
sint16 biasIndBigLowD     = 10;
float biasDutyAccLowI;
float  biasIndBigLowPD    = 0;
//方向环变量
uint8  dontBiasPID         = 0;      //0开启方向环，1屏蔽方向环
uint8  dontBiasPID1        = 0;      //0开启方向环，1屏蔽方向环
float  biasSoundErr       = 0;      //摄像头偏移度偏差量(左拐是负，右拐是正)
float  biasSoundDifferential = 0;   //声音差值的差分，代替微分
float  biasSoundErrLast   = 0;      //前一次声音偏差量
sint16 biasDuty         ;      //方向环的占空比
//串级****************************************************************//
float icmP= 1.0;
float icmI=0.00;
float icmD= 5.0;
float   icmErr       = 0;      //陀螺仪更新误差
float   icmDifferential = 0;   //声音差值的差分，代替微分
float   icmErrLast   = 0;      //前一次声音偏差量
float  icmDuty         ;      //方向环的占空比
float   icmAccI=0;           //方向环积分
float   icmPD    = 0;

//开环
sint16  open_bias_duty;

/*
speedDutyAccI += speedGiaI * speedErr;
if (speedDutyAccI > 2000) speedDutyAccI = 2000;
else if(speedDutyAccI < -2000) speedDutyAccI = -2000;
speedDutyAccPD += speedGiaP * speedDifferential + speedGiaD * speedDifferential2;
speedDuty = speedDutyAccI + speedDutyAccPD;
*/
//方向环，小角度转至目标角度

void biasPID_low(void)
{
    //计算控制量
   // biasSoundErr =Sound_error();
    biasSoundErr=g_Angle;
    biasSoundDifferential = biasSoundErr - biasSoundErrLast;
    if (dontBiasPID == 0)
    {
        biasDutyAccLowI+=biasIndBigLowI*biasSoundErr;
            biasDuty = biasIndBigLowP * biasSoundErr +biasDutyAccLowI+ biasIndBigLowD * biasSoundDifferential;
    }

    //限幅
    if (biasDutyAccLowI > 400) biasDutyAccLowI = 400;
    else if(biasDutyAccLowI < -400) biasDutyAccLowI = -400;
    if (biasDuty > 400)biasDuty = 400;
    else if (biasDuty < -400)biasDuty = -400;
    biasSoundErrLast = biasSoundErr;
}

//方向环，位置式PID,用于完成角度突变


void biasPID(void)
{
    //计算控制量
   // biasSoundErr =Sound_error();
    biasSoundErr=g_Angle;
    biasSoundDifferential = biasSoundErr - biasSoundErrLast;
    if (dontBiasPID == 0)
    {
        biasDutyAccFastI+=biasIndBigFastI*biasSoundErr;
        biasDuty = biasIndBigFastP * biasSoundErr +biasDutyAccFastI+ biasIndBigFastD * biasSoundDifferential;
    }

    //限幅
    if (biasDutyAccFastI > 1000) biasDutyAccFastI = 1000;
    else if(biasDutyAccFastI < -1000) biasDutyAccFastI = -1000;
    if (biasDuty > 1000)biasDuty = 1000;
    else if (biasDuty < -1000)biasDuty = -1000;
    biasSoundErrLast = biasSoundErr;
//    if(biasSoundErr<10&&biasSoundErr>-10){
//        rotate_flag=0;
//        cooldowm_flag=0;//重新冷却
//        cooldowntime=0;
//    }
}

void speedbiasPID(void)//串级
{
    //计算控制量
   // biasSoundErr =Sound_error();
    if(run_mode){
        icmP= 0.4;
        icmD=2.5;
    }
    else{
        icmP= 1.2;
        icmD=5;
    }

    icmErr=Sound_error();
    icmDifferential = icmErr - icmErrLast;
    if (dontBiasPID == 0)
    {
        icmAccI+=icmI*icmErr;
        icmDuty = icmP * icmErr +icmAccI+ icmD * icmDifferential;
    }
    if(run_mode){
    //限幅
    if (icmAccI > 10) icmAccI = 10;
    else if(icmAccI < -10) icmAccI = -10;
    if (icmDuty > 10)icmDuty =10;
    else if (icmDuty < -10)icmDuty = -10;
    }
    else{
    if (icmAccI > 25) icmAccI = 25;
    else if(icmAccI < -25) icmAccI = -25;
    if (icmDuty > 25)icmDuty =25;
    else if (icmDuty < -25)icmDuty = -25;
    }
    icmErrLast = icmErr;
//    if(icmErr<20&&icmErr>-20){
//        rotate_flag=0;
//        //cooldowm_flag=0;//重新冷却
//        cooldowntime=0;
//    }
}
/*
void open_bias(){//开环控制，用在45度以内控制
    if(g_Angle>=30)
        open_bias_duty=800;
    if(g_Angle>=20&&g_Angle<30)
        open_bias_duty=600;
    if(g_Angle>10&&g_Angle<20)
        open_bias_duty=400;
    if(g_Angle>=-10&&g_Angle<=10)
        open_bias_duty=0;
    if(g_Angle<-10&&g_Angle>-20)
        open_bias_duty=-400;
    if(g_Angle<=-20&&g_Angle>-30)
        open_bias_duty=-600;
    if(g_Angle<=-30)
        open_bias_duty=-800;
}
*/
void open_speed(){//开环速度设置控制

    if(!run_mode){
        if(speed_forward>35){
    if(g_Angle>90)
        speed_diff=8;
    else if(g_Angle>50&&g_Angle<=90)
        speed_diff=8;
    else if(g_Angle>=40&&g_Angle<=50)
        speed_diff=8;
    else if(g_Angle>=30&&g_Angle<40)
        speed_diff=7;
    else if(g_Angle>=20&&g_Angle<30)
        speed_diff=5;
    else if(g_Angle>10&&g_Angle<20)
        speed_diff=4;
    else if(g_Angle>0&&g_Angle<=10)
        speed_diff=2;
    else if(g_Angle==0)speed_diff=0;
    else if(g_Angle>=-10&&g_Angle<0)
        speed_diff=-2;
    else if(g_Angle<-10&&g_Angle>-20)
        speed_diff=-4;
    else if(g_Angle<=-20&&g_Angle>-30)
        speed_diff=-5;
    else if(g_Angle<=-30&&g_Angle>-40)
        speed_diff=-7;
    else if(g_Angle<=-40&&g_Angle>=-50)
        speed_diff=-8;
    else if(g_Angle<=-50&&g_Angle>=-90)
        speed_diff=-8;
    else if(g_Angle<-90)
        speed_diff=-8;
    }
    else{
        if(g_Angle>90)
            speed_diff=7;
        else if(g_Angle>50&&g_Angle<=90)
            speed_diff=6;
        else if(g_Angle>=40&&g_Angle<=50)
            speed_diff=6;
        else if(g_Angle>=30&&g_Angle<40)
            speed_diff=5;
        else if(g_Angle>=20&&g_Angle<30)
            speed_diff=4;
        else if(g_Angle>10&&g_Angle<20)
            speed_diff=3;
        else if(g_Angle>0&&g_Angle<=10)
            speed_diff=2;
        else if(g_Angle==0)speed_diff=0;
        else if(g_Angle>=-10&&g_Angle<0)
            speed_diff=-2;
        else if(g_Angle<-10&&g_Angle>-20)
            speed_diff=-3;
        else if(g_Angle<=-20&&g_Angle>-30)
            speed_diff=-4;
        else if(g_Angle<=-30&&g_Angle>-40)
            speed_diff=-5;
        else if(g_Angle<=-40&&g_Angle>=-50)
            speed_diff=-6;
        else if(g_Angle<=-50&&g_Angle>=-90)
            speed_diff=-6;
        else if(g_Angle<-90)
            speed_diff=-7;
    }
    }
    else{
        if(g_Angle>90)
                speed_diff=-3;
        else if(g_Angle>50&&g_Angle<=90)
            speed_diff=-3;
        else if(g_Angle>=40&&g_Angle<=50)
                speed_diff=-3;
        else if(g_Angle>=30&&g_Angle<40)
                speed_diff=-3;
        else if(g_Angle>=20&&g_Angle<30)
                speed_diff=-2;
        else if(g_Angle>10&&g_Angle<20)
                speed_diff=-1;
        else if(g_Angle>0&&g_Angle<=10)
               speed_diff=0;
        else if(g_Angle==0)speed_diff=0;
        else if(g_Angle>=-10&&g_Angle<0)
               speed_diff=0;
        else if(g_Angle<-10&&g_Angle>-20)
                speed_diff=1;
        else if(g_Angle<=-20&&g_Angle>-30)
                speed_diff=2;
        else if(g_Angle<=-30&&g_Angle>-40)
                speed_diff=3;
        else if(g_Angle<=-40&&g_Angle>=-50 )
                speed_diff=3;
        else if(g_Angle<=-50&&g_Angle>=-90)
            speed_diff=3;
        else if(g_Angle<-90)
                speed_diff=3;
    }
}
