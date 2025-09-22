/*
 * FFT_xcorr.h
 *
 *  Created on: 2023年5月23日
 *      Author: Administrator
 */





#ifndef SRC_APPSW_TRICORE_USER_FFT_XCORR_H_
#define SRC_APPSW_TRICORE_USER_FFT_XCORR_H_


#include <IfxCpu.h>
#include <LQ_ADC.h>
#include <LQ_CCU6.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>
#include <Main.h>
#include <Platform_Types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <Ifx_FftF32.h>
#include <Ifx_FftF32.h>
#include <LQ_GPIO_LED.h>
#include <math.h>
//2048

#define Sampling_Num 2048 //adc采集数据长度

/*
//512
#define Sampling_Num 512//
*/
extern volatile uint8  AdcFinishFlag;
extern short   rotatetime;
extern uint8   Istart;
extern short   soundtime_final;
extern short   soundtime;
extern volatile float g_Angle;
extern double wave_out;
extern double wave_out_last;
extern short sound_all;
extern uint8 amplitude_flag;//声幅标志
extern uint8 last_amplitude_flag;
extern short ampliting_critical;
//========================================================================================================
/*
extern  volatile int16_t MIC0_DATA[2][Sampling_Num];
extern  volatile int16_t MIC1_DATA[2][Sampling_Num];
extern  volatile int16_t MIC2_DATA[2][Sampling_Num];
extern  volatile int16_t MIC3_DATA[2][Sampling_Num];
*/
//adcflash

extern volatile int16_t MIC0_DATA[Sampling_Num];
extern volatile int16_t MIC1_DATA[Sampling_Num];
extern volatile int16_t MIC2_DATA[Sampling_Num];
extern volatile int16_t MIC3_DATA[Sampling_Num];

extern  volatile int16_t DATA0[3072];
extern  volatile int16_t DATA1[3072];
extern  volatile int16_t DATA2[3072];
extern  volatile int16_t DATA3[3072];


extern  volatile uint8 AdcBuffIndex;

void Sound_init(void);
void VoiceGetSample(void);
short MY_Xcorr(short* mic1,short* mic2,double* wave);
void sound_amplitude (void);
void get_stable_angle(void);
void VoiceProcess1(void);
//========================================================================================================


#endif /* SRC_APPSW_TRICORE_USER_FFT_XCORR_H*/
