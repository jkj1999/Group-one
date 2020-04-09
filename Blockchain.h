#pragma once

#include <iostream>
#include "md5.h"
#include "hashTree.h"
#include <fstream>
#include <iomanip>
#include <thread>
#include <queue>
using namespace std;
struct read_for_thread
{
	string time;
	string passage;
};
class node
{
public:
	node(string info, string time,string mac="");
	hashTree htree;//�˽��Ĺ�ϣ��
	string preHashCode;//��һ����ϣ���Ĺ�ϣֵ
	string timeStamp;//ʱ���
	node* next;
	string mac;//��¼�ϴ���mac��ַ����Դ
};
class chain
{
public:
	node* head, * rear;
	MD5_hash md5;//��ϣ����
	chain() { head = rear = NULL; }
	void addNode(string info, string time,string mac="");//���������
	void goOverChain();//debug��
	string retMd5(string info);
	void findChanged(string passage, string time);//�ҵ��۸Ķ���
	void keymatching();            //�ؼ��ֲ���
	void addNodeFromFile();//���ļ���������
	void addNode_from_queue(queue<read_for_thread>& info_queue);
	static void addNodeFromFile_thread(int begin,int end,queue<read_for_thread>* info_store);
};