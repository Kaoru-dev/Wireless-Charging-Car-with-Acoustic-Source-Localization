/*
 * JHCC_Direction.c
 *
 *  Created on: 2023��3��25��
 *      Author: ������
 */
#include"include.h"
#include "LQ_ICM20602.h"
#define delta_T     0.02f  // ��������1ms ��Ƶ��1KHZ
#define PI          3.1415926f
#define ICM20602_ACC_SAMPLE         (0x08)                                      // ���ٶȼ�����
// ����Ϊ:0x00 ���ٶȼ�����Ϊ:��2g          ��ȡ���ļ��ٶȼ����� ����16384      ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
// ����Ϊ:0x08 ���ٶȼ�����Ϊ:��4g          ��ȡ���ļ��ٶȼ����� ����8192       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
// ����Ϊ:0x10 ���ٶȼ�����Ϊ:��8g          ��ȡ���ļ��ٶȼ����� ����4096       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
// ����Ϊ:0x18 ���ٶȼ�����Ϊ:��16g         ��ȡ���ļ��ٶȼ����� ����2048       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)

#define ICM20602_GYR_SAMPLE         (0x18)                                      // ����������
// ����Ϊ:0x00 ����������Ϊ:��250 dps     ��ȡ�������������ݳ���131           ����ת��Ϊ������λ�����ݣ���λΪ����/s
// ����Ϊ:0x08 ����������Ϊ:��500 dps     ��ȡ�������������ݳ���65.5          ����ת��Ϊ������λ�����ݣ���λΪ����/s
// ����Ϊ:0x10 ����������Ϊ:��1000dps     ��ȡ�������������ݳ���32.8          ����ת��Ϊ������λ�����ݣ���λΪ����/s
// ����Ϊ:0x18 ����������Ϊ:��2000dps     ��ȡ�������������ݳ���16.4          ����ת��Ϊ������λ�����ݣ���λΪ����/s

/********************************************************���������Ư�Ƴ�ʼ��*********************************************************/

gyro_param_t GyroOffset;               // ������У׼ֵ
short soundtoseek=0;
/**
 * @brief ��������Ư��ʼ��
 * ͨ���ɼ�һ���������ֵ�������������ƫ��ֵ��
 * ���� �����Ƕ�ȡ������ - ��Ʈֵ������ȥ�����ƫ������
 */
