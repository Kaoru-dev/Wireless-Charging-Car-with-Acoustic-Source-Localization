/*
 * JHCC_control.h
 *
 *  Created on: 2023年3月24日
 *      Author: 君毋语
 */

#ifndef SRC_APPSW_TRICORE_USER_JHCC_CONTROL_SPEED_H_
#define SRC_APPSW_TRICORE_USER_JHCC_CONTROL_SPEED_H_
//-----------------------------宏定义区-----------------------------


//-----------------------------函数区-----------------------------
void    speedRead       (void);
void setSpeedTo(short target_left,short target_right,short target);
void    speedPID        (void);
void    delayMilimeter  (sint32);
void speedPID_left(void) ;
void speedPID_right(void) ;

//-----------------------------变量定义区-----------------------------
extern sint16  speedDuty;
extern sint16   speedActualx2;
extern uint8    dontSpeedPID;               //0开启速度环，1屏蔽速度环
extern sint16  speedSet_left;              //左轮设定速度
extern sint16  speedSet_right;             //右轮设定速度
extern sint16   speedDuty_left;                  //速度环输出
extern sint16   speedDuty_right;                  //速度环输出
extern sint16   speedActualLeft;            //实际左轮速度
extern sint16   speedActualRight;           //实际右轮速度
extern uint8    signleft;                   //左轮转动方向
extern uint8    signRight;                  //右轮转动方向
extern sint16  speedSet;
extern float    speedGiaP;
extern float    speedGiaD;
extern float    speedGiaI;
extern short    speed_forward;//速度基准
extern short    speed_diff;//速度差速
extern float    speedSmaP;
extern float    speedSmaD;
extern float    speedSmaI;





#endif /* SRC_APPSW_TRICORE_USER_JHCC_CONTROL_SPEED_H_ */
