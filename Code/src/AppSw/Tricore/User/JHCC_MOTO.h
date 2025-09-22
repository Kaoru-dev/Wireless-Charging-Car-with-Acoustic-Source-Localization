/*
 * JHCC_MOTO.h
 *
 *  Created on: 2023ƒÍ3‘¬22»’
 *      Author: æ˝Œ„”Ô
 */

#ifndef SRC_APPSW_TRICORE_USER_JHCC_MOTO_H_
#define SRC_APPSW_TRICORE_USER_JHCC_MOTO_H_
#include "include.h"

#define MOTO_Freq   10000
extern uint8   run_mode;
extern uint8   lastrun_mode;
extern int test_duty;
extern sint16 Leftduty,Rightduty;
extern short speed_to_set;

void MOTO_init(void);
void MOTO_Set_duty(void);
void updateDuty(void);
void Mode_Select(void);
#endif /* SRC_APPSW_TRICORE_USER_JHCC_MOTO_H_ */
