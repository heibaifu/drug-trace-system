/**
*@file MFRC522.cpp
*@author 
*@version v0.1
*@brief ��������MFRC522��ʹ��ʱ���� string.h ,������Ҫʹ��USART2�����������ֱ�Ӱ�USART2�ӹ��ˣ�Ϊ�˿γ����Ҳû�б�Ҫȥ���ġ�
*
*/


#include "MFRC522.h"
#include "string.h"
USART usart2test(2,9600);//Ĭ��9600
//USART usart1test(1,115200);//���������Ƿ���

MFRC522 mfrc522;


/******************************************************************************
 *@brief�����캯��
 *@param����
 *@retval����
 ******************************************************************************/

MFRC522::MFRC522()
{
	;
}




/******************************************************************************
 *@brief����ʼ��RC522
 *@param����
 *@retval����
 ******************************************************************************/
void MFRC522::init()
{
  reset();

  //Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
	//Tauto=1; f(Timer) = 6.78MHz/TPreScaler
	//TModeReg[3..0] + TPrescalerReg
  writeMFRC522(TModeReg, 0x8D);  
  writeMFRC522(TPrescalerReg, 0x3E); 
  writeMFRC522(TReloadRegL, 30);
  writeMFRC522(TReloadRegH, 0);
  writeMFRC522(TxAutoReg, 0x40);    //100%ASK
  writeMFRC522(ModeReg, 0x3D);    // CRC valor inicial de 0x6363

  //ClearBitMask(Status2Reg, 0x08); //MFCrypto1On=0
  //writeMFRC522(RxSelReg, 0x86);   //RxWait = RxSelReg[5..0]
  //writeMFRC522(RFCfgReg, 0x7F);     //RxGain = 48dB

//  antennaOn();    //������
}

/******************************************************************************
 *@brief��Ѱ��
 *@param����
 *@retval���п�������true���޿�������false
 ******************************************************************************/
bool MFRC522::isCard()
{
  unsigned char status;											//���MFRC���ص�״̬
  unsigned char str[MAX_LEN];
//	status = MFRC522Request(PICC_REQALL, str);//��ȡ���еĿ�
  status = MFRC522Request(PICC_REQIDL, str);
  if (status == MI_OK)
    return true;
  else
    return false;
}

/******************************************************************************
 *@brief��Ѱ������ȡ�����ͺ�
 *@param��reqMode--Ѱ����ʽ��
 *        TagType--���ؿ�Ƭ����
 *                    0x4400 = Mifare_UltraLight
 *                    0x0400 = Mifare_One(S50)
 *                    0x0200 = Mifare_One(S70)
 *                    0x0800 = Mifare_Pro(X)
 *                    0x4403 = Mifare_DESFire
 *@retval���ɹ�����MI_OK
 ******************************************************************************/

unsigned char MFRC522::MFRC522Request(unsigned char reqMode, unsigned char *TagType)
{
  unsigned char status;
  unsigned int backBits;      //���յ�������λ��

  writeMFRC522(BitFramingReg, 0x07);    //TxLastBists = BitFramingReg[2..0] ???��ʾ�������һ���ֽڵ����λ

  TagType[0] = reqMode;
  status = MFRC522ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);
//	printTest(TagType,2);//�õ������Ű׿�ȫ��0400

  if ((status != MI_OK) || (backBits != 0x10))
    status = MI_ERR;

  return status;
}


/******************************************************************************
 *@brief����MFRC522��ĳһ�Ĵ���дһ���ֽ�����
 *					����λ��Ϊ���ƶ�д��1Ϊ����0Ϊд
 *					����λ����
 *					0-5λΪ�Ĵ�����ֵ
 *@param��addr--�Ĵ�����ַ��val--Ҫд���ֵ
 *@retval����
 ******************************************************************************/
