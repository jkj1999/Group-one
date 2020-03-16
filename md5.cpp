/*
 *   file: md5.cpp
 *   author: Els-y
 *   time: 2017-10-16 21:08:21
*/
#include "MD5.h"

/* -- public --*/
MD5::MD5() {
    buffer = NULL;
    round_funcs[0] = &MD5::f_rf;
    round_funcs[1] = &MD5::g_rf;
    round_funcs[2] = &MD5::h_rf;
    round_funcs[3] = &MD5::i_rf;
}

MD5::~MD5() {
    clear();
}

string MD5::encrypt(string plain) {
    init_md();
    clear();
    padding(plain);

    int group_len = buffer_len / 64;

    for (int i = 0; i < group_len; ++i) h_md5(i);

    return md2str();
}

void MD5::print_buff() {
    cout << "buffer_len = " << buffer_len << endl;
    for (int i = 0; i < buffer_len; ++i) {
        bitset<8> ch = buffer[i];
        cout << ch << " ";
    }
    cout << endl;
}

/* -- private --*/
// 初始化 MD 缓冲区
void MD5::init_md() {
    md = vector<unsigned int>({ 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 });
}

// 填充 padding 和 length
void MD5::padding(string plain) {
    unsigned int plain_len = plain.size();
    unsigned long long plain_bits_len = plain.size() * 8;
    unsigned int fill_bits_len = plain_bits_len % 512 == 448 ? 512 : (960 - plain_bits_len % 512) % 512;
    unsigned int fill_len = fill_bits_len / 8;
    buffer_len = plain_len + fill_len + 8;
    buffer = new unsigned char[buffer_len];

    // 复制原始消息
    for (int i = 0; i < plain_len; ++i) buffer[i] = plain[i];

    // 填充 padding
    buffer[plain_len] = 0x80;
    for (int i = 1; i < fill_len; ++i) buffer[plain_len + i] = 0;

    // 填充原始消息 length
    for (int i = 0; i < 8; ++i) {
        unsigned char ch = plain_bits_len;
        buffer[plain_len + fill_len + i] = ch;
        plain_bits_len >>= 8;
    }
}

void MD5::clear() {
    if (buffer != NULL) {
        delete[]buffer;
        buffer = NULL;
    }
}

void MD5::h_md5(int groupid) {
    int buff_begin = 64 * groupid;

    unsigned int next;
    vector<unsigned int> last_md(md);

    const unsigned int CYCLE_BITS[4][4] = {
        {7, 12, 17, 22},
        {5, 9, 14, 20},
        {4, 11, 16, 23},
        {6, 10, 15, 21}
    };

    // round = [0, 1, 2, 3] 分别对应 [F, G, H, I] 轮
    for (int round = 0; round < 4; ++round) {
        for (int i = 0; i < 16; ++i) {
            unsigned int x = uchar2uint(buff_begin + get_x_index(round, i) * 4);
            unsigned int t = 0x100000000UL * abs(sin(round * 16 + i + 1));
            next = md[1] + cycle_left_shift(md[0] + (this->*round_funcs[round])(md[1], md[2], md[3]) + x + t, CYCLE_BITS[round][i % 4]);
            // (A, B, C, D) 循环右移
            md[0] = md[3];
            md[3] = md[2];
            md[2] = md[1];
            md[1] = next;
        }
    }

    for (int i = 0; i < 4; ++i) md[i] += last_md[i];
}

// 4 个轮函数
unsigned int MD5::f_rf(unsigned int x, unsigned int y, unsigned int z) {
    return (x & y) | (~x & z);
}

unsigned int MD5::g_rf(unsigned int x, unsigned int y, unsigned int z) {
    return (x & z) | (y & ~z);
}

unsigned int MD5::h_rf(unsigned int x, unsigned int y, unsigned int z) {
    return x ^ y ^ z;
}

unsigned int MD5::i_rf(unsigned int x, unsigned int y, unsigned int z) {
    return y ^ (x | ~z);
}

// 返回 MD 缓冲区转换后的 string 格式密文
string MD5::md2str() {
    string res;

    for (int i = 0; i < 4; ++i) {
        unsigned int val = md[i];
        for (int j = 0; j < 4; ++j) {
            unsigned char ch = val;
            res += uchar2hex(ch);
            val >>= 8;
        }
    }

    return res;
}

// 返回 buffer 中 [pos, pos + 3] 四个字节按照 little-endian 组成的 X
unsigned int MD5::uchar2uint(int pos) {
    unsigned int val = 0;
    int end = pos + 3;
    for (int i = end; i >= pos; --i) {
        val |= buffer[i];
        if (i != pos) val <<= 8;
    }
    return val;
}

// 返回 unsigned char 对应的十六进制 string
string MD5::uchar2hex(unsigned char uch) {
    string res;
    unsigned char mask = 0x0F;

    for (int i = 1; i >= 0; --i) {
        char ch = uch >> (i << 2)& mask;
        if (ch < 10) ch += '0';
        else ch += 'A' - 10;
        res += ch;
    }

    return res;
}

// 返回 val 循环左移　bits 位的值
unsigned int MD5::cycle_left_shift(unsigned int val, int bits) {
    bits %= 32;
    return (val << bits) | (val >> (32 - bits));
}

// 返回第 round 轮迭代中，第 step　步的 X 对应下标
int MD5::get_x_index(int round, int step) {
    if (round == 0) {
        return step;
    }
    else if (round == 1) {
        return (1 + 5 * step) % 16;
    }
    else if (round == 2) {
        return (5 + 3 * step) % 16;
    }
    else {
        return (7 * step) % 16;
    }
}
