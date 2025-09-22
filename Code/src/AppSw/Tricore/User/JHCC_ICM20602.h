/*
 * JHCC_Direction.h
 *
 *  Created on: 2023年3月25日
 *      Author: 君毋语
 */

#ifndef SRC_APPSW_TRICORE_USER_JHCC_ICM20602_H_
#define SRC_APPSW_TRICORE_USER_JHCC_ICM20602_H_
#include"include.h"
extern short soundtoseek;
extern float yaw_toset;
extern float s_error;
typedef struct
{
      int Xdata;
      int Ydata;
      int Zdata;
} gyro_param_t;//陀螺仪参数

typedef struct
{
        float acc_x;
        float acc_y;
        float acc_z;
        float gyro_x;
        float gyro_y;
        float gyro_z;
} icm_param_t;//icm陀螺仪和加速度参数


typedef struct
{
    float q0;
    float q1;
    float q2;
    float q3;
}quater_param_t;//四元数

typedef struct
{
    float pitch;
    float roll;
    float yaw;
}euler_param_t;
extern euler_param_t eulerAngle;
void ICM_init(void);
void IMU_quaterToEulerianAngles(void);
void icmGetValues(void);
float Sound_error(void);
#endif /* SRC_APPSW_TRICORE_USER_JHCC_ICM20602_H_ */
