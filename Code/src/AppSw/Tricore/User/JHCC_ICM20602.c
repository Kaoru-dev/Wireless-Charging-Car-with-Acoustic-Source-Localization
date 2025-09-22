/*
 * JHCC_Direction.c
 *
 *  Created on: 2023年3月25日
 *      Author: 君毋语
 */
#include"include.h"
#include "LQ_ICM20602.h"
#define delta_T     0.02f  // 采样周期1ms 即频率1KHZ
#define PI          3.1415926f
#define ICM20602_ACC_SAMPLE         (0x08)                                      // 加速度计量程
// 设置为:0x00 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x08 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x10 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x18 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)

#define ICM20602_GYR_SAMPLE         (0x18)                                      // 陀螺仪量程
// 设置为:0x00 陀螺仪量程为:±250 dps     获取到的陀螺仪数据除以131           可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x08 陀螺仪量程为:±500 dps     获取到的陀螺仪数据除以65.5          可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x10 陀螺仪量程为:±1000dps     获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x18 陀螺仪量程为:±2000dps     获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s

/********************************************************陀螺仪零点漂移初始化*********************************************************/

gyro_param_t GyroOffset;               // 陀螺仪校准值
short soundtoseek=0;
/**
 * @brief 陀螺仪零漂初始化
 * 通过采集一定数据求均值计算陀螺仪零点偏移值。
 * 后续 陀螺仪读取的数据 - 零飘值，即可去除零点偏移量。
 */
void gyroOffsetInit(void)
{
    short gyrox,gyroy,gyroz;        //陀螺仪原始数据
    GyroOffset.Xdata = 0;
    GyroOffset.Ydata = 0;
    GyroOffset.Zdata = 0;
    for (uint16_t i = 0; i < 200; ++i)
    {
        ICM_Get_Gyroscope(&gyrox,&gyroy,&gyroz);    // 获取陀螺仪角速度
        GyroOffset.Xdata += gyrox;
        GyroOffset.Ydata += gyroy;
        GyroOffset.Zdata += gyroz;
        delayms(5);    // 最大 1Khz
    }

    GyroOffset.Xdata /= 50;
    GyroOffset.Ydata /= 50;
    GyroOffset.Zdata /= 50;
    GyroOffset.Xdata= 0;
   GyroOffset.Ydata = 0;
   GyroOffset.Zdata = 0;
}

//float Icm20602_acc_transition (short  acc_value)
//{
//    float acc_data = 0;
//    switch(ICM20602_ACC_SAMPLE)
//    {
//        case 0x00: acc_data = (float)acc_value / 16384; break;      // 0x00 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)
//        case 0x08: acc_data = (float)acc_value / 8192;  break;      // 0x08 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
//        case 0x10: acc_data = (float)acc_value / 4096;  break;      // 0x10 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
//        case 0x18: acc_data = (float)acc_value / 2048;  break;      // 0x18 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)
//        default: break;
//    }
//    return acc_data;
//}
//
////-------------------------------------------------------------------------------------------------------------------
//// 函数简介     将 ICM20602 陀螺仪数据转换为实际物理数据
//// 参数说明     gyro_value              // 任意轴的陀螺仪数据
//// 返回参数     void
//// 使用示例     float data = icm20602_gyro_transition(imu660ra_gyro_x);  // 单位为°/s
//// 备注信息
////-------------------------------------------------------------------------------------------------------------------
//float icm20602_gyro_transition (short gyro_value)
//{
//    float gyro_data = 0;
//    switch(ICM20602_GYR_SAMPLE)
//    {
//        case 0x00: gyro_data = ((float)gyro_value- GyroOffset.Xdata) / 131.2f;  break;  //  0x00 陀螺仪量程为:±250 dps     获取到的陀螺仪数据除以131           可以转化为带物理单位的数据，单位为：°/s
//        case 0x08: gyro_data = ((float)gyro_value- GyroOffset.Xdata)/ 65.6f;   break;  //  0x08 陀螺仪量程为:±500 dps     获取到的陀螺仪数据除以65.5          可以转化为带物理单位的数据，单位为：°/s
//        case 0x10: gyro_data = ((float)gyro_value- GyroOffset.Xdata)/ 32.8f;   break;  //  0x10 陀螺仪量程为:±1000dps     获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
//        case 0x18: gyro_data = ((float)gyro_value- GyroOffset.Xdata)/ 16.4f;   break;  //  0x18 陀螺仪量程为:±2000dps     获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s
//        default: break;
//    }
//    return gyro_data;
//}


/********************************************************互补滤波算法************************************************************/

float I_ex, I_ey, I_ez;  // 误差积分
quater_param_t Q_info = {1, 0, 0, 0};  // 四元数初始化
euler_param_t eulerAngle;              // 欧拉角
icm_param_t icm_data;                  // ICM20602采集的六轴数值
float icm_kp= 10.0;    // 加速度计的收敛速率比例增益
float icm_ki=0.20;   // 陀螺仪收敛速率的积分增益






