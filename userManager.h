#pragma once

#include <iostream>
#include <fstream>
#include <io.h>
#include "winsock.h"
#include "stdio.h"
#include <IPHlpApi.h>
#include "RSA.h"
#include "md5.h"
#pragma comment(lib,"Iphlpapi.lib")
using namespace std;
class userManager
{//�û���
public:
	RSA rsa;
	int detectWhetherKey();//�жϴ��û��Ƿ����key
	string getMac();//����mac��ַ
	
	void userLogin();//�����û��������ʱ�ı�Ҫ��ʩ
	int* getPrivateKey();//�����û�˽Կ
	int sendMessageBySignal(string passage,string timeStamp);//���ʹ���ǩ������Ϣ
};

class serverManager
{//��������
public:
	RSA rsa;
	int* searchKeyByMac(string mac);//����mac����mac��ַ��Ӧ��key,�������ڷ����ַ���null
	int checkSignal(string passage, string timeStamp, long long signal, string mac);//�ж��Ƿ�Ϊ��ȫ����
};