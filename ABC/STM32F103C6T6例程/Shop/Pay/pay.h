#ifndef __PAY_H
#define __PAY_H
#include "sys.h"
#include "rc522.h"
//测试
void test_read_write(void);

void ReadCard(void);//读卡
void userRecharge(u16 money);//充值
void userConsume(u16 moeny);//扣钱
void MoneyChangeToArr(void);//数值转换工具
void changeUserPwd(void);//修改密码
void wallet_Init(void);//初始化白卡,交钱默认设置金额10元

#endif


