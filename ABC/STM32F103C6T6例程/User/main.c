#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "rc522.h"
#include "led.h"

/**
*   连线说明：
*   1--SDA  <----->PA4
*   2--SCK  <----->PA5
*   3--MOSI <----->PA7
*   4--MISO <----->PA6
*   5--悬空
*   6--GND <----->GND
*   7--RST <----->PB0
*   8--VCC <----->VCC
**/

int main(void)
{
    u8 num = 0;
	u8 KEY_A[6]= {0xff,0xff,0xff,0xff,0xff,0xff};
	u8 data[16] = {0};
	u8 UID[4]= {0x49, 0xe0, 0x05, 0x98};
	 char status;
	unsigned char snr, buf[16], TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	//串口初始化为115200
    LED_Init();
    RC522_Init();       //初始化射频卡模块
	
	delay_ms(3000);
	// RC522_data_break(); // 测试用数据爆破程序，仅供学习参考，请勿非法使用

	// UID为你要修改的卡的UID key_type：0为KEYA，非0为KEYB KEY为密钥 RW:1是读，0是写 data_addr为修改的地址 data为数据内容
	IC_RW ( UID, 0, KEY_A, 1, 0x10, data );
	PcdReset();
	PcdAntennaOff();
	PcdAntennaOn();
	M500PcdConfigISOType( 'A' );
	

	
    while(1)
    {
		
        //RC522_Handle();
		
		// 寻卡
		status= PcdRequest( REQ_ALL , TagType );    // REQ_ALL代表寻天线区内所有卡。TagType为返回的卡类型
		if(!status)
		{
            // 防冲突
			status = PcdAnticoll(SelectedSnr);
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
							status = PcdRead((snr*4+0), buf);   // 读卡，读取1扇区0块数据到buf[0]-buf[16]
							buf[0]='a';
							buf[1]='b';
							buf[2]='c';
							status = PcdWrite((snr*4+0), buf);   // 写卡，将buf[0]-buf[16]写入1扇区0块
							if(!status) 
							{
                                //读写成功
                                printf("read finish!\n");
                                printf("读到的值是： %s\n",buf);
								WaitCardOff();
							}
						}
					}
				}
			}
		}

        if(num % 20 == 0)
            LED0 = !LED0;
        num++;
    }
}
