#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include <iostream>
using namespace std;
//常量
const unsigned int s[4][4] = { {7,12,17,22},{5,9,14,20},{4,11,16,23},{6,10,15,21} };
//t[i]是4294967296*abs(sin(i))的整数部分,i的单位是弧度。
const unsigned long int t[64] =
{
 0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
 0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
 0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
 0x6b901122,0xfd987193,0xa679438e,0x49b40821,
 0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
 0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
 0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
 0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
 0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
 0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
 0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
 0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
 0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
 0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
 0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
 0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
};
//x[i]中来标识i的标签
const int label[64] = {
 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
 1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12,
 5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2,
 0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9
};
//a,b,c,d四个用于计算的四字节数，M代表子分组，t表示弧度，s表示位移量，choice表示选择什么运算
void Calculation(unsigned long int* a, unsigned long int b, unsigned long int c, unsigned long int d, unsigned long int M,  unsigned long int t, int s, int choice)
{
    unsigned long int temp;
    //四轮运算
    switch (choice) 
    {
    case 0:
        temp = (b & c) | ((~b) & d);
        break;
    case 1:
        temp = (b & d) | (c & (~d));
        break;
    case 2:
        temp = b ^ c ^ d;
        break;
    case 3:
        temp = c ^ (b | (~d));
        break;
    }
    *a += temp + M + t;
    *a = (*a << s) | (*a >> (32 - s));
    *a += b;
}
//参数为：hash指的是一开始初始化的A、B、C、D，之后不断加上此函数处理完的a、b、c、d；M为16个四字节子分组
void MD5Calculation(const unsigned long int M[16], unsigned long int hash[4]) 
{
    int i, j, sequence = 0;
    //共进行FF、GG、HH、II四轮
    //每轮按照abcd、dabc、cdab、bcda的次序重复进行四次，分别对应s11，s12，s13，s14...，弧度和x是依次往下
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) 
        {
            Calculation(&hash[0], hash[1], hash[2], hash[3], M[label[sequence]], t[sequence], s[i][0], i);
            sequence++;
            Calculation(&hash[3], hash[0], hash[1], hash[2], M[label[sequence]], t[sequence], s[i][1], i);
            sequence++;
            Calculation(&hash[2], hash[3], hash[0], hash[1], M[label[sequence]], t[sequence], s[i][2], i);
            sequence++;
            Calculation(&hash[1], hash[2], hash[3], hash[0], M[label[sequence]], t[sequence], s[i][3], i);
            sequence++;
        }
}
//输入字符串及其长度，输出output
void MD5(char* M, int inputlen, unsigned long int output[4]) 
{
    int i, j;
    unsigned long int hashout[4] = { 0x67452301,0xefcdab89,0x98badcfe,0x10325476 };//初始化A、B、C、D，这也是最终要输出的
    unsigned long int hashin[4];//输入到函数中处理的代替hashout的
    //填充
    long long previousbitslen = inputlen * 8;//字节数转换为bit数
    int previouslen = inputlen;
    //求余结果不等于448就填充一个1和多个0，字节长度也随之增加
    while (inputlen % 64 != 56) 
    {
        M[inputlen] = 0;
        inputlen++;
    }
    M[previouslen] = 0x80;
    *(long long*)(M + inputlen) = previousbitslen;//最后还剩下64位16个字节存放原来的位数，这是算法要求，longlong表示的是64位
    inputlen += 8;//扩展后的字节数要加上最后那个longlong,64的倍数
    //开始处理分组
    for (i = 0; i < inputlen; i += 64) 
    {
        memcpy(hashin, hashout, sizeof(long int) * 4);//4个4字节复制到hash里面
        MD5Calculation((const unsigned long int*)&M[i], hashin);//处理512bits分组，M的转换极其重要，之前是char*，一个字节，之后变成long int *，变为了4个字节
        //每次处理完后将处理过的a，b，c，d加上A，B，C，D用于下一波处理
        //这也是为什么四轮运算函数第一个要用指针或引用，要改变它的值
        for (j = 0; j < 4; j++)
            hashout[j] += hashin[j];
    }
    //处理输出，最终结果需要以低字节至高字节的顺序输出
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) 
            *((char*)(output + i) + j) = *((char*)(hashout + i) + 3 - j);//先将四字节变为一字节long int->char
}

int main() {
    int i;
    char Message[10000] = { 0 };
    unsigned long int output[4];
    cout << "请输入字符串：" << endl;
    cin>> Message;
    MD5(Message, strlen(Message), output);
    for (i = 0; i < 4; i++)
    {
        printf("%08lx", output[i]);//不能用cout<<hex<<output[i];不然可能会省略掉第一个0
    }
    return 0;
}

