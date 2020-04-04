#include "md5.h"

void MD5_hash::Calculation(unsigned long int* a, unsigned long int b, unsigned long int c, unsigned long int d, unsigned long int M,  unsigned long int t, int s, int choice)
{
    unsigned long int temp;
    //��������
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
//����Ϊ��hashָ����һ��ʼ��ʼ����A��B��C��D��֮�󲻶ϼ��ϴ˺����������a��b��c��d��MΪ16�����ֽ��ӷ���
void MD5_hash::MD5Calculation(const unsigned long int M[16], unsigned long int hash[4]) 
{
    int i, j, sequence = 0;
    //������FF��GG��HH��II����
    //ÿ�ְ���abcd��dabc��cdab��bcda�Ĵ����ظ������ĴΣ��ֱ��Ӧs11��s12��s13��s14...�����Ⱥ�x����������
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
//�����ַ������䳤�ȣ����output
void MD5_hash::MD5(char* M, int inputlen, unsigned long int output[4]) 
{
    int i, j;
    unsigned long int hashout[4] = { 0x67452301,0xefcdab89,0x98badcfe,0x10325476 };//��ʼ��A��B��C��D����Ҳ������Ҫ�����
    unsigned long int hashin[4];//���뵽�����д���Ĵ���hashout��
    //���
    long long previousbitslen = inputlen * 8;//�ֽ���ת��Ϊbit��
    int previouslen = inputlen;
    //������������448�����һ��1�Ͷ��0���ֽڳ���Ҳ��֮����
    while (inputlen % 64 != 56) 
    {
        M[inputlen] = 0;
        inputlen++;
    }
    M[previouslen] = 0x80;
    *(long long*)(M + inputlen) = previousbitslen;//���ʣ��64λ16���ֽڴ��ԭ����λ���������㷨Ҫ��longlong��ʾ����64λ
    inputlen += 8;//��չ����ֽ���Ҫ��������Ǹ�longlong,64�ı���
    //��ʼ�������
    for (i = 0; i < inputlen; i += 64) 
    {
        memcpy(hashin, hashout, sizeof(long int) * 4);//4��4�ֽڸ��Ƶ�hash����
        MD5Calculation((const unsigned long int*)&M[i], hashin);//����512bits���飬M��ת��������Ҫ��֮ǰ��char*��һ���ֽڣ�֮����long int *����Ϊ��4���ֽ�
        //ÿ�δ�����󽫴������a��b��c��d����A��B��C��D������һ������
        //��Ҳ��Ϊʲô�������㺯����һ��Ҫ��ָ������ã�Ҫ�ı�����ֵ
        for (j = 0; j < 4; j++)
            hashout[j] += hashin[j];
    }
    //������������ս����Ҫ�Ե��ֽ������ֽڵ�˳�����
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) 
            *((char*)(output + i) + j) = *((char*)(hashout + i) + 3 - j);//�Ƚ����ֽڱ�Ϊһ�ֽ�long int->char
}
string MD5_hash::get_md5_code(char *Message)
{
    int i;
    unsigned long int output[4];
    MD5(Message, strlen(Message), output);
    string code;
    for (i = 0; i < 4; i++)
    {
        //printf("%08lx", output[i]);//������cout<<hex<<output[i];��Ȼ���ܻ�ʡ�Ե���һ��0
        //code += to_string(output[i]);
        char temp[10];
        sprintf_s(temp, "%08lx", output[i]);
        code += temp;
    }
    return code;
}