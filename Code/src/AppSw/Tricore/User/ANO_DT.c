

#include <stdint.h>

#include "include.h"
//ʹ������4.3��λ��Э��
/////////////////////////////////////////////////////////////////////////////////////
//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )     /*!< uint32_t ���ݲ�� byte0  */
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )     /*!< uint32_t ���ݲ�� byte1  */
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )     /*!< uint32_t ���ݲ�� byte2  */
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )     /*!< uint32_t ���ݲ�� byte3  */

/**  �������ݻ��� */
unsigned char data_to_send[50];

/*!
  * @brief    Send_Data������Э�������з������ݹ���ʹ�õ��ķ��ͺ���
  *
  * @param    dataToSend   :   Ҫ���͵������׵�ַ
  * @param    length       :   Ҫ���͵����ݳ���
  *
  * @return   ��
  *
  * @note     ��ֲʱ���û�Ӧ��������Ӧ�õ����������ʹ�õ�ͨ�ŷ�ʽ��ʵ�ִ˺���
  *
  * @see      �ڲ�����
  *
  * @date     2019/5/28 ���ڶ�
  */
void ANO_DT_Send_Data(unsigned char *dataToSend , unsigned short length)
{

    /**ʹ�ô��������������ݣ������Ҫ1.5ms*/
	UART_PutBuff(UART0, dataToSend, length);     //�����޸Ĳ�ͬ�Ĵ��ڷ�������

}





/*!
  * @brief    ����λ�����ͷ���8��int16_t����
  *
  * @param    data1 - data8  �� ���͸���λ����ʾ����
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      ANO_DT_send_int16(1, 2, 3, 0, 0, 0, 0, 0);
  *
  * @date     2019/5/28 ���ڶ�
  */
void ANO_DT_send_int16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 )
{
    unsigned char  _cnt=0;
	unsigned char  sum = 0,ac, i = 0;
    data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA

	data_to_send[_cnt++] = 0xFF;

	data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1

    data_to_send[_cnt++] = 16;        //8��int16_t ���� 16���ֽ�
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
    data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
    data_to_send[_cnt++] = 0xFF;
    data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1
    data_to_send[_cnt++] = 8;        //8��int16_t ���� 16���ֽ�

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
       data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
       data_to_send[_cnt++] = 0xFF;
       data_to_send[_cnt++] = 0xE2;      //ʹ���û�Э��֡0xF1
       data_to_send[_cnt++] = 6;        //8��int16_t ���� 16���ֽ�

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
/*У��֡
 *
 */
void ANODT_SendCheck(uint8 _id, uint8 _sc,uint8 ac)
{
    unsigned char  _cnt=0;
          unsigned char  sum = 0,asum, i = 0;
          data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
          data_to_send[_cnt++] = 0xFF;
          data_to_send[_cnt++] = 0x00;      //ʹ���û�Э��֡0xF1
          data_to_send[_cnt++] = 3;        //8��int16_t ���� 16���ֽ�

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


/*��������
 *���ܣ�ʵ���ٶȻ���λ�û���pidϵ�����ڣ�ע�⣺ϵ���ڷ��ͺͽ���ʱ���Ŵ�100����id��0��1��2��3��4��Ӧspeed P I D,��dir P D
 */

void ANODT_Anl(void)
{
    uint8 sc=0;//У��
    uint8 ac=0;
   // uint8 _datalen=DataGet[3];
    for(uint8 i=0;i<DataGet[3]+4;i++)
    {
        sc+=DataGet[i];
        ac+=sc;
    }
    if(sc!=DataGet[DataGet[3]+4]||ac!=DataGet[DataGet[3]+5])    //У�飬��mcu�������λ�����͵�У��λ�Ƿ����
    {
        return;}
    if(DataGet[2]==0xE1)                                        //���������λ�����͵��Ƕ�ȡ����
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
    else if(DataGet[2]==0xE2)                                       //�����д������
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
/*��λ��������ȡ
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
    else if(_sta==4)//���ݽ��ջ�һֱ��������ֱ�����ݽ�����ɣ���_datacnt>=_datalen����������д�����len=6��
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


