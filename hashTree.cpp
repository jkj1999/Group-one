#include "hashTree.h"

hashNode::hashNode(string info)
{
	left = right = NULL;
	hashCode = info;
	paragraph = -1;
	store_data = NULL;
}
void hashTree::createTree(string info)
{
	queue<string>* paragraphs = split(info, "\n");
	queue<hashNode*>* hashQueue = createFloor(paragraphs);
	int jugeNumber = hashQueue->size();
	while (jugeNumber > 1)
	{
		for (int m = 0;m < jugeNumber / 2;m++)
		{
			hashNode* left, * right;
			left = hashQueue->front();
			hashQueue->pop();
			right = hashQueue->front();
			hashQueue->pop();
			hashQueue->push(createNode(left, right));
		}
		if (jugeNumber % 2 == 1)
		{
			hashNode* temp = hashQueue->front();
			hashQueue->pop();
			string aftMd5 = md5.encrypt(temp->hashCode);
			hashNode* newCode = new hashNode(aftMd5);
			newCode->left = temp;
			hashQueue->push(newCode);
		}
		jugeNumber = hashQueue->size();
	}
	root = hashQueue->front();
	hashQueue->pop();
}
queue<string>* hashTree::split(string position, string parameter)
{
	queue<string>* store = new queue<string>;
	position += parameter;
	int lenPara = parameter.length();
	int end = position.length() - lenPara;
	int pre = position.find(parameter);
	while (pre != end)
	{
		string cat = position.substr(0, pre);
		store->push(cat);
		int length = position.length() - lenPara - pre;
		position = position.substr(pre + lenPara, length);
		pre = position.find(parameter);
		end = position.length() - lenPara;
	}
	store->push(position.substr(0, pre));
	return store;
}
queue<hashNode*>* hashTree::createFloor(queue<string>* store)
{
	queue<hashNode*>* tempStore = new queue<hashNode*>;
	int size = store->size();
	store_data_node*rear=NULL;
	for (int m = 0;m < size;m++)
	{
		string aftMd5 = md5.encrypt(store->front());
		hashNode* temp = new hashNode(aftMd5);
		temp->paragraph = m;
		store_data_node * data_store = new store_data_node(store->front());
		if(!head)
		{
			head = data_store;
			rear = head;
		}
		else
		{
			data_store->right = rear->right;
			rear->right = data_store;
			rear = data_store;
		}
		temp->store_data = data_store;
		tempStore->push(temp);
		store->pop();
	}
	return tempStore;
}
hashNode* hashTree::createNode(hashNode* left, hashNode* right)
{
	string sumMd5 = md5.encrypt(left->hashCode + right->hashCode);
	hashNode* temp = new hashNode(sumMd5);
	temp->left = left;
	temp->right = right;
	return temp;
}
void hashTree::goOverTree(hashNode* node)
{
	if (!node)
		return;
	cout << node->hashCode << endl;
	goOverTree(node->left);
	goOverTree(node->right);
}
int hashTree::isLeaf(hashNode* node)//判断是否是叶子结点
{
	if (node->left == NULL && node->right == NULL)
		return true;
	else
		return false;
}
queue<int>* hashTree::findChanged(hashNode* example, hashNode* mayChanged)
{
	queue<int>* changedParagraph = new queue<int>;
	queue<hashNode*>exampleQueue, changedQueue;
	if (!example)
		return NULL;
	if (example->hashCode == mayChanged->hashCode)
		return NULL;//没有篡改的段落
	exampleQueue.push(example);
	changedQueue.push(mayChanged);
	while (exampleQueue.size())//直到队列为空
	{
		hashNode* tempExam = exampleQueue.front();
		exampleQueue.pop();
		hashNode* tempChanged = changedQueue.front();
		changedQueue.pop();
		if (tempExam->hashCode != tempChanged->hashCode)
		{
			if (!isLeaf(tempChanged))
			{
				if (tempExam->left)
					exampleQueue.push(tempExam->left);
				if (tempExam->right)
					exampleQueue.push(tempExam->right);
				if (tempChanged->left)
					changedQueue.push(tempChanged->left);
				if (tempChanged->right)
					changedQueue.push(tempChanged->right);
			}
			else
			{
				changedParagraph->push(tempChanged->paragraph);
			}
		}
	}
	return changedParagraph;
}
store_data_node::store_data_node(string paragraph)
{
	data = aft_compress(paragraph);
	right = NULL;
}
string store_data_node::aft_compress(string data)
{
	//进行哈夫曼编码
	return data;
}
void hashTree::goOverFloor()
{
	store_data_node *temp = head;
	for(;temp;temp=temp->right)
	{
		cout<<temp->data<<" ";
	}
}