void gyroOffsetInit(void)
{
    short gyrox,gyroy,gyroz;        //������ԭʼ����
    GyroOffset.Xdata = 0;
    GyroOffset.Ydata = 0;
    GyroOffset.Zdata = 0;
    for (uint16_t i = 0; i < 200; ++i)
    {
        ICM_Get_Gyroscope(&gyrox,&gyroy,&gyroz);    // ��ȡ�����ǽ��ٶ�
        GyroOffset.Xdata += gyrox;
        GyroOffset.Ydata += gyroy;
        GyroOffset.Zdata += gyroz;
        delayms(5);    // ��� 1Khz
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
//        case 0x00: acc_data = (float)acc_value / 16384; break;      // 0x00 ���ٶȼ�����Ϊ:��2g          ��ȡ���ļ��ٶȼ����� ����16384      ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
//        case 0x08: acc_data = (float)acc_value / 8192;  break;      // 0x08 ���ٶȼ�����Ϊ:��4g          ��ȡ���ļ��ٶȼ����� ����8192       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
//        case 0x10: acc_data = (float)acc_value / 4096;  break;      // 0x10 ���ٶȼ�����Ϊ:��8g          ��ȡ���ļ��ٶȼ����� ����4096       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
//        case 0x18: acc_data = (float)acc_value / 2048;  break;      // 0x18 ���ٶȼ�����Ϊ:��16g         ��ȡ���ļ��ٶȼ����� ����2048       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
//        default: break;
//    }
//    return acc_data;
//}
//
////-------------------------------------------------------------------------------------------------------------------
//// �������     �� ICM20602 ����������ת��Ϊʵ����������
//// ����˵��     gyro_value              // �����������������
//// ���ز���     void
//// ʹ��ʾ��     float data = icm20602_gyro_transition(imu660ra_gyro_x);  // ��λΪ��/s
//// ��ע��Ϣ
////-------------------------------------------------------------------------------------------------------------------
//float icm20602_gyro_transition (short gyro_value)
//{
//    float gyro_data = 0;
//    switch(ICM20602_GYR_SAMPLE)
//    {
//        case 0x00: gyro_data = ((float)gyro_value- GyroOffset.Xdata) / 131.2f;  break;  //  0x00 ����������Ϊ:��250 dps     ��ȡ�������������ݳ���131           ����ת��Ϊ������λ�����ݣ���λΪ����/s
//        case 0x08: gyro_data = ((float)gyro_value- GyroOffset.Xdata)/ 65.6f;   break;  //  0x08 ����������Ϊ:��500 dps     ��ȡ�������������ݳ���65.5          ����ת��Ϊ������λ�����ݣ���λΪ����/s
//        case 0x10: gyro_data = ((float)gyro_value- GyroOffset.Xdata)/ 32.8f;   break;  //  0x10 ����������Ϊ:��1000dps     ��ȡ�������������ݳ���32.8          ����ת��Ϊ������λ�����ݣ���λΪ����/s
//        case 0x18: gyro_data = ((float)gyro_value- GyroOffset.Xdata)/ 16.4f;   break;  //  0x18 ����������Ϊ:��2000dps     ��ȡ�������������ݳ���16.4          ����ת��Ϊ������λ�����ݣ���λΪ����/s
//        default: break;
//    }
//    return gyro_data;
//}


/********************************************************�����˲��㷨************************************************************/

float I_ex, I_ey, I_ez;  // ������
quater_param_t Q_info = {1, 0, 0, 0};  // ��Ԫ����ʼ��
euler_param_t eulerAngle;              // ŷ����
icm_param_t icm_data;                  // ICM20602�ɼ���������ֵ
float icm_kp= 10.0;    // ���ٶȼƵ��������ʱ�������
float icm_ki=0.20;   // �������������ʵĻ�������






//������ƽ����
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



/*******************��������ֵת��Ϊʵ��������*********************************/
/*���ٶ�һ���˲�
 * ������ȥ��Ư��
 * */
void icmGetValues(void)
{
    float alpha = 0.7;
    short gyrox,gyroy,gyroz,accx,accy,accz;        //������ԭʼ����
       ICM_Get_Raw_data(&accx,&accy,&accz,&gyrox,&gyroy,&gyroz);
        //ANO_DT_send_int16(gyrox,gyroy,gyroz,accx,accy,accz,battery,Charge_Flag);
    //һ�׵�ͨ�˲�����λg
    icm_data.acc_x = (((float) accx) * alpha) / 8192 + icm_data.acc_x * (1 - alpha);
    icm_data.acc_y = (((float) accy) * alpha) / 8192 + icm_data.acc_y * (1 - alpha);
    icm_data.acc_z = (((float) accz) * alpha) / 8192 + icm_data.acc_z * (1 - alpha);

    //! �����ǽ��ٶȱ���ת��Ϊ�����ƽ��ٶ�: deg/s -> rad/s
    icm_data.gyro_x = ((float) gyrox - GyroOffset.Xdata) * PI / 180 / 16.4f;
    icm_data.gyro_y = ((float) gyroy - GyroOffset.Ydata) * PI / 180 / 16.4f;
    icm_data.gyro_z = ((float) gyroz - GyroOffset.Zdata) * PI / 180 / 16.4f;
//    ANO_DT_send_int16(icm_data.gyro_x,icm_data.gyro_y,icm_data.gyro_z,accx,accy,accz,battery,Charge_Flag);
}

// * @brief �û����˲��㷨������������̬(�����ü��ٶȼ����������ǵĻ������)
// * ���ٶȼƶ���֮��������Ƚ����У��������ݼ��������̬���ţ������Ƕ������������У��������ݿ��ţ�������ʹ�û����������(�ڲ������㷨�Ŵ�̬���)��
// * ���ʹ����̬�����˲����������������ǣ��������ż��ٶȼơ�
// * @tips: n - ��������ϵ�� b - ��������ϵ
// */
void icmAHRSupdate(icm_param_t* icm)
{
    float halfT = 0.5 * delta_T;    // ��������һ��
    float vx, vy, vz;               // ��ǰ��̬��������������������ϵķ���,���ٶ�
    float ex, ey, ez;               // ��ǰ���ټƲ�õ��������ٶ��������ϵķ������õ�ǰ��̬��������������������ϵķ��������

    float q0 = Q_info.q0;  //��Ԫ��
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;

    float q0q0 = q0 * q0;  //����ˣ������������
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    // ������ֹ״̬Ϊ-g ����������
    if(icm->acc_x * icm->acc_y * icm->acc_z == 0) // �Ӽƴ�����������״̬ʱ(��ʱg = 0)��������̬���㣬��Ϊ�������ĸ���������
        return;

    // �Լ��ٶ����ݽ��й�һ�� �õ���λ���ٶ� (a^b -> ��������ϵ�µļ��ٶ�)
    float norm = invSqrt(icm->acc_x * icm->acc_x + icm->acc_y * icm->acc_y + icm->acc_z * icm->acc_z);
    icm->acc_x = icm->acc_x * norm;
    icm->acc_y = icm->acc_y * norm;
    icm->acc_z = icm->acc_z * norm;

    // ��������ϵ���������������ϵķ���
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    // g^b �� a^b ��������ˣ��õ������ǵ�У����������e��ϵ��
    ex = icm->acc_y * vz - icm->acc_z * vy;
    ey = icm->acc_z * vx - icm->acc_x * vz;
    ez = icm->acc_x * vy - icm->acc_y * vx;

    // ����ۼ�
    I_ex += halfT * ex;
    I_ey += halfT * ey;
    I_ez += halfT * ez;

    // ʹ��PI�������������������(������Ư�����)
    icm->gyro_x = icm->gyro_x + icm_kp* ex + icm_ki* I_ex;
    icm->gyro_y = icm->gyro_y + icm_kp* ey + icm_ki* I_ey;
    icm->gyro_z = icm->gyro_z + icm_kp* ez + icm_ki* I_ez;

    // һ����������������Ԫ��΢�ַ��̣�����halfTΪ�������ڵ�1/2��gx gy gzΪbϵ�����ǽ��ٶȡ�
    q0 = q0 + (-q1 * icm->gyro_x - q2 * icm->gyro_y - q3 * icm->gyro_z) * halfT;
    q1 = q1 + (q0 * icm->gyro_x + q2 * icm->gyro_z - q3 * icm->gyro_y) * halfT;
    q2 = q2 + (q0 * icm->gyro_y - q1 * icm->gyro_z + q3 * icm->gyro_x) * halfT;
    q3 = q3 + (q0 * icm->gyro_z + q1 * icm->gyro_y - q2 * icm->gyro_x) * halfT;

    // ��λ����Ԫ���ڿռ���תʱ�������죬������ת�Ƕȣ������㷨�������Դ�����������任
    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    Q_info.q0 = q0 * norm;
    Q_info.q1 = q1 * norm;
    Q_info.q2 = q2 * norm;
    Q_info.q3 = q3 * norm;  // ��ȫ�ֱ�����¼��һ�μ������Ԫ��ֵ
}

/*��Ԫ��ת��Ϊŷ����*/


void ICM_init(void)
{

    QSPI_InitConfig(QSPI2_CLK_P15_8, QSPI2_MISO_P15_7, QSPI2_MOSI_P15_5, QSPI2_CS_P15_2, 5000000, 3);
    if(ICM20602_Init())
    {
     // UART_PutStr(UART0, "icm_fail");
    }
   // UART_PutStr(UART0, "icm_yes");
    gyroOffsetInit();//���Ư��
    //delayms(100);
    //STM_InitConfig(STM1, STM_Channel_1, 10000);//ÿ10ms����һ�Σ�Ƶ��0.1khz
}

void IMU_quaterToEulerianAngles(void)
{

  //  UART_PutStr(UART0, "icm_yes");
    icmGetValues();
    icmAHRSupdate(&icm_data);
        //��Ԫ��
        float q0 = Q_info.q0;
        float q1 = Q_info.q1;
        float q2 = Q_info.q2;
        float q3 = Q_info.q3;
        //����ŷ����
//        eulerAngle.pitch = asin(-2*q1*q3 + 2*q0*q2) * 180/PI;                        // pitch
//        eulerAngle.roll = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1) * 180/PI; // roll
        eulerAngle.yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 180 / PI;
      //  ANO_DT_send_int16(eulerAngle.yaw, eulerAngle.roll,  eulerAngle.pitch, 100, 0, 0, 0, 100);
}
/*��ƫΪ��
 * ƫ������ƫΪ������ƫΪ����ÿ�����180�ȣ���������ת������
 */
//���Ժ�����ʹС����ƫ45�ȣ�С��ƫ��45��Ϊ������Ϊ��
float opp_angle;
float yaw_toset,s_error;//yaw_toset=ΪeulerAngle.yaw+g_Angle
short   aimangle_1;
short   aimangle_2;
uint8   update_flag=1;
float Sound_error(void)
{
//        if(aimangleflag1){   //����Ԥ֪ת��
//        soundtoseek=aimangle1;//Ԥ֪ת��Ƕ�
//        allowaimangle=1;
//        }
//        if(aimangleflag2)
    if(g_Angle>=160)
        soundtoseek=g_Angle*0.85;
    else if(g_Angle<=-160)
        soundtoseek=g_Angle*0.85;
    else
        soundtoseek=g_Angle*0.9;//����ת��Ƕ�
//    else
//    soundtoseek=g_Angle;
    //eulerAngle.yaw�Ǿ������ƫ�ǣ���Ҫһ����Խ�
    //�Ƕȹ���
//    if(aimangleflag1){
//        if(aimangleflag1&&!lastaimangleflag1&&allowaimangle)//ÿ0.2s����һ�νǶ�
//        {
//            yaw_toset=eulerAngle.yaw+soundtoseek;
//            if(yaw_toset>180)
//                yaw_toset-=360;
//            else if(yaw_toset<-180)
//                yaw_toset+=360;
//                s_error=yaw_toset-eulerAngle.yaw;//
//        }
//        else if(aimangleflag1&&aimangleflag1)//�������
//        {
//                s_error=yaw_toset-eulerAngle.yaw;//
//        }
//    }
//    if(aimangleflag2){
        if(rotate_flag&&!lastrotate_flag)//ÿ0.2s����һ�νǶ�
            {
                yaw_toset=eulerAngle.yaw+g_Angle;
                if(yaw_toset>180)
                    yaw_toset-=360;
                else if(yaw_toset<-180)
                    yaw_toset+=360;
                s_error=yaw_toset-eulerAngle.yaw;//
                allowaimangle=0;
            }
            else if(rotate_flag&&lastrotate_flag)//�������
            {
                s_error=yaw_toset-eulerAngle.yaw;//
            }

   /*�������
    * 1����soundtoseek=180->-180
        * ��Ҫת��180��eulerAngle.yaw=170��soundtoseek=10��yaw_toset=180��s_error=10��9��8������0��360��359����ʱ�жϵ�����180��-360��Ϊ-1
        *  ��Ҫת��-180��eulerAngle.yaw=-170��soundtoseek=-10��yaw_toset=-180��s_error=-10��-9��-8������0��-360��-359����ʱ�жϵ�����180��-360��Ϊ-1
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

/*ת����Ʋ��ԣ��Ƕ�ͻ��
 * ����Ƕȹ���ʹ��������һ����ת��Ŀ��Ƕȣ��������ڽǶ�ͻȻ�ı�ᵼ��һ���Ĳ��ȶ��ԣ������ڸı�ǶȺ��1s�ڲ�����ʹ��
 *
 */
