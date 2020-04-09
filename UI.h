#pragma once
#include <iostream>
#include <conio.h>
#include "Blockchain.h"
#include "userManager.h"
using namespace std;
class UI
{
public:
	chain block_chain;
	userManager usermanager;//模拟用户端
	void default_set();
	void header();
	void footer();
	void store_news();//存储新闻
	void main_window();//主界面
	void find_difference();//篡改检查
	void key_search();//关键字查询
	int main_width;
	int main_height;
	string read_passage();//读取输入文章
	UI();
};