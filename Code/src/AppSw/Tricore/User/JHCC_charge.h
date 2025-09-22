/*
 * JHCC_charge.h
 *
 *  Created on: 2023��5��14��
 *      Author: Administrator
 */

#ifndef SRC_APPSW_TRICORE_USER_JHCC_CHARGE_H_
#define SRC_APPSW_TRICORE_USER_JHCC_CHARGE_H_
extern short charge_power;
extern double battery;//���ݵ���
extern uint8 turn_off_flag;
extern uint8 power_flag;
extern uint8 charg_flag;
extern int time_charge;
extern short turn_off_time;




// �궨��
#define SD P00_3                  // ʹ�ܶ�

// �������
extern double Value_i1;
extern double Value_v1;
extern double Value_v2;

typedef struct{     //PID�ṹ��
    float Charge_Kp;
    float Charge_Ki;
    float Charge_Kd;
}Charge_InitTypedef;

extern float Charge_Time;   //���ʱ��
extern float CHARGE_TIME;   //��ʼʱ��
extern float Charge_PWM;    //ռ�ձ�
extern uint8 Charge_Flag;   //����־
extern uint8 Charge_Flag_start;
extern uint8 CHARGE_P;

//��������
float SeqIntPID(float Kp, float Ki, float Kd, float GiveValue, float ActualValue);
void Charge_Ctrl(float GiveValue, float ActualValue);
void Charge_Interface(float Charge_Value);
//��ʼ��
void Charge_Init(void);
//��磬���ô˼���
void Charge_Start(void);
void Chrage_PID_Init(void);
void charge_init(void);
void setup(void);

void charge_read(void);
void charge_judge(void);
#endif /* SRC_APPSW_TRICORE_USER_JHCC_CHARGE_H_ */
