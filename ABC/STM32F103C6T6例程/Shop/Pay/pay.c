#include "pay.h"
#include "usart.h"

unsigned char snr, buf[16],buf_name[16], TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char name[9]="DavidNan";
unsigned char money[9]="200";//初始金额200块
unsigned char password[6]="123456";
unsigned char password_16[6] = {0x12, 0x34, 0x56, 0x00, 0x00, 0x00};//123456的16进制形式
extern unsigned char SN[4];
extern u8 kid[9];
/* ---------------------------------------------------------------- */
/**
  * @Name    test_read_write
  * @param   : [输入/出]
  * @author  DavidNan
  * @Data    2023-03-29
  * <description> :简单测试一下读写功能
  *snr*4是指扇区号（Sector Number）乘以4，其结果表示该扇区的第一个块号。在MIFARE Classic 1K卡片中，每个扇区包含4个块，
  *因此每个扇区的第一个块号可以表示为snr*4。通过计算得到该块号后，就可以根据需要进一步计算出需要读写的块号。
  *snr*4+n,其中n的范围0-3;若snr=1时表示的是扇区1，n=3表示的是1扇区的第三块，这里放的是控制块以及密码。
 **/
/* ---------------------------------------------------------------- */

void test_read_write() {
    unsigned char status;
// 寻卡
    status= PcdRequest( REQ_ALL, TagType );     // REQ_ALL代表寻天线区内所有卡。TagType为返回的卡类型
    if(!status)
    {
        // 防冲突
        status = PcdAnticoll(SelectedSnr);
		ShowID(SelectedSnr);
		for(int i=0;i<9;i++){
			buf[i]=kid[i];//读到了什么。。
			buf_name[i]=name[i];
		}
	
		
        if(!status)
        {
            // 选卡
            status=PcdSelect(SelectedSnr);
            if(!status)
            {
                // 认证
                snr = 2;    // 扇区号1
				//验证A密钥，块地址，扇区密码，卡序列号 ###读写卡
                status = PcdAuthState(KEYA, (snr*4+3), DefaultKey, SelectedSnr);    // 校验1扇区密码，密码位于每一扇区第3块
//				status =PcdAuthState(KEYB, (snr*4+3), DefaultKey, SelectedSnr);
                // 验证A密钥，块地址，扇区密码，卡序列号
                {
                    if(!status)
                    {
                        //读写卡
                        //status = PcdRead((snr*4+0), buf);   // 读卡，读取1扇区0块数据到buf[0]-buf[16
						

                        status = PcdWrite((snr*4+0), buf);   // 写卡，将buf[0]-buf[16]写入1扇区0块
						status = PcdWrite((snr*4+1), buf_name);   // 写卡，将buf[0]-buf[16]写入1扇区0块
						status = PcdWrite((snr*4+2), money); //填入初始金额
						//status = PcdWrite((snr*4+3), password);
						
						status = PcdRead((snr*4+0), buf);   // 读卡，读取1扇区0块数据到buf[0]-buf[16
                        if(!status)
                        {
                            DEBUG_LOG("1.读到的ID值是： %s\n",buf);
                            //WaitCardOff();//等待卡离开
                        }
						status = PcdRead((snr*4+1), buf);   // 读卡，读取1扇区0块数据到buf[0]-buf[16
						  if(!status)
                        {
                            DEBUG_LOG("2.读到的用户名是： %s\n",buf);
                            //WaitCardOff();//等待卡离开
                        }
						status = PcdRead((snr*4+2), buf);   // 读卡，读取1扇区0块数据到buf[0]-buf[16
						  if(!status)
                        {
                            DEBUG_LOG("3.读到的余额是： %s\n",buf);
                            //WaitCardOff();//等待卡离开
                        }
							status = PcdRead((snr*4+3), buf);   // 读卡，读取1扇区0块数据到buf[0]-buf[16
						  if(!status)
                        {
                            DEBUG_LOG("4.读到的卡密是： %s\n",buf);
                            WaitCardOff();//等待卡离开
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
  * @param   : [输入/出] 
  * @author  DavidNan
  * @Data    2023-03-29
  * <description> :读出IC卡中的数据
  *   1扇区第3块的密码、卡序列号、金额存放在1扇区0块数据到buf[0]-buf[16]
 **/
/* ---------------------------------------------------------------- */

void ReadCard(){
	 unsigned char status;
	 status= PcdRequest( REQ_ALL, TagType );     // REQ_ALL代表寻天线区内所有卡。TagType为返回的卡类型
    if(!status)
    {
        // 防冲突
        status = PcdAnticoll(SelectedSnr);
		ShowID(SelectedSnr);
		for(int i=0;i<9;i++){
			buf[i]=kid[i];//将卡号明文写入buf给下一步存入1扇区0块
		}
		
        if(!status)
        {
            // 选卡
            status=PcdSelect(SelectedSnr);
            if(!status)
            {
                // 认证
                snr = 1;    // 扇区号1
                status = PcdAuthState(KEYA, (snr*4+3), DefaultKey, SelectedSnr);    // 校验1扇区密码，密码位于每一扇区第3块
                // 验证A密钥，块地址，扇区密码，卡序列号
                {
                    if(!status)
                    {
                        //读写卡
                       
                        status = PcdWrite((snr*4+0), buf);   // 写卡，将buf[0]-buf[16]写入1扇区0块 
						status = PcdRead((snr*4+0), buf);   // 读卡，读取1扇区0块数据到buf[0]-buf[16]
                        if(!status)
                        {
                            //读写成功
//                                printf("read finish!\n");
//                                printf("读到的值是： %s\n",buf);
                            DEBUG_LOG("读写成功!");
                            DEBUG_LOG("读到的值是： %s\n",buf);
                            WaitCardOff();
                        }
                    }
                }
            }
        }
    }
}
