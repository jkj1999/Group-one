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
};
int main()
{
	chain chainTest;
	chainTest.addNode("jjjj\nffff\nfff\naaaee", "2018");
	chainTest.addNode("ccc\nggg\nsss", "2019");
	chainTest.addNode("jjjj\nffff\nfff\naaaee", "2020");
	chainTest.findChanged("jjjj\nfffffddf\nfgg\nfaa", "2020");
	chainTest.head->htree.goOverFloor();
}
node::node(string info, string time)
{
	htree.createTree(info);
	next = NULL;
	preHashCode = "";
	timeStamp = time;
}

void chain::addNode(string info, string time)
{
	if (!head)
	{
		head = new node(info, time);
		rear = head;
	}
	else
	{
		node* newNode = new node(info, time);
		newNode->preHashCode = rear->htree.root->hashCode;
		rear->next = newNode;
		rear = rear->next;
	}
}
string chain::retMd5(string info)
{
	return md5.encrypt(info);
}
void chain::goOverChain()
{
	node* temp = head;
	while (temp)
	{
		cout << temp->htree.root->hashCode << endl << endl;
		temp = temp->next;
	}
}

void chain::findChanged(string passage, string time)
{
	node* temp;
	for (temp = head;temp;temp = temp->next)
	{
		if (temp->timeStamp == time)
			break;
	}
	hashTree position;
	position.createTree(passage);
	queue<int>* changeParagraph = temp->htree.findChanged(position.root,temp->htree.root);
	if (!changeParagraph)
		cout << "完全一致"<<endl;
	else
	{
		while (changeParagraph->size())
		{
			cout << changeParagraph->front() + 1<<" ";
			changeParagraph->pop();
		}
		cout<<"发生篡改"<<endl;
	}
}
