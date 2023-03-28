#include "pay.h"
#include "usart.h"

unsigned char snr, buf[16],buf_name[16], TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char name[9]="DavidNan";
unsigned char money[9]="200";//��ʼ���200��
unsigned char password[6]="123456";
unsigned char password_16[6] = {0x12, 0x34, 0x56, 0x00, 0x00, 0x00};//123456��16������ʽ
extern unsigned char SN[4];
extern u8 kid[9];
/* ---------------------------------------------------------------- */
/**
  * @Name    test_read_write
  * @param   : [����/��]
  * @author  DavidNan
  * @Data    2023-03-29
  * <description> :�򵥲���һ�¶�д����
  *snr*4��ָ�����ţ�Sector Number������4��������ʾ�������ĵ�һ����š���MIFARE Classic 1K��Ƭ�У�ÿ����������4���飬
  *���ÿ�������ĵ�һ����ſ��Ա�ʾΪsnr*4��ͨ������õ��ÿ�ź󣬾Ϳ��Ը�����Ҫ��һ���������Ҫ��д�Ŀ�š�
  *snr*4+n,����n�ķ�Χ0-3;��snr=1ʱ��ʾ��������1��n=3��ʾ����1�����ĵ����飬����ŵ��ǿ��ƿ��Լ����롣
 **/
/* ---------------------------------------------------------------- */

void test_read_write() {
    unsigned char status;
// Ѱ��
    status= PcdRequest( REQ_ALL, TagType );     // REQ_ALL����Ѱ�����������п���TagTypeΪ���صĿ�����
    if(!status)
    {
        // ����ͻ
        status = PcdAnticoll(SelectedSnr);
		ShowID(SelectedSnr);
		for(int i=0;i<9;i++){
			buf[i]=kid[i];//������ʲô����
			buf_name[i]=name[i];
		}
	
		
        if(!status)
        {
            // ѡ��
            status=PcdSelect(SelectedSnr);
            if(!status)
            {
                // ��֤
                snr = 2;    // ������1
				//��֤A��Կ�����ַ���������룬�����к� ###��д��
                status = PcdAuthState(KEYA, (snr*4+3), DefaultKey, SelectedSnr);    // У��1�������룬����λ��ÿһ������3��
//				status =PcdAuthState(KEYB, (snr*4+3), DefaultKey, SelectedSnr);
                // ��֤A��Կ�����ַ���������룬�����к�
                {
                    if(!status)
                    {
                        //��д��
                        //status = PcdRead((snr*4+0), buf);   // ��������ȡ1����0�����ݵ�buf[0]-buf[16
						

                        status = PcdWrite((snr*4+0), buf);   // д������buf[0]-buf[16]д��1����0��
						status = PcdWrite((snr*4+1), buf_name);   // д������buf[0]-buf[16]д��1����0��
						status = PcdWrite((snr*4+2), money); //�����ʼ���
						//status = PcdWrite((snr*4+3), password);
						
						status = PcdRead((snr*4+0), buf);   // ��������ȡ1����0�����ݵ�buf[0]-buf[16
                        if(!status)
                        {
                            DEBUG_LOG("1.������IDֵ�ǣ� %s\n",buf);
                            //WaitCardOff();//�ȴ����뿪
                        }
						status = PcdRead((snr*4+1), buf);   // ��������ȡ1����0�����ݵ�buf[0]-buf[16
						  if(!status)
                        {
                            DEBUG_LOG("2.�������û����ǣ� %s\n",buf);
                            //WaitCardOff();//�ȴ����뿪
                        }
						status = PcdRead((snr*4+2), buf);   // ��������ȡ1����0�����ݵ�buf[0]-buf[16
						  if(!status)
                        {
                            DEBUG_LOG("3.����������ǣ� %s\n",buf);
                            //WaitCardOff();//�ȴ����뿪
                        }
							status = PcdRead((snr*4+3), buf);   // ��������ȡ1����0�����ݵ�buf[0]-buf[16
						  if(!status)
                        {
                            DEBUG_LOG("4.�����Ŀ����ǣ� %s\n",buf);
                            WaitCardOff();//�ȴ����뿪
                        }
							
                    }
                }
            }
        }
    }
}

/* ---------------------------------------------------------------- */
/**
  * @Name    ReadCard
  * @param   : [����/��] 
  * @author  DavidNan
  * @Data    2023-03-29
  * <description> :����IC���е�����
  *   1������3������롢�����кš��������1����0�����ݵ�buf[0]-buf[16]
 **/
/* ---------------------------------------------------------------- */

void ReadCard(){
	 unsigned char status;
	 status= PcdRequest( REQ_ALL, TagType );     // REQ_ALL����Ѱ�����������п���TagTypeΪ���صĿ�����
    if(!status)
    {
        // ����ͻ
        status = PcdAnticoll(SelectedSnr);
		ShowID(SelectedSnr);
		for(int i=0;i<9;i++){
			buf[i]=kid[i];//����������д��buf����һ������1����0��
		}
		
        if(!status)
        {
            // ѡ��
            status=PcdSelect(SelectedSnr);
            if(!status)
            {
                // ��֤
                snr = 1;    // ������1
                status = PcdAuthState(KEYA, (snr*4+3), DefaultKey, SelectedSnr);    // У��1�������룬����λ��ÿһ������3��
                // ��֤A��Կ�����ַ���������룬�����к�
                {
                    if(!status)
                    {
                        //��д��
                       
                        status = PcdWrite((snr*4+0), buf);   // д������buf[0]-buf[16]д��1����0�� 
						status = PcdRead((snr*4+0), buf);   // ��������ȡ1����0�����ݵ�buf[0]-buf[16]
                        if(!status)
                        {
                            //��д�ɹ�
//                                printf("read finish!\n");
//                                printf("������ֵ�ǣ� %s\n",buf);
                            DEBUG_LOG("��д�ɹ�!");
                            DEBUG_LOG("������ֵ�ǣ� %s\n",buf);
                            WaitCardOff();
                        }
                    }
                }
            }
        }
    }
}
