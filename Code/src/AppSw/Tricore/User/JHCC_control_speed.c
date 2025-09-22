/*
 * JHCC_control.c
 *
 *  Created on: 2023年3月24日
 *      Author: 君毋语
 */
//-----------------------------头文件区-----------------------------
#include <include.h>

//-----------------------------宏定义区-----------------------------
#define INT_SATURATION      500     //对速度误差的积分饱和
//-----------------------------变量定义区-----------------------------
//速度环PID参数50,95,2；
float   speedGiaP           = 100;
float   speedGiaD           = 4;
float   speedGiaI           = 30;

float   speedSmaP           = 40;
float   speedSmaD           = 0;
float   speedSmaI           = 0;

//速度环变量
uint8   dontSpeedPID        = 0;    //0开启速度环，1屏蔽速度环
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
sint16  speedErr_left       = 0;    //左偏差量
sint16  speedErr_right       = 0;   //右偏差量
sint16  speedDifferential_left   = 0;    //偏差量后向差分
sint16  speedDifferential2_left  = 0;    //偏差量二阶后向差分
sint16  speedDifferential_right   = 0;    //偏差量后向差分
sint16  speedDifferential2_right  = 0;    //偏差量二阶后向差分
sint16  speedDuty_left           = 0;    //速度环的占空比
sint16  speedDuty_right           = 0;    //速度环的占空比
sint16  speedDutyAccI_left       = 0;    //速度环I累加
sint16  speedDutyAccPD_left      = 0;    //速度环P和D累加
sint16  speedDutyAccI_right      = 0;    //速度环I累加
sint16  speedDutyAccPD_right      = 0;    //速度环P和D累加
sint16  speedErrLast_left        = 0;    //前一次偏差量
sint16  speedErrLast_right        = 0;    //前一次偏差量
sint16  speedErrLastLast_left    = 0;    //上上一次偏差量
sint16  speedErrLastLast_right    = 0;    //上上一次偏差量

//速度读取
sint16  speedSet=40;                   //设定速度
sint16  speedSet_left;              //左轮设定速度
sint16  speedSet_right;             //右轮设定速度
sint16  speedActualLeft;            //实际左轮速度
sint16  speedActualRight;           //实际右轮速度
sint16  speedActualx2;              //实际速度
sint32  speedSumx2;                 //速度累加，即距离
uint8   signleft = 1;               //左轮转动方向
uint8   signRight = 1;              //右轮转动方向


short speed_forward=25;
short speed_diff=0;
//-----------------------------函数区-----------------------------

//速度期望值设置
void setSpeedTo(short target_left,short target_right,short target)
{
   // speedSet_left = target_left;
    speedSet_left = target_left;
    speedSet_right=target_right;
    speedSet=target;
}

//读取速度
void speedRead(void)
{
    if(!run_mode){
    //获得编码器计数ENC6_InPut_P20_3
    speedActualRight  = ENC_GetCounter(ENC5_InPut_P10_3);
    speedActualLeft = -ENC_GetCounter(ENC6_InPut_P20_3);
    //相加得到小车重心的速度的两倍
    speedActualx2 = speedActualLeft + speedActualRight;
    }
    else{
    speedActualRight  = -ENC_GetCounter(ENC5_InPut_P10_3);
    speedActualLeft = +ENC_GetCounter(ENC6_InPut_P20_3);
           //相加得到小车重心的速度的两倍
    speedActualx2 = speedActualLeft + speedActualRight;
    }
}

//速度环，增量式PID

void speedPID(void) {
    //计算控制量
    speedErr = speedSet - speedActualx2;
    speedDifferential = speedErr - speedErrLast;
    speedDifferential2 = speedErr - 2 * speedErrLast + speedErrLastLast;
        speedDutyAccI += speedGiaI * speedErr;
        if (speedDutyAccI > 7000) speedDutyAccI = 7000;
        else if(speedDutyAccI < -7000) speedDutyAccI = -7000;
        //if(speedActualx2<1)speedDutyAccI=0;

        speedDutyAccPD += speedGiaP * speedDifferential + speedGiaD * speedDifferential2;

        speedDuty = speedDutyAccI + speedDutyAccPD;


    //限幅
    if (speedDuty > 8700)speedDuty = 8700;
    else if (speedDuty < -8700)speedDuty = -8700;
    speedErrLastLast = speedErrLast;
    speedErrLast = speedErr;
//    if(buff_flag){//方向变换后对值清空
//        speedErr=0;
//        speedDifferential=0;
//        speedDifferential2=0;
//        speedDutyAccI=0;
//    }
}

////左轮速度环pid
void speedPID_left(void) {
    //计算控制量
    speedErr_left = speedSet_left - speedActualLeft;
    speedDifferential_left = speedErr_left - speedErrLast_left;
    speedDifferential2_left = speedErr_left - 2 * speedErrLast_left + speedErrLastLast_left;
        speedDutyAccI_left += speedGiaI * speedErr_left;
        if (speedDutyAccI_left > 8700) speedDutyAccI_left = 8700;
        else if(speedDutyAccI_left < -8700) speedDutyAccI_left = -8700;
        //if(speedActualx2<1)speedDutyAccI=0;
        speedDutyAccPD_left += speedGiaP * speedDifferential_left + speedGiaD * speedDifferential2_left;
        speedDuty_left = speedDutyAccI_left + speedDutyAccPD_left;
    //限幅
    if (speedDuty_left > 8700)speedDuty_left = 8700;
    else if (speedDuty_left < -8700)speedDuty_left = -8700;
    speedErrLastLast_left = speedErrLast_left;
    speedErrLast_left = speedErr_left;
        if(buff_flag){//方向变换后对值清空
          speedErr_left=0;
          speedDifferential_left=0;
          speedDifferential2_left=0;
          speedDutyAccI_left=0;
        }
}
//右轮速度环
void speedPID_right(void) {
    //计算控制量
    speedErr_right = speedSet_right - speedActualRight;
    speedDifferential_right = speedErr_right - speedErrLast_right;
    speedDifferential2_right = speedErr_right - 2 * speedErrLast_right + speedErrLastLast_right;
        speedDutyAccI_right += speedGiaI * speedErr_right;
        if (speedDutyAccI_right > 8700) speedDutyAccI_right = 8700;
        else if(speedDutyAccI_right < -8700) speedDutyAccI_right = -8700;
        //if(speedActualx2<1)speedDutyAccI=0;
        speedDutyAccPD_right += speedGiaP * speedDifferential_right + speedGiaD * speedDifferential2_right;
        speedDuty_right = speedDutyAccI_right+ speedDutyAccPD_right;
    //限幅
    if (speedDuty_right > 8700)speedDuty_right = 8700;
    else if (speedDuty_right < -8700)speedDuty_right = -8700;
    speedErrLastLast_right = speedErrLast_right;
    speedErrLast_right = speedErr_right;
    if(buff_flag){//方向变换后对值清空
        speedErr_right=0;
        speedDifferential_right=0;
        speedDifferential2_right=0;
        speedDutyAccI_right=0;
    }
}


