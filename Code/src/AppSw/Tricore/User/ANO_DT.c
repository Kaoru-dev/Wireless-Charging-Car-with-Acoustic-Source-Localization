

#include <stdint.h>

#include "include.h"
//使用匿名4.3上位机协议
/////////////////////////////////////////////////////////////////////////////////////
//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )     /*!< uint32_t 数据拆分 byte0  */
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )     /*!< uint32_t 数据拆分 byte1  */
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )     /*!< uint32_t 数据拆分 byte2  */
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )     /*!< uint32_t 数据拆分 byte3  */

/**  发送数据缓存 */
unsigned char data_to_send[50];

/*!
  * @brief    Send_Data函数是协议中所有发送数据功能使用到的发送函数
  *
  * @param    dataToSend   :   要发送的数据首地址
  * @param    length       :   要发送的数据长度
  *
  * @return   无
  *
  * @note     移植时，用户应根据自身应用的情况，根据使用的通信方式，实现此函数
  *
  * @see      内部调用
  *
  * @date     2019/5/28 星期二
  */
void ANO_DT_Send_Data(unsigned char *dataToSend , unsigned short length)
{

    /**使用串口正常发送数据，大概需要1.5ms*/
	UART_PutBuff(UART0, dataToSend, length);     //可以修改不同的串口发送数据

}





/*!
  * @brief    向上位机发送发送8个int16_t数据
  *
  * @param    data1 - data8  ： 发送给上位机显示波形
  *
  * @return   无
  *
  * @note     无
  *
  * @see      ANO_DT_send_int16(1, 2, 3, 0, 0, 0, 0, 0);
  *
  * @date     2019/5/28 星期二
  */
