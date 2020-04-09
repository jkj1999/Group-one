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
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();// PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);// �õ��ṹ���С,����GetAdaptersInfo����
	int netCardNum = 0;// ��¼�������� 
	int IPnumPerNetCard = 0; // ��¼ÿ�������ϵ�IP��ַ����

	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize]; // ���������ڴ�ռ������洢����������Ϣ
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
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
	//�ͷ��ڴ�ռ�
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
	
	return mac;
}

void userManager::userLogin()
{
	if(!detectWhetherKey())
	{//������key�ļ�
		ofstream outPerson("userKey.txt");
		int * keys = rsa.getKey();
		string mac = getMac();
		outPerson<<mac<<endl;//����mac
		outPerson<<keys[2]<<" "<<keys[3];//����˽Կ
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
	string md5Data = md5.get_md5_code(Message);//����->ժҪ

	long long code = rsa.getCode(md5Data);//ժҪ->ժҪ��ֵ
	int *key = getPrivateKey();//��ȡ˽Կ
	long long signal = rsa.encrypted(code,key[0],key[1]);//ժҪ��ֵ->ǩ��
	string mac = getMac();

	serverManager imageServer;//ģ�������
	return imageServer.checkSignal(passage,timeStamp,signal,mac);//ģ�ⷢ����������
}

int serverManager::checkSignal(string passage,string timeStamp,long long signal,string mac)
{
	int *key = searchKeyByMac(mac);//��ȡ��Կ
	if(!key)
		return 0;
	long long code = rsa.decrypt(signal,key[0],key[1]);//ǩ��->ժҪ��ֵ

	MD5_hash md5;
	string totalData = timeStamp+passage;
	char Message[10000] = { 0 };
    for(int m=0;m<totalData.length();m++)
    	Message[m] = totalData[m];
	string md5Data = md5.get_md5_code(Message);//����->ժҪ

	long long dataCode = rsa.getCode(md5Data);//ժҪ->ժҪ��ֵ

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