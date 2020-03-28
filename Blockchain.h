#pragma once

#include <iostream>
#include "MD5.h"
#include "hashTree.h"
using namespace std;
class node
{
public:
	node(string info, string time);
	hashTree htree;//此结点的哈希树
	string preHashCode;//上一个哈希结点的哈希值
	string timeStamp;//时间戳
	node* next;
};
class chain
{
	
public:
	node* head, * rear;
	MD5 md5;//哈希函数
	chain() { head = rear = NULL; }
	void addNode(string info, string time);//添加区块结点
	void goOverChain();//debug用
	string retMd5(string info);
	void findChanged(string passage, string time);//找到篡改段落
	void keymatching();            //关键字查找
};