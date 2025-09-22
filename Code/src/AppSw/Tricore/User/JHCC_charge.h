/*
 * JHCC_charge.h
 *
 *  Created on: 2023年5月14日
 *      Author: Administrator
 */

#ifndef SRC_APPSW_TRICORE_USER_JHCC_CHARGE_H_
#define SRC_APPSW_TRICORE_USER_JHCC_CHARGE_H_
extern short charge_power;
extern double battery;//电容电量
extern uint8 turn_off_flag;
extern uint8 power_flag;
extern uint8 charg_flag;
extern int time_charge;
extern short turn_off_time;




// 宏定义
#define SD P00_3                  // 使能端

// 定义变量
extern double Value_i1;
extern double Value_v1;
extern double Value_v2;

typedef struct{     //PID结构体
    float Charge_Kp;
    float Charge_Ki;
    float Charge_Kd;
}Charge_InitTypedef;

extern float Charge_Time;   //充电时间
extern float CHARGE_TIME;   //初始时间
extern float Charge_PWM;    //占空比
extern uint8 Charge_Flag;   //充电标志
extern uint8 Charge_Flag_start;
extern uint8 CHARGE_P;

//函数声明
float SeqIntPID(float Kp, float Ki, float Kd, float GiveValue, float ActualValue);
void Charge_Ctrl(float GiveValue, float ActualValue);
void Charge_Interface(float Charge_Value);
//初始化
void Charge_Init(void);
//充电，调用此即可
void Charge_Start(void);
void Chrage_PID_Init(void);
void charge_init(void);
void setup(void);

void charge_read(void);
void charge_judge(void);
#endif /* SRC_APPSW_TRICORE_USER_JHCC_CHARGE_H_ */
