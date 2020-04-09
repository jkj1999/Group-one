#pragma once

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
	char Message[10000] = { 0 };
    const char* temp = info.c_str();
    for (int m = 0;m < strlen(temp);m++)
        Message[m] = temp[m];
    return md5.get_md5_code(Message);
}
void chain::goOverChain()
{
	node* temp = head;
	while (temp)
	{
		cout << temp->htree.root->hashCode << endl << endl;
		//cout<<temp->timeStamp<<endl<<endl;
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
		cout << "��ȫһ��"<<endl;
	else
	{
		while (changeParagraph->size())
		{
			cout <<"����"<< changeParagraph->front() + 1<<" ";
			changeParagraph->pop();
		}
		cout<<"�����۸�"<<endl;
	}
}
void chain::keymatching() {
	int keys_num;
	int valueindex[3] = { -1,-1,-1 };         //�ؼ��ʳ��ִ���ǰ�������µ��±� 
	int valuenum[3] = { -1,-1,-1 };           //�ؼ��ʳ��ִ��� 
	cout << "����������Ҫ�����Ĺؼ��ʸ���" << endl;
	cin >> keys_num;
	cout << "����������Ҫ��ѯ�Ĺؼ���(�԰�Ƕ���Ϊ������س�Ϊ����)" << endl;
	string keys;
	cin >> keys;
	node* temp_head = head;
	int news_num = 0;                         //��¼�����±�(��ʼΪ0)
	for (;temp_head != NULL;temp_head = temp_head->next) {      //��ÿƪ���µĹؼ��ʸ���
		//cout<<temp_head->timeStamp<<endl;
		store_data_node* temp_data = temp_head->htree.head;     //ָ��ײ���
		int count = 0;
		for (;temp_data != NULL;temp_data = temp_data->right) {       //�õ�count
			int temp_keynum = keys_num;
			int keyindex = 0;
			for (;temp_keynum > 0;temp_keynum--) {                         //�ֱ���temp_keynum���ؼ��ֵĸ��� 
				//cout << count << endl;
				int keylen = 0;
				string paragraph = temp_data->decompress(temp_data->data);//�����½�ѹ
				for (;keys[keylen + keyindex] != '\0' && keys[keylen + keyindex] != ',';keylen++);      //���ַ���key�ĳ���
				int index = 0, wordlen = 0;           //index�����µ�ǰλ�ã�count�ǳ��ִ�����wordlen��������ָ��ĵ��ʳ���
				while (paragraph[index] != '\0') {
					wordlen = 0;
					bool yn;                            //������ؼ����Ƿ���� 
					while (paragraph[index] == ' '|| paragraph[index] == ','|| paragraph[index] == '.'|| paragraph[index] == '\''|| paragraph[index] == '\"'|| paragraph[index] == '?' || paragraph[index] == '!')            //���˿ո� 
						index++;
					while (paragraph[index + wordlen] != '\0' && paragraph[index + wordlen] != ' ' && paragraph[index+wordlen] != ',' && paragraph[index+wordlen] != '.' && paragraph[index + wordlen] != '\'' && paragraph[index + wordlen] != '\"' && paragraph[index + wordlen] != '?' && paragraph[index + wordlen] != '!')    //wordlen�󵥴ʳ���(���Կ��������������)
						wordlen++;
					if (wordlen != keylen)
						index += wordlen;
					else {
						yn = true;
						int temp_index = index;
						int worddex = 0;
						while (worddex < wordlen) {
							if (paragraph[temp_index] != keys[worddex + keyindex]) {       //�и�
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
				//for (;keys[keyindex] == ' ';keyindex++);  //���˹ؼ����м�Ŀո� 
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
		cout << "������Ĺؼ������޷�ƥ���κ����£���˶Ժ��ٴ�����" << endl;
	else if (valuenum[1] == 0)
		cout << "������Ĺؼ��������ƥ��һƪ����" << endl;
	else if (valuenum[2] == 0)
		cout << "������Ĺؼ��������ƥ����ƪ����" << endl;
	for (int temp_i = 0;temp_i < 3 && valuenum[temp_i] > 0;temp_i++) {
		int temp_new = valueindex[temp_i];
		int i = 0;
		for (temp_head = head;i != temp_new;temp_head = temp_head->next)
			i++;
		cout << "�����¹����ؼ���" << valuenum[temp_i] << "��" << endl;
		store_data_node * temp_data_head = temp_head->htree.head;
		// for (;temp_data_head != NULL;temp_data_head = temp_data_head->right)
		// 	cout << "  " << temp_data_head->data << endl;
		for (;temp_data_head != NULL;temp_data_head = temp_data_head->right)
		{
			cout << "  " << temp_data_head->decompress(temp_data_head->data) << endl;
		}
		cout << "ȫ��������" << endl;

	}
}

void chain::addNodeFromFile()
{
	queue<read_for_thread> info_one,info_two,info_three,info_four;

	thread read_thread_one(addNodeFromFile_thread,1,50,&info_one);
	thread read_thread_two(addNodeFromFile_thread,51,100,&info_two);
	thread read_thread_three(addNodeFromFile_thread,101,150,&info_three);
	thread read_thread_four(addNodeFromFile_thread,151,205,&info_four);
	read_thread_one.join();
	read_thread_two.join();
	read_thread_three.join();
	read_thread_four.join();
	addNode_from_queue(info_one);
	addNode_from_queue(info_two);
	addNode_from_queue(info_three);
	addNode_from_queue(info_four);

	// goOverChain();
	// char temp;
	// cin>>temp;
}
void chain:: addNodeFromFile_thread(int begin,int end,queue<read_for_thread>* info_store)
{
	for(int m=begin;m<=end;m++)
	{
		string file_name = "News/"+to_string(m)+".txt";
		
		ifstream in(file_name);
		string time,passage,temp;
		getline(in,time);
		while(getline(in,temp))
		{
			passage += temp+'\n';
		}
		in.close();
		passage.pop_back();
		read_for_thread temp_store;
		temp_store.time = time;
		temp_store.passage = passage;
		info_store->push(temp_store);
	}
}
void chain:: addNode_from_queue(queue<read_for_thread>& info_queue)
{
	while(!info_queue.empty())
	{
		//cout<<info_queue.front().time<<endl;
		addNode(info_queue.front().passage,info_queue.front().time);
		info_queue.pop();
	}
}