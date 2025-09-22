/*
 * JHCC_Process.h
 *
 *  Created on: 2023��7��16��
 *      Author: ������
 */

#ifndef SRC_APPSW_TRICORE_USER_JHCC_PROCESS_H_
#define SRC_APPSW_TRICORE_USER_JHCC_PROCESS_H_
#include "include.h"


extern uint8 modechange_coolflag;
extern uint8 cooldowm_flag;
extern uint8  lastrotate_flag;
extern uint8 rotate_flag;
extern uint8 reduce_speedflag;
extern uint8 up_speedflag;
extern uint8 dontspeedselect;
extern uint8 aimangleflag1;
extern uint8 aimangleflag2;
extern short aimangle1;
extern short aimangle2;
extern uint8 lastaimangleflag1;
extern uint8 allowaimangle;
extern uint8 lastaimangleflag2;
extern uint8 buff_flag;
//**************************************************************//
extern short mode_change_cnt;
extern short modechange_cooltime;//��ͷ��ȴʱ��
extern short cooldowntime;
extern short rotate_time;
extern uint8 buff_time;
void bias_control(void);
void select_speed(void);
void modechange_speed(void);
#endif /* SRC_APPSW_TRICORE_USER_JHCC_PROCESS_H_ */
