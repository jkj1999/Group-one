#pragma once
#include <iostream>
#include <conio.h>
#include "Blockchain.h"
using namespace std;
class UI
{
public:
	chain block_chain;
	void default_set();
	void header();
	void footer();
	void store_news();//�洢����
	void main_window();//������
	void find_difference();//�۸ļ��
	void key_search();//�ؼ��ֲ�ѯ
	int main_width;
	int main_height;
	string read_passage();//��ȡ��������
	UI();
};