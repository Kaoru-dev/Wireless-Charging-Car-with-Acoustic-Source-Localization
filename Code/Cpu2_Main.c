/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC2/3xx���İ�
����    д��chiusir
��E-mail��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2021��1��18��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC2/3xx
��Crystal�� 20.000Mhz
��SYS PLL�� 200/300MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,
ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "..\Driver\include.h"//����ģ���ͷ�ļ�
#include <Main.h>
#include "include.h"
uint8  core2_ok = 0;
uint8  knvdsjkv = 0;
void core2_main(void)
{
    // ����CPU���ж�
    IfxCpu_enableInterrupts();

    // �رտ��Ź�
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
        core2_ok = 1;
       while(!core0_ok || !core1_ok) knvdsjkv++;
    // �ȴ�CPU0 ��ʼ�����
 //   while(!IfxCpu_acquireMutex(&mutexCpu0InitIsOk));
      // charge_init();
    while(1)
    {
            speedRead();
            delayms(20);
            soundtime=0;

           //Charge_Start();
    }
}
