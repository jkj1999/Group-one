#pragma once
// #ifndef _hashTree_H
// #define _hashTree_H

#include <iostream>
#include "md5.h"
#include <queue>
using namespace std;
class store_data_node
{
	//�洢ѹ��������ݵ���ײ�ڵ�
public:
	string data;
	string aft_compress(string data);//��������ѹ��,����ѹ��������
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
	store_data_node *head;//�ײ��ͷ���,���ڱ���
	MD5_hash md5;
	string retMd5(string info);
	hashTree() { root = NULL; head = NULL;}
	void createTree(string info);
	queue<hashNode*>* createFloor(queue<string>* store);//������ײ㣺���ַ���ת��ϣ
	queue<string>* split(string position, string parameter);
	hashNode* createNode(hashNode* left, hashNode* right);//left��right��δ����,����right��left�����½ڵ�
	void goOverTree(hashNode* node);//����debugʹ��,������ϣ��
	void goOverFloor();//����debugʹ�ã�������ײ���
	queue<int>* findChanged(hashNode* example, hashNode* mayChanged);//�ҳ���һ�µĶ���,ע�⣺������0��ʼ
	int isLeaf(hashNode* node);//�ж��Ƿ���Ҷ�ӽ��
};
//#endif