//求导数的平方根
float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}



/*******************将陀螺仪值转换为实际物理量*********************************/
/*加速度一阶滤波
 * 陀螺仪去除漂移
 * */
void icmGetValues(void)
{
    float alpha = 0.7;
    short gyrox,gyroy,gyroz,accx,accy,accz;        //陀螺仪原始数据
       ICM_Get_Raw_data(&accx,&accy,&accz,&gyrox,&gyroy,&gyroz);
        //ANO_DT_send_int16(gyrox,gyroy,gyroz,accx,accy,accz,battery,Charge_Flag);
    //一阶低通滤波，单位g
    icm_data.acc_x = (((float) accx) * alpha) / 8192 + icm_data.acc_x * (1 - alpha);
    icm_data.acc_y = (((float) accy) * alpha) / 8192 + icm_data.acc_y * (1 - alpha);
    icm_data.acc_z = (((float) accz) * alpha) / 8192 + icm_data.acc_z * (1 - alpha);

    //! 陀螺仪角速度必须转换为弧度制角速度: deg/s -> rad/s
    icm_data.gyro_x = ((float) gyrox - GyroOffset.Xdata) * PI / 180 / 16.4f;
    icm_data.gyro_y = ((float) gyroy - GyroOffset.Ydata) * PI / 180 / 16.4f;
    icm_data.gyro_z = ((float) gyroz - GyroOffset.Zdata) * PI / 180 / 16.4f;
//    ANO_DT_send_int16(icm_data.gyro_x,icm_data.gyro_y,icm_data.gyro_z,accx,accy,accz,battery,Charge_Flag);
}

// * @brief 用互补滤波算法解算陀螺仪姿态(即利用加速度计修正陀螺仪的积分误差)
// * 加速度计对振动之类的噪声比较敏感，长期数据计算出的姿态可信；陀螺仪对振动噪声不敏感，短期数据可信，但长期使用积分误差严重(内部积分算法放大静态误差)。
// * 因此使用姿态互补滤波，短期相信陀螺仪，长期相信加速度计。
// * @tips: n - 导航坐标系； b - 载体坐标系
// */
void icmAHRSupdate(icm_param_t* icm)
{
    float halfT = 0.5 * delta_T;    // 采样周期一半
    float vx, vy, vz;               // 当前姿态计算得来的重力在三轴上的分量,加速度
    float ex, ey, ez;               // 当前加速计测得的重力加速度在三轴上的分量与用当前姿态计算得来的重力在三轴上的分量的误差

    float q0 = Q_info.q0;  //四元数
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;

    float q0q0 = q0 * q0;  //先相乘，方便后续计算
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    // 正常静止状态为-g 反作用力。
    if(icm->acc_x * icm->acc_y * icm->acc_z == 0) // 加计处于自由落体状态时(此时g = 0)不进行姿态解算，因为会产生分母无穷大的情况
        return;

    // 对加速度数据进行归一化 得到单位加速度 (a^b -> 载体坐标系下的加速度)
    float norm = invSqrt(icm->acc_x * icm->acc_x + icm->acc_y * icm->acc_y + icm->acc_z * icm->acc_z);
    icm->acc_x = icm->acc_x * norm;
    icm->acc_y = icm->acc_y * norm;
    icm->acc_z = icm->acc_z * norm;

    // 载体坐标系下重力在三个轴上的分量
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    // g^b 与 a^b 做向量叉乘，得到陀螺仪的校正补偿向量e的系数
    ex = icm->acc_y * vz - icm->acc_z * vy;
    ey = icm->acc_z * vx - icm->acc_x * vz;
    ez = icm->acc_x * vy - icm->acc_y * vx;

    // 误差累加
    I_ex += halfT * ex;
    I_ey += halfT * ey;
    I_ez += halfT * ez;

    // 使用PI控制器消除向量积误差(陀螺仪漂移误差)
    icm->gyro_x = icm->gyro_x + icm_kp* ex + icm_ki* I_ex;
    icm->gyro_y = icm->gyro_y + icm_kp* ey + icm_ki* I_ey;
    icm->gyro_z = icm->gyro_z + icm_kp* ez + icm_ki* I_ez;

    // 一阶龙格库塔法求解四元数微分方程，其中halfT为测量周期的1/2，gx gy gz为b系陀螺仪角速度。
    q0 = q0 + (-q1 * icm->gyro_x - q2 * icm->gyro_y - q3 * icm->gyro_z) * halfT;
    q1 = q1 + (q0 * icm->gyro_x + q2 * icm->gyro_z - q3 * icm->gyro_y) * halfT;
    q2 = q2 + (q0 * icm->gyro_y - q1 * icm->gyro_z + q3 * icm->gyro_x) * halfT;
    q3 = q3 + (q0 * icm->gyro_z + q1 * icm->gyro_y - q2 * icm->gyro_x) * halfT;

    // 单位化四元数在空间旋转时不会拉伸，仅有旋转角度，下面算法类似线性代数里的正交变换
    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    Q_info.q0 = q0 * norm;
    Q_info.q1 = q1 * norm;
    Q_info.q2 = q2 * norm;
    Q_info.q3 = q3 * norm;  // 用全局变量记录上一次计算的四元数值
}

