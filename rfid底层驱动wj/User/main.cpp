#include "USART.h"
#include "TaskManager.h"
//#include "MFRC522.h"
#include "MFRC522.h"


/**
*@file 		�γ���ƶ�MFCR522��demo
*@author 	
*@version v0.1
*@brief 	
*/



//ԭ����A���룬16��������ÿ����������6Byte
unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
//unsigned char DefaultKey[6] = {0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc}; 

unsigned char sector[16]={0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60}; //�������׵�ַ

USART usart(1,115200);


//4�ֽڿ����кţ���5�ֽ�ΪУ���ֽ�

unsigned char i,tmp;
unsigned char status;
unsigned char status2;
unsigned char blockAddr;     //ѡ������Ŀ��ַ0��63
u8 sendDataBuffer[24];       //���巢�ͻ��壬ÿ�η���24�ֽ�

u8 readsuccess  = 0xca;      //��֤RFID����Ϣ�ɹ�
u8 readfail     = 0xcf;      //��֤RFID����Ϣʧ��

u8 writesuccess = 0xcb;      //дRFID����Ϣ�ɹ�

u8 checkcard    = 0xc0;      //�Ƿ��п���
u8 checkread    = 0xc1;      //�Ƿ�ɶ���


unsigned char Receive[20] = {};   //���յĻ���
unsigned char ReceiveStr[10][16]={};  //�˴����д10����
unsigned char EndStr[24]={0xaa,0xaa,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
                         ,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};  //����֡

u8 SendbackDataBuffer[24];							 

//��������MFRC522
												 
/*
 *checkSum ��У�飬����һ�����ݵĺ�												 
 *param *sendDataBuffer  ��Ҫ��͵�����
 *param size  ��Ҫ�������ݵ�����ĳ���
												 */
u8 checkSum(u8 *sendDataBuffer,char size) //��У��
{
	u32 sum=0;
	unsigned char i;
	for(i=0;i<size;i++)
	{
		sum += sendDataBuffer[i];
	}
	return sum%256;
}

/*
 *checkSum ��У�飬����У��ͨ��������true������У��ʧ�ܣ�����false												 
 *param *RecivieDataBuffer  ����֤��У��ĵ�����
 *param size  ��ҪУ���У�����ݵ�����ĳ���
												 */
