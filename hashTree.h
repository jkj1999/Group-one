#pragma once
// #ifndef _hashTree_H
// #define _hashTree_H

#include <iostream>
#include "md5.h"
#include <queue>
using namespace std;
class store_data_node
{
	//存储压缩后的数据的最底层节点
public:
	string data;
	string aft_compress(string data);//进行数据压缩,返回压缩后数据
	store_data_node* right;
	store_data_node(string paragraph);
};
class hashNode
{
public:
	hashNode* left, * right;
	string hashCode;
	hashNode(string info);
	int paragraph;
	store_data_node *store_data;
};

class hashTree
{
public:
	hashNode* root;
	store_data_node *head;//底层的头结点,便于遍历
	MD5_hash md5;
	string retMd5(string info);
	hashTree() { root = NULL; head = NULL;}
	void createTree(string info);
	queue<hashNode*>* createFloor(queue<string>* store);//创建最底层：将字符串转哈希
	queue<string>* split(string position, string parameter);
	hashNode* createNode(hashNode* left, hashNode* right);//left与right均未加密,依据right与left创建新节点
	void goOverTree(hashNode* node);//仅供debug使用,遍历哈希树
	void goOverFloor();//仅供debug使用，遍历最底层结点
	queue<int>* findChanged(hashNode* example, hashNode* mayChanged);//找出不一致的段落,注意：段落由0开始
	int isLeaf(hashNode* node);//判断是否是叶子结点
};
//#endif
