#pragma once

#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;
class RSA
{
public:
	int getPrimeNumber();
	RSA();
	int getRelativePrime(int number);//��ȡ��number���ʵ���
	int extendGcd(int a,int& x,int b,int& y);//��չŷ������㷨
	int* getKey();//��ʽ:[public_x][public_y][private_x][private_y]
	long long encrypted(long long message,int public_x,int public_y);//����
	long long decrypt(long long message,int private_x,int private_y);//����
	long long getCode(string info);
};