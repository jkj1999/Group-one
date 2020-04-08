#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<cstring>
#include<queue>
#include<stack>
#include <io.h> 
#include <string>

using namespace std;

struct HFM_node {
public:
	char binary[15];//��ű��� 15λ����
	char data;//�ַ�
	int weight;//�ַ����ֵ�Ƶ��
	HFM_node* parent;
	HFM_node* left;
	HFM_node* right;
};

class HFM {
private:
	HFM_node huf[201];//�������֡�Ӣ����ĸ�������������������г���100���ַ��ڵ�
public:
	string compress(string passage);//ѹ��
	string uncompress(string hufpassage);//��ѹ��
	int return_start(int& a, int& b, int& c, string m);//�������ַ���m��ǰ��������ȡ���������ڱ���a��b��c�У������ر�ѹ�����ݵ��׵�ַ
};

//int return_start(int& a, int& b, int& c, string m);//�������ַ���m��ǰ��������ȡ���������ڱ���a��b��c�У������ر�ѹ�����ݵ��׵�ַ