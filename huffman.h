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
	char binary[15];//存放编码 15位即可
	char data;//字符
	int weight;//字符出现的频次
	HFM_node* parent;
	HFM_node* left;
	HFM_node* right;
};

class HFM {
private:
	HFM_node huf[201];//根据数字、英文字母及标点符号数量，不会有超过100个字符节点
public:
	string compress(string passage);//压缩
	string uncompress(string hufpassage);//解压缩
	int return_start(int& a, int& b, int& c, string m);//将存在字符串m的前三个整数取出来，放在变量a，b，c中，并返回被压缩数据的首地址
};

//int return_start(int& a, int& b, int& c, string m);//将存在字符串m的前三个整数取出来，放在变量a，b，c中，并返回被压缩数据的首地址