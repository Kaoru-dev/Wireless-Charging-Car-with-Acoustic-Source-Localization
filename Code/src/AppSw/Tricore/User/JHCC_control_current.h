/*————————————————镜湖车厂————————————————
版本：
    2022.4.22
代码功能：
        电流环
版权信息：
        陈博，王熙平，刘毅涵
        以及所有给予支持的机器人俱乐部的前辈们！！！
—————————————————镜湖车厂———————————————*/
/*
 * JHCC_control_current.h
 *
 *  Created on: 2023年4月21日
 *      Author: 陈博
 */

#ifndef SRC_APPSW_TRICORE_USER_JHCC_CONTROL_CURRENT_H_
#define SRC_APPSW_TRICORE_USER_JHCC_CONTROL_CURRENT_H_


//-----------------------------头文件区-----------------------------
#include <include.h>

//-----------------------------宏定义区-----------------------------


//-----------------------------函数区-----------------------------
void    currentRead     (void);
void    setCurrentTo    (sint16, sint16);
void    currentPID      (void);
sint16  filter1         (sint16 value_buf[], sint16 new_value, sint16 num);
sint16  filter2         (sint16 value_buf[], sint16 new_value, sint16 num);

//-----------------------------变量定义区-----------------------------
extern sint16   currentDuty1;
extern sint16   currentDuty2;
extern sint16  currentDesired1;
extern sint16  currentDesired2;
extern sint16  currentData1;       //电流采集量
extern sint16  currentData2;       //电流采集量
extern sint16   currentActual1;     //电流（单位mA）
extern sint16   currentActual2;     //电流（单位mA）
extern float    currentKP1;         //电流环KP
extern float    currentKD1;         //电流环KD
extern float    currentKI1;         //电流环KI
extern float    currentKP2;         //电流环KP
extern float    currentKD2;         //电流环KD
extern float    currentKI2;         //电流环KI
extern uint16   currentMetro;
extern sint16   currentErr1;
extern sint16   currentErr2;
extern uint8    dontCurrentPID;     //1禁止电流环，0开启电流环
extern sint16  currentErrAccm1;
extern sint16  currentErrDiff1  ;

#endif /* SRC_APPSW_TRICORE_USER_JHCC_CONTROL_CURRENT_H_ */
