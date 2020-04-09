#include "huffman.h"

string HFM::compress(string passage)
{
	string compress_passage;

	int node_number = 0;   //节点数(字符种类数)
	int total = passage.length();   //计算字符总数
	int sum_length = 0;//记录0/1串长度
	int root;//指示根节点所在的数组下标

	//统计各个字符的频数
	int tt = 0;
	bool flag = true;
	char ch;
	while (tt < total)
	{
		ch = passage[tt];
		//total++;//字符总数
		flag = true;
		for (int j = 0; j < node_number; j++) //遍历当前已保存的字符
			if (ch == huf[j].data) // 已经存在该节点
			{
				huf[j].weight++;//频度+1
				flag = false;
			}
		//当遍历完所有节点，该节点不存在时
		if (flag) {
			huf[node_number].data = ch;
			huf[node_number].weight = 1;
			huf[node_number].left = huf[node_number].right = huf[node_number].parent = NULL;
			node_number++;
		}
		tt++;
	}

	cout << "字符总数:" << total << endl;
	cout << "节点总数:" << node_number << endl;

	huf[200].weight = 9999999;//作为得出最小值的比较媒介

	//构建哈夫曼树
	int number = node_number;  //固定的有内容的节点number,即有效的节点数 按照哈夫曼构造规则可得 node_number个叶子节点对应node_number-1个非叶节点
	for (int i = 0; i < number - 1; i++)
	{
		int ct1 = 200, ct2 = 200;//标记下标

		//寻找当前频度最小的两个节点
		//最小值
		for (int j = 0; j < node_number; j++)
			if (huf[j].parent == NULL && huf[j].weight < huf[ct1].weight)//设置parent指针的目的，用于判断节点是否已经被取用
				ct1 = j;
		//次小值
		for (int j = 0; j < node_number; j++) {
			if (j != ct1 && huf[j].parent == NULL && huf[j].weight < huf[ct2].weight)
				ct2 = j;
		}
		//从底层向顶层建树
		huf[node_number].weight = huf[ct1].weight + huf[ct2].weight;
		huf[node_number].data = '\0';//非叶子节点 无有效数据
		huf[node_number].left = &huf[ct1];
		huf[node_number].right = &huf[ct2];
		huf[node_number].parent = NULL;
		huf[ct1].parent = &huf[node_number];
		huf[ct2].parent = &huf[node_number];
		node_number++;
	}
	root = node_number - 1;//root为哈夫曼树头指针，为目前所有有效节点的最后一个

	//cout << "哈夫曼树树根对应数组下标:" << root << endl;
	//cout << "哈夫曼树树根权值:" << huf[root].weight << endl;


	//编码  左0右1
	queue<node*> q;

	strcpy(huf[root].binary, "0");      //为了方便 把头结点 也编码为‘0’
	strcpy(huf[root].left->binary, "0");
	strcpy(huf[root].right->binary, "1");
	q.push(huf[root].left);
	q.push(huf[root].right);
	while (!q.empty())
	{
		node* k = q.front();
		q.pop();
		if (k->left != NULL)
		{
			strcpy(k->left->binary, k->binary);
			strcat(k->left->binary, "0");//将两个字符串连接
			q.push(k->left);
		}
		if (k->right != NULL)
		{
			strcpy(k->right->binary, k->binary);
			strcat(k->right->binary, "1");
			q.push(k->right);
		}
	}
	/*
	//输出字符及其编码
	for (int i = 0; i < number; i++) {
		cout << huf[i].data << ":" << huf[i].binary << endl;
	}
	*/

	//遍历,计算0/1串总长
	tt = 0;
	while (tt < total)
	{
		ch = passage[tt];
		for (int i = 0; i < number; i++) {
			if (ch == huf[i].data) {
				sum_length += strlen(huf[i].binary);
				break;
			}
		}
		tt++;
	}
	cout << "0/1串总长度：" << sum_length << endl;

	//compress_passage += (char*)&sum_length;
	//compress_passage += (char*)&total;
	//compress_passage += (char*)&number;
	compress_passage += std::to_string(sum_length);
	compress_passage += "|";
	compress_passage += std::to_string(total);
	compress_passage += "|";
	compress_passage += std::to_string(number);
	compress_passage += "|";

	char a = '\0';
	int counts = 0;
	int max_length = 0;

	for (int n = 1; n <= total; n++)//处理文件全部字符
	{
		int i;
		//fin.get(ch);
		ch = passage[n - 1];

		//遍历所有字符节点，找到对应的0/1串
		for (i = 0; i < node_number; i++)
			if (ch == huf[i].data)
				break;

		//将整个0/1串存入
		//存入的规律：因为从高位遍历二进制串，所以每次通过左移将已处理好的二进制串向前移，留出最后一位的位置
		//若新进的一位为0，则左移直接可以添0，无需其他操作；若新进的一位为0，通过与1逻辑或，把1放入最后一位，再左移

		for (int j = 0; j < strlen(huf[i].binary); j++)
		{
			if ((huf[i].binary[j] - '0') == 1) {//若为'1'
				a = a | 1;//通过逻辑或转为二进制的1
			}
			counts++;
			if (counts == 8)//每8位存一次  因为char类型的a为1个字节8比特位
				//根据验证结果可知，当剩余位不足以存下一个0/1串时，则剩余位保留0
				//同样由验证结果可知，因为每次都是最后左移，故最后一位始终是0 不能保证每次j的for循环结束后，该串恰为已处理的串的组合0/1串
			{
				compress_passage += a;
				//重置存储载体和8位计数器
				a = '\0';
				counts = 0;
			}
			a = a << 1;//左移1位,为下一个bit腾出位置
		}
	}

	//处理最后剩余的不足8位，单纯左移添0处理
	for (; counts <= 6; counts++)     //之前多执行了一次左移,已经执行count+1次   并且,进入for循环未改变counts又直接执行了一次
		a = a << 1;                    //相当于多执行了两次左移，因此限制为最多6次 
	compress_passage += a;

	return compress_passage;
}