/*四元数转换为欧拉角*/


void ICM_init(void)
{

    QSPI_InitConfig(QSPI2_CLK_P15_8, QSPI2_MISO_P15_7, QSPI2_MOSI_P15_5, QSPI2_CS_P15_2, 5000000, 3);
    if(ICM20602_Init())
    {
     // UART_PutStr(UART0, "icm_fail");
    }
   // UART_PutStr(UART0, "icm_yes");
    gyroOffsetInit();//零点漂移
    //delayms(100);
    //STM_InitConfig(STM1, STM_Channel_1, 10000);//每10ms进入一次，频率0.1khz
}

void IMU_quaterToEulerianAngles(void)
{

  //  UART_PutStr(UART0, "icm_yes");
    icmGetValues();
    icmAHRSupdate(&icm_data);
        //四元数
        float q0 = Q_info.q0;
        float q1 = Q_info.q1;
        float q2 = Q_info.q2;
        float q3 = Q_info.q3;
        //换算欧拉角
//        eulerAngle.pitch = asin(-2*q1*q3 + 2*q0*q2) * 180/PI;                        // pitch
//        eulerAngle.roll = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1) * 180/PI; // roll
        eulerAngle.yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 180 / PI;
      //  ANO_DT_send_int16(eulerAngle.yaw, eulerAngle.roll,  eulerAngle.pitch, 100, 0, 0, 0, 100);
}
/*右偏为正
 * 偏航角左偏为正，右偏为负，每方最多180度，超过界限转换符号
 */
//测试函数，使小车左偏45度，小车偏离45左为正，右为负
float opp_angle;
float yaw_toset,s_error;//yaw_toset=为eulerAngle.yaw+g_Angle
short   aimangle_1;
short   aimangle_2;
uint8   update_flag=1;
float Sound_error(void)
{
//        if(aimangleflag1){   //开启预知转向
//        soundtoseek=aimangle1;//预知转向角度
//        allowaimangle=1;
//        }
//        if(aimangleflag2)
    if(g_Angle>=160)
        soundtoseek=g_Angle*0.85;
    else if(g_Angle<=-160)
        soundtoseek=g_Angle*0.85;
    else
        soundtoseek=g_Angle*0.9;//叠加转向角度
//    else
//    soundtoseek=g_Angle;
    //eulerAngle.yaw是绝对相对偏角，需要一个相对角
    //角度归正
//    if(aimangleflag1){
//        if(aimangleflag1&&!lastaimangleflag1&&allowaimangle)//每0.2s更新一次角度
//        {
//            yaw_toset=eulerAngle.yaw+soundtoseek;
//            if(yaw_toset>180)
//                yaw_toset-=360;
//            else if(yaw_toset<-180)
//                yaw_toset+=360;
//                s_error=yaw_toset-eulerAngle.yaw;//
//        }
//        else if(aimangleflag1&&aimangleflag1)//更新误差
//        {
//                s_error=yaw_toset-eulerAngle.yaw;//
//        }
//    }
//    if(aimangleflag2){
        if(rotate_flag&&!lastrotate_flag)//每0.2s更新一次角度
            {
                yaw_toset=eulerAngle.yaw+g_Angle;
                if(yaw_toset>180)
                    yaw_toset-=360;
                else if(yaw_toset<-180)
                    yaw_toset+=360;
                s_error=yaw_toset-eulerAngle.yaw;//
                allowaimangle=0;
            }
            else if(rotate_flag&&lastrotate_flag)//更新误差
            {
                s_error=yaw_toset-eulerAngle.yaw;//
            }

   /*特殊情况
    * 1：当soundtoseek=180->-180
        * 若要转到180度eulerAngle.yaw=170，soundtoseek=10，yaw_toset=180，s_error=10，9，8，，，0，360，359，此时判断到大于180，-360后为-1
        *  若要转到-180度eulerAngle.yaw=-170，soundtoseek=-10，yaw_toset=-180，s_error=-10，-9，-8，，，0，-360，-359，此时判断到大于180，-360后为-1
    */
    if(s_error>180)
        {
            s_error-=360;
        }
        else if(s_error<-180)
        {
            s_error+=360;
        }
    //ANO_DT_send_int16(g_Angle,eulerAngle.yaw,s_error,0,0,0,0,0);
        return (s_error);
}

/*转向控制策略：角度突变
 * 如果角度过大，使用陀螺仪一次性转到目标角度，但是由于角度突然改变会导致一定的不稳定性，所以在改变角度后的1s内不能再使用
 *
 */