void MFRC522::writeMFRC522(unsigned char addr, unsigned char val)
{
unsigned char writeaddval[2] = {addr,val};
unsigned char reciveval = 0x01;
  
	usart2test.SendData(writeaddval,1);//���͵�ַ�����ݹ�ȥ
	for(uint32_t i=0;i<100000;++i);
	if(usart2test.ReceiveBufferSize()>0)
	{
		usart2test.GetReceivedData(&reciveval,1);

	}
	if(reciveval == addr)//������ػ�����ֵ�ǵ�ַ���ͱ������ʳɹ�
	{
		usart2test.SendData(&val,1);//���͵�ַ�����ݹ�ȥ
	}
}
/******************************************************************************
 *@brief����MFRC522��ĳһ�Ĵ�����һ���ֽ�����
 *					����λ��Ϊ���ƶ�д��1Ϊ����0Ϊд
 *					����λ����
 *					0-5λΪ�Ĵ�����ֵ
 *@param��addr--�Ĵ�����ַ
 *@retval�����ض�ȡ����һ���ֽ�����
 ******************************************************************************/
unsigned char MFRC522::readMFRC522(u8 addr)
{
	unsigned char reciveval[1] = {0x01};
  u8 reeaaddre = (addr|0x80);

  usart2test.SendData(&reeaaddre,1);
	for(uint32_t i=0;i<100000;++i);
	if(usart2test.ReceiveBufferSize()>0)
	{
//		int receivedDataNum=usart2test.ReceiveBufferSize();
		usart2test.GetReceivedData(reciveval,1);
	}
	//////////////////////////////////////
	///���Ե�ʱ����usart1��ӡ����
	///////////////////////////////////
//	usart1test.SendData(reciveval,1);
  return reciveval[0];
}

/******************************************************************************
 *@brief��ͨ��RC522��ISO14443��ͨѶ
 *@param��Command[IN]:RC522������
 *          pInData[IN]:ͨ��RC522���͵���Ƭ������
 *          InLenByte[IN]:�������ݵ��ֽڳ���
 *          pOutData[OUT]:���յ��Ŀ�Ƭ��������
 *          *pOutLenBit[OUT]:�������ݵ�λ����
 *@retval���ɹ�����MI_OK
 ******************************************************************************/
unsigned char MFRC522::MFRC522ToCard(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
		unsigned int MAXRLEN = 18;
    switch (Command)
    {
       case PCD_AUTHENT:
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
       case PCD_TRANSCEIVE:
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
       default:
         break;
    }
   
    writeMFRC522(CommIEnReg,irqEn|0x80);
    clearBitMask(CommIrqReg,0x80);
    writeMFRC522(CommandReg,PCD_IDLE);
    setBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   writeMFRC522(FIFODataReg, pInData[i]);    }
    writeMFRC522(CommandReg, Command);
   
    
    if (Command == PCD_TRANSCEIVE)
    {    setBitMask(BitFramingReg,0x80);  }
    
    i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
    do 
    {
         n = readMFRC522(CommIrqReg);
         i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    clearBitMask(BitFramingReg,0x80);
	      
    if (i!=0)
    {    
         if(!(readMFRC522(ErrorReg)&0x1B))
         {
             status = MI_OK;
             if (n & irqEn & 0x01)
             {   status = MI_NOTAGERR;   }
             if (Command == PCD_TRANSCEIVE)
             {
               	n = readMFRC522(FIFOLevelReg);
              	lastBits = readMFRC522(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = readMFRC522(FIFODataReg);    }
            }
         }
         else
         {   status = MI_ERR;   }
        
   }
   

   setBitMask(ControlReg,0x80);           // stop timer now
   writeMFRC522(CommandReg,PCD_IDLE); 
   return status;
}

///******************************************************************************
// * �� �� ����MFRC522ToCard
// * ����������RC522��ISO14443��ͨѶ
// * ���������command--MF522�����֣�
// *           sendData--ͨ��RC522���͵���Ƭ������,
// *                     sendLen--���͵����ݳ���
// *                     backData--���յ��Ŀ�Ƭ�������ݣ�
// *                     backLen--�������ݵ�λ����
// * �� �� ֵ���ɹ�����MI_OK
// ******************************************************************************/
//unsigned char MFRC522::MFRC522ToCard(unsigned char command, 
//																			unsigned char *sendData, 
//																				unsigned char sendLen, 
//																					unsigned char *backData, 
//																						unsigned int *backLen)
//{
//  unsigned char status = MI_ERR;
//  unsigned char irqEn = 0x00;
//  unsigned char waitIRq = 0x00;
//  unsigned char lastBits;
//  unsigned char n;
//  unsigned int i;
//	
//			u8 str = 0xaa;//��������

