#ifndef __PAY_H
#define __PAY_H
#include "sys.h"
#include "rc522.h"
//����
void test_read_write(void);

void ReadCard(void);//����
void userRecharge(u16 money);//��ֵ
void userConsume(u16 moeny);//��Ǯ
void MoneyChangeToArr(void);//��ֵת������
void changeUserPwd(void);//�޸�����
void wallet_Init(void);//��ʼ���׿�,��ǮĬ�����ý��10Ԫ

#endif


