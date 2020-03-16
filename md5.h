/*
 *   file: md5.hpp
 *   author: Els-y
 *   time: 2017-10-16 21:08:21
*/
#ifndef _MD5_H
#define _MD5_H

#include <string>
#include <vector>
#include <cstring>
#include <cmath>
#include <iostream>
#include <bitset>
using std::string;
using std::vector;
using std::bitset;
using std::cout;
using std::endl;
using std::sin;
using std::abs;

// default little-endian
class MD5 {
public:
    MD5();
    ~MD5();
    string encrypt(string plain);
    // 输出扩展后的消息
    void print_buff();

private:
    // 128 位 MD 缓冲区，md[0...3] = {A, B, C, D}
    vector<unsigned int> md;
    // 存储扩展后的消息
    unsigned char* buffer;
    // 扩展后的消息长度，以字节为单位
    unsigned int buffer_len;
    // 存放 4 个轮函数的数组
    unsigned int (MD5::* round_funcs[4])(unsigned int, unsigned int, unsigned int);

    // 初始化 MD 缓冲区
    void init_md();
    // 填充 padding 和 length
    void padding(string plain);
    void clear();
    void h_md5(int groupid);
    // 4 个轮函数
    unsigned int f_rf(unsigned int x, unsigned int y, unsigned int z);
    unsigned int g_rf(unsigned int x, unsigned int y, unsigned int z);
    unsigned int h_rf(unsigned int x, unsigned int y, unsigned int z);
    unsigned int i_rf(unsigned int x, unsigned int y, unsigned int z);
    // 返回 MD 缓冲区转换后的 string 格式密文 
    string md2str();
    // 返回 buffer 中 [pos, pos + 3] 四个字节按照 little-endian 组成的 X
    unsigned int uchar2uint(int pos);
    // 返回 unsigned char 对应的十六进制 string
    string uchar2hex(unsigned char uch);
    // 返回 val 循环左移　bits 位的值
    unsigned int cycle_left_shift(unsigned int val, int bits);
    // 返回第 round 轮迭代中，第 step　步的 X 对应下标
    int get_x_index(int round, int step);
};

#endif