//  switch (command)
//  {
//    case PCD_AUTHENT:   //��֤����
//    {
//      irqEn = 0x12;
//      waitIRq = 0x10;
//      break;
//    }
//    case PCD_TRANSCEIVE:  //����FIFO������
//    {
//      irqEn = 0x77;
//      waitIRq = 0x30;
//      break;
//    }
//    default:
//      break;
//  }

//  writeMFRC522(CommIEnReg, irqEn|0x80); //�����ж�����
//  clearBitMask(CommIrqReg, 0x80);       //��������ж�����λ
//  setBitMask(FIFOLevelReg, 0x80);       //FlushBuffer=1, FIFO��ʼ��

//  writeMFRC522(CommandReg, PCD_IDLE);   //�޶�����ȡ����ǰ����

//  //��FIFO��д������
//  for (i=0; i<sendLen; i++)
//    writeMFRC522(FIFODataReg, sendData[i]);

//  //ִ������
//  writeMFRC522(CommandReg, command);
//  if (command == PCD_TRANSCEIVE)
//    setBitMask(BitFramingReg, 0x80);    //StartSend=1,transmission of data starts

//  //�ȴ������������
//  i = 2000; //i����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
//  do
//  {
//    //CommIrqReg[7..0]
//    //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
//    n = readMFRC522(CommIrqReg);
//    i--;
//  }
//  while ((i!=0) && !(n&0x01) && !(n&waitIRq));

//  clearBitMask(BitFramingReg, 0x80);      //StartSend=0

//  if (i != 0)
//  {

//    if(!(readMFRC522(ErrorReg) & 0x1B)) //BufferOvfl Collerr CRCErr ProtecolErr
//    {
//      status = MI_OK;
//      if (n & irqEn & 0x01)
//        status = MI_NOTAGERR;     //�ж��Ƿ��п�

//      if (command == PCD_TRANSCEIVE)
//      {
//        n = readMFRC522(FIFOLevelReg);
//        lastBits = readMFRC522(ControlReg) & 0x07;
//        if (lastBits)
//          *backLen = (n-1)*8 + lastBits;
//        else
//          *backLen = n*8;

//        if (n == 0)
//          n = 1;
//        if (n > MAX_LEN)
//          n = MAX_LEN;

//        //��ȡFIFO�н��յ�������
//        for (i=0; i<n; i++)
//          backData[i] = readMFRC522(FIFODataReg);
//      }
//    }
//    else
//		{
//			if(!(readMFRC522(CollReg)&0x20))//1&1 = 1 0&1 = 0,0�����г�ͻ��1����û��ͻ
//			{

//				u8 where = readMFRC522(CollReg)&0x1f;//�洢��һ�������˳�ͻ�ĵط�
////				printTest(&where,1); 
//			//������ײ���������ײ�㷨
//			 if (command == PCD_TRANSCEIVE)
//      {
//        n = readMFRC522(FIFOLevelReg);
//        lastBits = readMFRC522(ControlReg) & 0x07;//�洢�����յ����Լ�����Чλ��Ŀ
//        if (lastBits)
//          *backLen = (n-1)*8 + lastBits;
//        else
//          *backLen = n*8;

//        if (n == 0)
//          n = 1;
//        if (n > MAX_LEN)
//          n = MAX_LEN;

//        //��ȡFIFO�н��յ�������
//        for (i=0; i<n; i++)
//          backData[i] = readMFRC522(FIFODataReg);
//      }
////			printTest(&str,1);
////			printTest(backData,n);
//      status = MI_ERR;
//		}
//	}
//  }

//  //SetBitMask(ControlReg,0x80);           //timer stops
//  //Write_MFRC522(CommandReg, PCD_IDLE);

//  return status;
//}
/******************************************************************************
 *@brief����RC522�Ĵ���λ
 *@param��reg--�Ĵ�����ַ;mask--��λֵ
 *@retval����
 ******************************************************************************/
void MFRC522::clearBitMask(unsigned char reg, unsigned char mask)
{
  unsigned char tmp;
  tmp = readMFRC522(reg);
  writeMFRC522(reg, tmp & (~mask));  // clear bit mask
}

/******************************************************************************
 *@brief����λRC522�Ĵ���
 *@param��reg--�Ĵ�����ַ;mask--��λֵ
 *@retval����
 ******************************************************************************/
void MFRC522::setBitMask(unsigned char reg, unsigned char mask)
{
  unsigned char tmp;
  tmp = readMFRC522(reg);
  writeMFRC522(reg, tmp | mask);  // set bit mask
}


