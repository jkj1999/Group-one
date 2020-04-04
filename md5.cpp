#include "md5.h"

void MD5_hash::Calculation(unsigned long int* a, unsigned long int b, unsigned long int c, unsigned long int d, unsigned long int M,  unsigned long int t, int s, int choice)
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
void MD5_hash::MD5Calculation(const unsigned long int M[16], unsigned long int hash[4]) 
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
void MD5_hash::MD5(char* M, int inputlen, unsigned long int output[4]) 
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
string MD5_hash::get_md5_code(char *Message)
{
    int i;
    unsigned long int output[4];
    MD5(Message, strlen(Message), output);
    string code;
    for (i = 0; i < 4; i++)
    {
        //printf("%08lx", output[i]);//不能用cout<<hex<<output[i];不然可能会省略掉第一个0
        //code += to_string(output[i]);
        char temp[10];
        sprintf_s(temp, "%08lx", output[i]);
        code += temp;
    }
    return code;
}