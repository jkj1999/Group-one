#include "RSA.h"

int RSA::getPrimeNumber()
{
	int distancePosition = rand()%100;
	int startPosition = rand()%100;
	int target = startPosition+distancePosition;
	ifstream in("src/primeNumber.txt");
	int prime;
	 for(int m=0;in>>prime;m++)
	 {
	 	if(m == target)
	 		break;
	 }
	return prime;
}
RSA::RSA()
{
	srand((int)time(0));
}
int RSA::getRelativePrime(int number)
{
	int e = rand()%number;
	int x,y;
	while(e<2 || extendGcd(e,x,number,y)!=1)
	{
		e = rand()%number;
	}
	return e;
}
int RSA::extendGcd(int a,int& x,int b,int& y)
{
	//ax+by=gcd(a,b),已知a,b求x,y,和最大公约数gcd
	int gcd;
	int q = a/b;
	int r0 = a,r1 = b;
	int s0 = 1,s1 = 0;
	int t0 = 0,t1 = 1;
	while(r1)
	{	
		int temp = s1;	
		s1 = s0-q*s1;
		s0 = temp;
		temp = t1;
		t1 = t0-q*t1;
		t0 = temp;
		temp = r1;
		r1 = r0%r1;
		r0 = temp;
		if(r1)
			q = r0/r1;
	}
	x = s0;
	y = -t0;
	gcd = r0;
	return gcd;
}
int* RSA::getKey()
{
	int *key = new int[4];
	int p = getPrimeNumber();
	int q;
	while((q=getPrimeNumber())==p);
	//cout<<"p:"<<p<<" q:"<<q<<endl;
	int n = p*q;
	int euler_n = (p-1)*(q-1);//与n互质的数字个数
	//cout<<"euler_n:"<<euler_n<<endl;
	int e = getRelativePrime(euler_n);
	//cout<<"e:"<<e<<endl;
	int d,k;
	extendGcd(e,d,euler_n,k);
	while(d<0)
		d += euler_n;
	key[0] = n;
	key[1] = e;
	key[2] = n;
	key[3] = d;
	return key;
}
long long RSA::encrypted(long long message,int public_x,int public_y)
{
	long long code = 1;
	for(int m=0;m<public_y;m++)
	{
		code *= message;
		code = code%public_x;
	}
	return code;
}
long long RSA::decrypt(long long message,int private_x,int private_y)
{
	long long code = 1;
	for(int m=0;m<private_y;m++)
	{
		code *= message;
		code = code%private_x;
	}
	return code;
}
long long RSA::getCode(string info)
{
	long long code = 0;
	for(int m=0;m<info.length();m++)
	{
		code += int(info[m]);
	}
	return code;
}