/******************************************************************************
 *@brief�������λRC522
 *@param����
*@retval����
 ******************************************************************************/
void MFRC522::reset()
{
  writeMFRC522(CommandReg, PCD_RESETPHASE);
}
/******************************************************************************
 *@brief����������,ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
 *@param����
 *@retval����
 ******************************************************************************/
void MFRC522::antennaOn(void)
{
  unsigned char temp;

  temp = readMFRC522(TxControlReg);
  if (!(temp & 0x03))
  {
    setBitMask(TxControlReg, 0x03);
  }
}
/******************************************************************************
 *@brief���ر�����,ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
 *@param����
 *@retval����
 ******************************************************************************/
void MFRC522::antennaOff(void)
{
  unsigned char temp;

  temp = readMFRC522(TxControlReg);
  if (!(temp & 0x03))
  {
    clearBitMask(TxControlReg, 0x03);
  }
}
/******************************************************************************
*@brief�����ؿ������к� 4�ֽ�
*@param����
*@retval���ɹ�����ture ʧ�ܷ���false
 ******************************************************************************/
bool MFRC522::readCardSerial(){

  unsigned char status;
  unsigned char str[MAX_LEN];
  
  // ����ײ�����ؿ������к� 4�ֽڣ�����serNum��
  status = anticoll(str);
  memcpy(serNum, str, 5);
  
  if (status == MI_OK)
    return true;
  else
    return false;
}
/******************************************************************************
 *@brief������ͻ��⣬��ȡѡ�п�Ƭ�Ŀ����к�
 *@param��serNum--����4�ֽڿ����к�,��5�ֽ�ΪУ���ֽ�
 *@retval���ɹ�����MI_OK
 ******************************************************************************/
unsigned char MFRC522::anticoll(unsigned char *serNum)
{
  unsigned char status;
  unsigned char i;
  unsigned char serNumCheck=0;
  unsigned int unLen;

//  clearBitMask(Status2Reg, 0x08);   //TempSensclear
//  clearBitMask(CollReg,0x80);     //ValuesAfterColl
  writeMFRC522(BitFramingReg, 0x00);    //TxLastBists = BitFramingReg[2..0]

  serNum[0] = PICC_ANTICOLL;
  serNum[1] = 0x20;
  status = MFRC522ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

  if (status == MI_OK)
  {
    //У�鿨���к�
    for (i=0; i<4; i++)
      serNumCheck ^= serNum[i];
    if (serNumCheck != serNum[i])
      status = MI_ERR;
  }

  //SetBitMask(CollReg, 0x80);    //ValuesAfterColl=1

  return status;
}



/******************************************************************************
 *@brief�����Ƭ��������״̬
 *@param����
 *@retval����
 ******************************************************************************/
void MFRC522::halt()
{
  unsigned char status;
  unsigned int unLen;
  unsigned char buff[4];

  buff[0] = PICC_HALT;
  buff[1] = 0;
  calculateCRC(buff, 2, &buff[2]);

  status = MFRC522ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);//���������ݵ������MFRC�������ߵ��������
}


/******************************************************************************
 *@brief��ѡ����Halt
 *@brief�����Ƭ��������״̬
 *@param����
 *@retval����
 ******************************************************************************/
void MFRC522::haltselect(unsigned char* str)
{
  unsigned int unLen;
	unsigned char status;
  unsigned char buff[6];

  buff[0] = PICC_HALT;
  buff[1] = *str;
	buff[2] = *str+1;
	buff[3] = *str+2;
	buff[4] = *str+3;
	buff[5] = *str+4;
//  calculateCRC(buff, 2, &buff[2]);

  status = MFRC522ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);//���������ݵ������MFRC�������ߵ��������
}


/******************************************************************************
 *@brief����MF522����CRC
 *@param��pIndata--Ҫ����CRC�����ݣ�len--���ݳ��ȣ�pOutData--�����CRC���
 *@retval����
 ******************************************************************************/
