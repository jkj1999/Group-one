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
	int getRelativePrime(int number);//获取与number互质的数
	int extendGcd(int a,int& x,int b,int& y);//扩展欧几里得算法
	int* getKey();//格式:[public_x][public_y][private_x][private_y]
	long long encrypted(long long message,int public_x,int public_y);//加密
	long long decrypt(long long message,int private_x,int private_y);//解密
	long long getCode(string info);
};