string HFM::uncompress(string hufpassage)
{
	string outcome = "";
	//int total = (int)hufpassage[0];//总0/1串长度
	//int ch_num = (int)hufpassage[1];//原文章字符数 防止将多个0译码
	//int number = (int)hufpassage[2];//字符种类数
	int total, ch_num, number;//分别指示：总0/1串长度、原文章字符数（防止将多个0译码）、字符种类数 
	int p = return_start(total, ch_num, number, hufpassage);//p指向压缩数据的首部
	cout << "0/1串总长：" << total << endl;
	cout << "原文章字符数：" << ch_num << endl;
	cout << "字符节点数：" << number << endl;
	int binary[10];
	string temp = "";
	int len;
	if (total % 8 == 0) {
		len = total / 8;
	}
	else {
		len = total / 8 + 1;
	}
  //解码
	node* head;
	//先读取一次
	char ch = hufpassage[p];
	for (int i = 1; i <= 8; i++) {
		if (ch & 1)
			binary[i] = 1;
		else
			binary[i] = 0;
		ch = ch >> 1;
	}

	int couts = 8;
	for (int i = 0; i < ch_num; i++) {

		head = &huf[2*number-2];

		while (1) {
			if (binary[couts])    //左为 0  右为 1
				head = head->right;
			else
				head = head->left;

			couts--;

			if (!couts) {   //再次读取
				ch = hufpassage[++p];
				for (int i = 1; i <= 8; i++) {
					if (ch & 1)
						binary[i] = 1;
					else
						binary[i] = 0;
					ch = ch >> 1;
				}
				couts = 8;
			}

			if (head->data != '\0') {
				outcome += head->data;
				break;
			}
		}
	}
	return outcome;
}

int return_start(int& a, int& b, int& c, string m)
{
	//用total|ch_num|number|hufpassage存储
	int time = 0;
	char mid = '|';
	string temp = "";

	for (int i = 0; i < 100; i++)
	{
		temp += m[i];
		if (m[i] == mid)
		{
			time++;
			switch (time) {
			case 1:a = atoi(temp.c_str()); break;
			case 2:b = atoi(temp.c_str()); break;
			case 3:c = atoi(temp.c_str());
			}
			cout << atoi(temp.c_str()) << endl;
			temp = "";
			if (time == 3)
			{
				return i + 1;//p+1指向压缩数据的第一位
			}
		}
	}
}
