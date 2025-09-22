/*
 * JHCC_control_dir.h
 *
 *  Created on: 2023年3月26日
 *      Author: 君毋语
 */

#ifndef SRC_APPSW_TRICORE_USER_JHCC_CONTROL_DIR_H_
#define SRC_APPSW_TRICORE_USER_JHCC_CONTROL_DIR_H_
extern sint16 biasIndBigFastP ;
extern sint16 biasIndBigFastD  ;
extern float  biasIndBigFastPD  ;
extern sint16  open_bias_duty;
//方向环变量
extern uint8  dontBiasPID ;      //0开启方向环，1屏蔽方向环
extern uint8  dontBiasPID1 ;      //0开启方向环，1屏蔽方向环
extern float biasSoundErr ;      //摄像头偏移度偏差量(左拐是负，右拐是正)
extern  float  biasSoundDifferential;   //声音差值的差分，代替微分
extern float  biasSoundErrLast ;      //前一次声音偏差量
extern sint16 biasDuty ;      //方向环的占空比
extern float icmDuty;
extern  float   icmErr;
void biasPID_low(void);
void biasPID(void);
void speedbiasPID(void);
void open_bias(void);
void open_speed(void);
#endif /* SRC_APPSW_TRICORE_USER_JHCC_CONTROL_DIR_H_ */
