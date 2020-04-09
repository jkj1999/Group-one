#include "userManager.h"

int userManager::detectWhetherKey()
{
	const char* keyPath = "userKey.txt";
	if (_access(keyPath,0) == -1)
	{
		return 0;
	}
	return 1;
}
string userManager::getMac()
{
	string mac;
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();// PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);// 得到结构体大小,用于GetAdaptersInfo参数
	int netCardNum = 0;// 记录网卡数量 
	int IPnumPerNetCard = 0; // 记录每张网卡上的IP地址数量

	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize]; // 重新申请内存空间用来存储所有网卡信息
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
	}
	if (ERROR_SUCCESS == nRel)
	{
		for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
		{
			char code[10];
			if (i < pIpAdapterInfo->AddressLength - 1)
			{
				sprintf_s(code, "%02X:", pIpAdapterInfo->Address[i]);
			}
			else
			{
				sprintf_s(code,"%02X", pIpAdapterInfo->Address[i]);
			}
			mac += code;
		}
		IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);

	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
	
	return mac;
}

void userManager::userLogin()
{
	if(!detectWhetherKey())
	{//不存在key文件
		ofstream outPerson("userKey.txt");
		int * keys = rsa.getKey();
		string mac = getMac();
		outPerson<<mac<<endl;//输入mac
		outPerson<<keys[2]<<" "<<keys[3];//输入私钥
		outPerson.close();

		ofstream outServer("serverKey.txt", ios::app);
		outServer<<mac<<endl;
		outServer<<keys[0]<<" "<<keys[1]<<endl;
		outServer.close();
	}

}
int* userManager::getPrivateKey()
{
	ifstream in("userKey.txt");
	string mac;
	getline(in,mac);
	int *key = new int[2];
	in>>key[0];
	in>>key[1];
	return key;
}

int userManager::sendMessageBySignal(string passage,string timeStamp)
{
	MD5_hash md5;
	string totalData = timeStamp+passage;
	char Message[10000] = { 0 };
    for(int m=0;m<totalData.length();m++)
    	Message[m] = totalData[m];
	string md5Data = md5.get_md5_code(Message);//文章->摘要

	long long code = rsa.getCode(md5Data);//摘要->摘要数值
	int *key = getPrivateKey();//获取私钥
	long long signal = rsa.encrypted(code,key[0],key[1]);//摘要数值->签名
	string mac = getMac();

	serverManager imageServer;//模拟服务器
	return imageServer.checkSignal(passage,timeStamp,signal,mac);//模拟发送至服务器
}

int serverManager::checkSignal(string passage,string timeStamp,long long signal,string mac)
{
	int *key = searchKeyByMac(mac);//获取公钥
	if(!key)
		return 0;
	long long code = rsa.decrypt(signal,key[0],key[1]);//签名->摘要数值

	MD5_hash md5;
	string totalData = timeStamp+passage;
	char Message[10000] = { 0 };
    for(int m=0;m<totalData.length();m++)
    	Message[m] = totalData[m];
	string md5Data = md5.get_md5_code(Message);//文章->摘要

	long long dataCode = rsa.getCode(md5Data);//摘要->摘要数值

	if(dataCode == code)
		return 1;
	else
		return 0;
}
int* serverManager::searchKeyByMac(string mac)
{
	int *key = new int[2];
	ifstream in("serverKey.txt");
	string line;
	while(getline(in,line))
	{
		in>>key[0];
		in>>key[1];
		if(line != mac)
			continue;
		else
			return key;
	}
	return NULL;
}