bool checkSumIsOk(u8 *RecivieDataBuffer,char size) //20
{
	u32 sum=0;
	unsigned char i;
	for(i=0;i<size-1;i++)  // i��18
	{
		sum += RecivieDataBuffer[i];
	}
	u8 sum1 = sum%256;
	if(sum1 == RecivieDataBuffer[size-1])
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*
 * initSendDataBuffer ��ʼ����Ҫ���͵ķ������飬�����ɹ�������һ֡����֡ͷ�������룬���ݣ���У�飩
 * param fun_number �����룬˵��������������
*/
void initSendDataBuffer(unsigned char fun_number)  //��ʼ����������
{
	 unsigned char x;
	 unsigned char y;
	 u8 sum1;
	 sendDataBuffer[0] = 0xaa;
	 sendDataBuffer[1] = 0xaa;
	 sendDataBuffer[2] = fun_number;
	 for(x = 3;x<7;x++)
	 {
		  sendDataBuffer[x] = mfrc522.serNum[x-3];
	 }
	 for(y = 7;y<23;y++)
	 {
		  sendDataBuffer[y] = mfrc522.ReadStr[y-7];
	 }
	 sum1 = checkSum(sendDataBuffer,23);
	 sendDataBuffer[23] = sum1;
}

/*
 * 
*/
void initSendbackDataBuffer(unsigned char fun_number,unsigned char e)  //��ʼ���޸�ȷ�Ϸ�������
{
	 unsigned char x;
	 unsigned char y;
	 u8 sum1;
	 SendbackDataBuffer[0] = 0xaa;
	 SendbackDataBuffer[1] = 0xaa;
	 SendbackDataBuffer[2] = fun_number;
	 for(x = 3;x<7;x++)
	 {
		  SendbackDataBuffer[x] = mfrc522.serNum[x-3];
	 }
	 for(y = 7;y<23;y++)
	 {
		  SendbackDataBuffer[y] = ReceiveStr[e][y-7];
	 }
	 sum1 = checkSum(SendbackDataBuffer,23);
	 SendbackDataBuffer[23] = sum1;
}

/*
 * CheckBlock ������Ƿ���ã�
*/
void CheckBlock(unsigned char &block)
{
	if((block+1)%4!=0)  
	{
		block++;
		if((block+1)%4 == 0)
		{
			block++;
		}
	}
}

/*
 *MakeSendDataToCardBuffer ����д��RFID������
 * param ReceiveBuffer �Ӵ��ڽ��յ�������
*/
unsigned char MakeSendDataToCardBuffer(unsigned char *ReceiveBuffer)  //����д��RFID��������
{
	  unsigned char position=0;
	  if(ReceiveBuffer[0] == 0xaa && ReceiveBuffer[1] == 0xaa && ReceiveBuffer[2] ==0x01)  //д��ҩƷ
		{
			 for(position=0;position<16;position++)  //��RFID��д��16���ֽ�
			 {
				  ReceiveStr[0][position] = ReceiveBuffer[position+3];
			 }
			 return 4;          //����Ӧд���4��
		}
//		if(ReceiveBuffer[0] == 0xaa && ReceiveBuffer[1] == 0xaa && ReceiveBuffer[2] ==0x01)  //д��������
//		{
//			 for(position=0;position<16;position++)  //��RFID��д��16���ֽ�
//			 {
//				  ReceiveStr[0][position] = ReceiveBuffer[position+3];
//			 }
//			 return 4;          //����Ӧд���4��
//		}
		
		if(ReceiveBuffer[0] == 0xaa && ReceiveBuffer[1] == 0xaa && ReceiveBuffer[2] ==0x02)  //д�빤��
		{
			 for(position=0;position<16;position++)  //��RFID��д��16���ֽ�
			 {
				  ReceiveStr[1][position] = ReceiveBuffer[position+3];
			 }
			 return 5;           //����Ӧд���5��
		}
		if(ReceiveBuffer[0] == 0xaa && ReceiveBuffer[1] == 0xaa && ReceiveBuffer[2] ==0x03)  //д��������
		{
			 for(position=0;position<16;position++)  //��RFID��д��16���ֽ�
			 {
				  //ReceiveStr[2][position] = ReceiveBuffer[position+3];
				 ReceiveStr[2][position] = ReceiveBuffer[position+3];
			 }
			 return 6;               //����Ӧд���6��
		}
		
		return 63;
}


int main()
{
	tskmgr.DelayMs(2000);
	mfrc522.init();							//���ͳ�ʼ��������
	tskmgr.DelayMs(2);					//�ȴ�2ms֮�������
	mfrc522.antennaOn();				//������
	tskmgr.DelayMs(2);
	
	while(1)
	{
		usart.ClearReceiveBuffer();
		mfrc522.isCard();
		if(mfrc522.readCardSerial())
		{
			usart.SendData(&checkcard,1);     //���͸����п�������Ϣ
			usart.SendData(&checkread,1);     //���ͻ����ɶ���Ϣ
			usart.SendData(mfrc522.serNum,5); //�����������ID  �ģ����������Ϣһ���ͳ�ȥ
		}
		mfrc522.selectTag(mfrc522.serNum);
		blockAddr = sector[1];                //���ݿ�4  �˴�Ҳ�����˻������ݿ�,��һ�����������滵��
		                              //(i+1)%4==0  ����д   ��(3,7,11,15....)
		status = mfrc522.auth(PICC_AUTHENT1A, blockAddr, DefaultKey, mfrc522.serNum);//������֤��״̬
		if (status == MI_OK)
		{
			  if(mfrc522.read(blockAddr,mfrc522.ReadStr) == MI_OK)    //ҩ��+������Ϣ
				{
					initSendDataBuffer(0x01);
					usart.SendData(sendDataBuffer,24);
				}
				CheckBlock(blockAddr);
				if(mfrc522.read(blockAddr,mfrc522.ReadStr) == MI_OK)    //������Ϣ
				{
					initSendDataBuffer(0x02);
					usart.SendData(sendDataBuffer,24);
				}
				CheckBlock(blockAddr);
				if(mfrc522.read(blockAddr,mfrc522.ReadStr) == MI_OK)    //��������Ϣ
				{
					initSendDataBuffer(0x03);
					usart.SendData(sendDataBuffer,24);
					if(usart.ReceiveBufferSize()<20)         //������û������
				  {
					  usart.SendData(EndStr,24); //���ͽ���֡
				  }
				}
				
				if(usart.ReceiveBufferSize()>=20)  //����������Ĵ�С����20
				{
					// usart.ClearReceiveBuffer();
					usart.GetReceivedData(Receive,20);
					blockAddr = MakeSendDataToCardBuffer(Receive);
					if(blockAddr == 4)
					{
						if(mfrc522.write(blockAddr,ReceiveStr[0]) == MI_OK)   //д��ɹ�
						{
							initSendbackDataBuffer(0x01,0);
							usart.SendData(SendbackDataBuffer,24);   
							usart.SendData(EndStr,24);							
						}
					}
					if(blockAddr == 5)
					{
						if(mfrc522.write(blockAddr,ReceiveStr[1]) == MI_OK)   //д��ɹ�
						{
							 initSendbackDataBuffer(0x02,1);
							 usart.SendData(SendbackDataBuffer,24); 
							 usart.SendData(EndStr,24);							
						}
					}
					if(blockAddr == 6)
					{
						if(mfrc522.write(blockAddr,ReceiveStr[2]) == MI_OK)   //д��ɹ�
						{
							 initSendbackDataBuffer(0x03,2);
							 usart.SendData(SendbackDataBuffer,24);  
							 usart.SendData(EndStr,24);							
						}
				 }
		   }
				
		}
		else
			{
				usart.SendData(&readfail,1);  //������֤ʧ����Ϣ
			}
			
			mfrc522.halt();
	}
		
	return 0;
	
}



