#include "Blockchain.h"
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
			cout <<"段落"<< changeParagraph->front() + 1<<" ";
			changeParagraph->pop();
		}
		cout<<"发生篡改"<<endl;
	}
}
void chain::keymatching() {
	int keys_num;
	int valueindex[3] = { -1,-1,-1 };         //关键词出现次数前三的文章的下标 
	int valuenum[3] = { -1,-1,-1 };           //关键词出现次数 
	cout << "请输入您想要检索的关键词个数" << endl;
	cin >> keys_num;
	cout << "请输入你想要查询的关键词(以半角逗号为间隔，回车为结束)" << endl;
	string keys;
	cin >> keys;
	node* temp_head = head;
	int news_num = 0;                         //记录文章下标(起始为0)
	for (;temp_head != NULL;temp_head = temp_head->next) {      //求每篇文章的关键词个数
		store_data_node* temp_data = temp_head->htree.head;     //指向底层结点
		int count = 0;
		for (;temp_data != NULL;temp_data = temp_data->right) {       //得到count
			int temp_keynum = keys_num;
			int keyindex = 0;
			for (;temp_keynum > 0;temp_keynum--) {                         //分别求temp_keynum个关键字的个数 
				//cout << count << endl;
				int keylen = 0;
				for (;keys[keylen + keyindex] != '\0' && keys[keylen + keyindex] != ',';keylen++);      //求字符串key的长度
				int index = 0, wordlen = 0;           //index是文章当前位置，count是出现次数，wordlen是文章中指向的单词长度
				while (temp_data->data[index] != '\0') {
					wordlen = 0;
					bool yn;                            //单词与关键词是否相等 
					while (temp_data->data[index] == ' '|| temp_data->data[index] == ','|| temp_data->data[index] == '.'|| temp_data->data[index] == '\''|| temp_data->data[index] == '\"'|| temp_data->data[index] == '?' || temp_data->data[index] == '!')            //过滤空格 
						index++;
					while (temp_data->data[index + wordlen] != '\0' && temp_data->data[index + wordlen] != ' ' && temp_data->data[index+wordlen] != ',' && temp_data->data[index+wordlen] != '.' && temp_data->data[index + wordlen] != '\'' && temp_data->data[index + wordlen] != '\"' && temp_data->data[index + wordlen] != '?' && temp_data->data[index + wordlen] != '!')    //wordlen求单词长度(可以考虑两个两个相加)
						wordlen++;
					if (wordlen != keylen)
						index += wordlen;
					else {
						yn = true;
						int temp_index = index;
						int worddex = 0;
						while (worddex < wordlen) {
							if (temp_data->data[temp_index] != keys[worddex + keyindex]) {       //有改
								yn = false;
								break;
							}
							worddex++;
							temp_index++;
						}
						if (yn)
							count++;
						index += wordlen;
					}
				}
				keyindex += keylen;
				//for (;keys[keyindex] == ' ';keyindex++);  //过滤关键字中间的空格 
				while(keys[keyindex] != '\0' && keys[keyindex] == ',')
					keyindex++;
			}
		}
		if (count > valuenum[2]) {
			if (valuenum[1] >= count) {
				valueindex[2] = news_num;
				valuenum[2] = count;
			}
			else {
				if (valuenum[0] >= count) {
					valueindex[2] = valueindex[1];
					valuenum[2] = valuenum[1];
					valueindex[1] = news_num;
					valuenum[1] = count;
				}
				else {
					valueindex[2] = valueindex[1];
					valuenum[2] = valuenum[1];
					valueindex[1] = valueindex[0];
					valuenum[1] = valuenum[0];
					valueindex[0] = news_num;
					valuenum[0] = count;
				}
			}
		}
		news_num += 1;
	}
	if (valuenum[0] == 0)
		cout << "您输入的关键词组无法匹配任何文章，请核对后再次输入" << endl;
	else if (valuenum[1] == 0)
		cout << "您输入的关键词组仅能匹配一篇文章" << endl;
	else if (valuenum[2] == 0)
		cout << "您输入的关键词组仅能匹配两篇文章" << endl;
	for (int temp_i = 0;temp_i < 3 && valuenum[temp_i] != 0;temp_i++) {
		int temp_new = valueindex[temp_i];
		int i = 0;
		for (temp_head = head;i != temp_new;temp_head = temp_head->next)
			i++;
		cout << "该文章共含关键词" << valuenum[temp_i] << "个" << endl;
		store_data_node * temp_data_head = temp_head->htree.head;
		for (;temp_data_head != NULL;temp_data_head = temp_data_head->right)
			cout << "  " << temp_data_head->data << endl;
		cout << "全文输出完毕" << endl;

	}
}