void ANO_DT_send_int16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 )
{
    unsigned char  _cnt=0;
	unsigned char  sum = 0,ac, i = 0;
    data_to_send[_cnt++] = 0xAA;      //匿名协议帧头  0xAAAA

	data_to_send[_cnt++] = 0xFF;

	data_to_send[_cnt++] = 0xF1;      //使用用户协议帧0xF1

    data_to_send[_cnt++] = 16;        //8个int16_t 长度 16个字节
 	data_to_send[_cnt++]=BYTE0(data1);

	data_to_send[_cnt++]=BYTE1(data1);

	data_to_send[_cnt++]=BYTE0(data2);

	data_to_send[_cnt++]=BYTE1(data2);

	data_to_send[_cnt++]=BYTE0(data3);

	data_to_send[_cnt++]=BYTE1(data3);

    data_to_send[_cnt++]=BYTE0(data4);

    data_to_send[_cnt++]=BYTE1(data4);

    data_to_send[_cnt++]=BYTE0(data5);

    data_to_send[_cnt++]=BYTE1(data5);

    data_to_send[_cnt++]=BYTE0(data6);
    data_to_send[_cnt++]=BYTE1(data6);

    data_to_send[_cnt++]=BYTE0(data7);

    data_to_send[_cnt++]=BYTE1(data7);

    data_to_send[_cnt++]=BYTE0(data8);

    data_to_send[_cnt++]=BYTE1(data8);

    sum = 0;ac=0;
	for(i=0;i<_cnt;i++)
	{
		sum += data_to_send[i];
		ac+=sum;
	}
	data_to_send[_cnt++]=sum;
	data_to_send[_cnt++]=ac;
	ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_send_int16_sound(short data1, short data2, short data3, short data4 )
{
    unsigned char  _cnt=0;
    unsigned char  sum = 0,ac, i = 0;
    data_to_send[_cnt++] = 0xAA;      //匿名协议帧头  0xAAAA
    data_to_send[_cnt++] = 0xFF;
    data_to_send[_cnt++] = 0xF1;      //使用用户协议帧0xF1
    data_to_send[_cnt++] = 8;        //8个int16_t 长度 16个字节

    data_to_send[_cnt++]=BYTE0(data1);
    data_to_send[_cnt++]=BYTE1(data1);

    data_to_send[_cnt++]=BYTE0(data2);
    data_to_send[_cnt++]=BYTE1(data2);

    data_to_send[_cnt++]=BYTE0(data3);
    data_to_send[_cnt++]=BYTE1(data3);

    data_to_send[_cnt++]=BYTE0(data4);
    data_to_send[_cnt++]=BYTE1(data4);

    sum = 0;ac=0;
    for(i=0;i<_cnt;i++)
    {
        sum += data_to_send[i];
        ac+=sum;
    }
    data_to_send[_cnt++]=sum;
    data_to_send[_cnt++]=ac;
    ANO_DT_Send_Data(data_to_send, _cnt);
}
uint8 DataToSend[100];
uint8 DataGet[60];
sint32 userPar10=123;
/*
 *
 */
void ANODT_SendPar(uint16 _id,sint32 _val)
{
    unsigned char  _cnt=0;
       unsigned char  sum = 0,ac, i = 0;
       data_to_send[_cnt++] = 0xAA;      //匿名协议帧头  0xAAAA
       data_to_send[_cnt++] = 0xFF;
       data_to_send[_cnt++] = 0xE2;      //使用用户协议帧0xF1
       data_to_send[_cnt++] = 6;        //8个int16_t 长度 16个字节

       data_to_send[_cnt++]=BYTE0(_id);
       data_to_send[_cnt++]=BYTE1(_id);

       data_to_send[_cnt++]=BYTE0(_val);
       data_to_send[_cnt++]=BYTE1(_val);
       data_to_send[_cnt++]=BYTE2(_val);
       data_to_send[_cnt++]=BYTE3(_val);

       sum = 0;ac=0;
       for(i=0;i<_cnt;i++)
       {
           sum += data_to_send[i];
           ac+=sum;
       }
       data_to_send[_cnt++]=sum;
       data_to_send[_cnt++]=ac;
       ANO_DT_Send_Data(data_to_send, _cnt);

    }
/*校验帧
 *
 */
void ANODT_SendCheck(uint8 _id, uint8 _sc,uint8 ac)
{
    unsigned char  _cnt=0;
          unsigned char  sum = 0,asum, i = 0;
          data_to_send[_cnt++] = 0xAA;      //匿名协议帧头  0xAAAA
          data_to_send[_cnt++] = 0xFF;
          data_to_send[_cnt++] = 0x00;      //使用用户协议帧0xF1
          data_to_send[_cnt++] = 3;        //8个int16_t 长度 16个字节

          data_to_send[_cnt++]=_id;
          data_to_send[_cnt++]=_sc;
          data_to_send[_cnt++]=ac;


          sum = 0;asum=0;
          for(i=0;i<_cnt;i++)
          {
              sum += data_to_send[i];
              asum+=sum;
          }
          data_to_send[_cnt++]=sum;
          data_to_send[_cnt++]=asum;
          ANO_DT_Send_Data(data_to_send, _cnt);


    }


/*解析函数
 *功能：实现速度环，位置环的pid系数调节，注意：系数在发送和接收时均放大100倍，id：0，1，2，3，4对应speed P I D,和dir P D
 */

void ANODT_Anl(void)
{
    uint8 sc=0;//校验
    uint8 ac=0;
   // uint8 _datalen=DataGet[3];
    for(uint8 i=0;i<DataGet[3]+4;i++)
    {
        sc+=DataGet[i];
        ac+=sc;
    }
    if(sc!=DataGet[DataGet[3]+4]||ac!=DataGet[DataGet[3]+5])    //校验，即mcu计算和上位机发送的校验位是否相等
    {
        return;}
    if(DataGet[2]==0xE1)                                        //如果接收上位机发送的是读取命令
    {
       // LED_Ctrl(LEDALL,RVS);
        uint16 _id=DataGet[4]+(uint16)(DataGet[5]<<8);
        switch (_id)
        {
            case 0:
              ANODT_SendPar(_id,speedGiaP*100);break;
            case 1:
              ANODT_SendPar(_id,speedGiaI*100);break;
            case 2:
              ANODT_SendPar(_id,speedGiaD*100);break;
            case 3:
              ANODT_SendPar(_id,biasIndBigFastP*100);break;
            case 4:
              ANODT_SendPar(_id,biasIndBigFastD*100);break;
            case 5:
             ANODT_SendPar(_id,speedSet);break;
            case 6:
             ANODT_SendPar(_id,soundtoseek);break;
            case 7:
                         ANODT_SendPar(_id,currentDesired1);break;
            case 8:
                         ANODT_SendPar(_id,currentDesired2);break;
            case 9:
                         ANODT_SendPar(_id,currentKP1*100);break;
            case 10:
                         ANODT_SendPar(_id,currentKI1*100);break;
            case 11:
                         ANODT_SendPar(_id,currentKD1*100);break;

            case 12:
                ANODT_SendPar(_id,userPar10);break;
            default :
                ANODT_SendPar(_id,0);break;
        }
    }
    else if(DataGet[2]==0xE2)                                       //如果是写入命令
    {
        uint16 _id=DataGet[4]+(uint16)(DataGet[5]<<8);
        switch(_id)
        {
            case 0:speedGiaP=(float)*(sint32*)(&DataGet[6])/100;break;
            case 1:speedGiaI=(float)*(sint32*)(&DataGet[6])/100;break;
            case 2:speedGiaD=(float)*(sint32*)(&DataGet[6])/100;break;
            case 3:biasIndBigFastP=(float)*(sint32*)(&DataGet[6])/100;break;
            case 4:biasIndBigFastD=(float)*(sint32*)(&DataGet[6])/100;break;
            case 5:speedSet=(float)*(sint32*)(&DataGet[6]);break;
            case 6:soundtoseek=(float)*(sint32*)(&DataGet[6]);break;
            case 7:currentDesired1=(float)*(sint32*)(&DataGet[6]);break;
            case 8:currentDesired2=(float)*(sint32*)(&DataGet[6]);break;
            case 9:currentKP1=(float)*(sint32*)(&DataGet[6])/100;break;
            case 10:currentKI1=(float)*(sint32*)(&DataGet[6])/100;break;
            case 11:currentKD1=(float)*(sint32*)(&DataGet[6])/100;break;


            case 12:userPar10=*(sint32*)(&DataGet[6]);break;
        }
        ANODT_SendCheck(DataGet[2],DataGet[DataGet[3]+4],DataGet[DataGet[3]+5]);

    }
    }
/*上位机参数读取
 *
 */

void ANODT_Getbyte(uint8 data)
{
    static uint8 _sta=0;
    static uint8 _datalen=0;
    static uint8 _datacnt=0;
    if(_sta==0)
    {
        DataGet[0]=data;
        _datacnt=0;
        _datalen=0;
        if(data==0xAA)
        _sta=1;
    }
    else if(_sta==1)
    {
        DataGet[1]=data;
        _sta=2;
    }
    else if(_sta==2)
    {
        DataGet[2]=data;
        _sta=3;
    }
    else if(_sta==3)
    {
         if(data>50)
         _sta=0;
         else
         {
             _sta=4;
             DataGet[3]=data;
             _datalen=data;
         }
     }
    else if(_sta==4)//数据接收会一直进入这里直到数据接收完成，即_datacnt>=_datalen，例：对于写入命令，len=6；
       {
        DataGet[4+_datacnt]=data;
        _datacnt++;
        if(_datacnt>=_datalen)
            _sta=5;
       }
    else if(_sta==5)
       {
        DataGet[4+_datacnt++]=data;
            _sta=6;
       }
    else if(_sta==6)
         {
          DataGet[4+_datacnt++]=data;
          _sta=0;
          ANODT_Anl();
         }
}