void MFRC522::calculateCRC(unsigned char *pIndata, unsigned char len, unsigned char *pOutData)
{
  unsigned char i, n;

  clearBitMask(DivIrqReg, 0x04);      //CRCIrq = 0
  setBitMask(FIFOLevelReg, 0x80);     //��FIFOָ��
  //Write_MFRC522(CommandReg, PCD_IDLE);

  //��FIFO��д������
  for (i=0; i<len; i++)
    writeMFRC522(FIFODataReg, *(pIndata+i));
  writeMFRC522(CommandReg, PCD_CALCCRC);

  //�ȴ�CRC�������
  i = 0xFF;
  do
  {
    n = readMFRC522(DivIrqReg);
    i--;
  }
  while ((i!=0) && !(n&0x04));      //CRCIrq = 1

  //��ȡCRC������
  pOutData[0] = readMFRC522(CRCResultRegL);
  pOutData[1] = readMFRC522(CRCResultRegM);
}
/******************************************************************************
 *@brief��ѡ������ȡ���洢������
 *@param��serNum--���뿨���к�
 *@retval���ɹ����ؿ�����
 ******************************************************************************/
unsigned char MFRC522::selectTag(unsigned char *serNum)
{
  unsigned char i;
  unsigned char status;
  unsigned char size;
  unsigned int recvBits;
  unsigned char buffer[9];

  //ClearBitMask(Status2Reg, 0x08);                        //MFCrypto1On=0

  buffer[0] = PICC_SElECTTAG;
  buffer[1] = 0x70;

  for (i=0; i<5; i++)
    buffer[i+2] = *(serNum+i);

  calculateCRC(buffer, 7, &buffer[7]);
  
  status = MFRC522ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
  if ((status == MI_OK) && (recvBits == 0x18))
    size = buffer[0];
  else
    size = 0;
  return size;
}

/******************************************************************************
 *@brief����֤��Ƭ����
 *@param��authMode--������֤ģʽ
 *                     0x60 = ��֤A��Կ
 *                     0x61 = ��֤B��Կ
 *           BlockAddr--���ַ
 *           Sectorkey--��������
 *           serNum--��Ƭ���кţ�4�ֽ�
 *@retval���ɹ�����MI_OK
 ******************************************************************************/
unsigned char MFRC522::auth(unsigned char authMode, unsigned char BlockAddr, unsigned char *Sectorkey, unsigned char *serNum)
{
  unsigned char status;
  unsigned int recvBits;
  unsigned char i;
  unsigned char buff[12];

  //��ָ֤��+���ַ���������룫�����к�
  buff[0] = authMode;
  buff[1] = BlockAddr;
  for (i=0; i<6; i++)
    buff[i+2] = *(Sectorkey+i);
  for (i=0; i<4; i++)
    buff[i+8] = *(serNum+i);
    
  status = MFRC522ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);//��ʵ����֤�ɹ��˵�				
//  if ((status != MI_OK) || (!(readMFRC522(Status2Reg) & 0x08)))
//    status = MI_ERR;
  if ((status != MI_OK))
    status = MI_ERR;
  return status;
}
/******************************************************************************
 *@brief����������
 *@param��blockAddr--���ַ;recvData--�����Ŀ�����
 *@retval���ɹ�����MI_OK
 ******************************************************************************/
unsigned char MFRC522::read(unsigned char blockAddr, unsigned char *recvData)
{
  unsigned char status;
  unsigned int unLen;

  recvData[0] = PICC_READ;
  recvData[1] = blockAddr;
  calculateCRC(recvData,2, &recvData[2]);
  status = MFRC522ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);

  if ((status != MI_OK) || (unLen != 0x90))
    status = MI_ERR;

  return status;
}
/******************************************************************************
 *@brief��д������
 *@param��blockAddr--���ַ;writeData--���д16�ֽ�����
 *@retval���ɹ�����MI_OK
 ******************************************************************************/
unsigned char MFRC522::write(unsigned char blockAddr, unsigned char *writeData)
{
  unsigned char status;
  unsigned int recvBits;
  unsigned char i;
  unsigned char buff[18];

  buff[0] = PICC_WRITE;
  buff[1] = blockAddr;
  calculateCRC(buff, 2, &buff[2]);
  status = MFRC522ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);

  if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
    status = MI_ERR;

  if (status == MI_OK)
  {
    for (i=0; i<16; i++)    //?FIFO?16Byte?? Datos a la FIFO 16Byte escribir
      buff[i] = *(writeData+i);
      
    calculateCRC(buff, 16, &buff[16]);
    status = MFRC522ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);

    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
      status = MI_ERR;
  }

  return status;
}
















