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
{//用户端
public:
	RSA rsa;
	int detectWhetherKey();//判断此用户是否存在key
	string getMac();//返回mac地址
	
	void userLogin();//处理用户进入程序时的必要措施
	int* getPrivateKey();//返回用户私钥
	int sendMessageBySignal(string passage,string timeStamp);//发送带有签名的信息
};

class serverManager
{//服务器端
public:
	RSA rsa;
	int* searchKeyByMac(string mac);//根据mac查找mac地址对应的key,若不存在返回字符串null
	int checkSignal(string passage, string timeStamp, long long signal, string mac);//判断是否为安全传输
};