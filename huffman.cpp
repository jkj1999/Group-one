#include "huffman.h"

string HFM::compress(string passage)
{
	//cout << passage<<endl;
	string compress_passage;

	int node_number = 0;   //�ڵ���(�ַ�������)
	int total = passage.length();   //�����ַ�����
	int sum_length = 0;//��¼0/1������
	int root;//ָʾ���ڵ����ڵ������±�

	//ͳ�Ƹ����ַ���Ƶ��
	int tt = 0;
	bool flag = true;
	char ch;
	while (tt < total)
	{
		ch = passage[tt];
		//total++;//�ַ�����
		flag = true;
		for (int j = 0; j < node_number; j++) //������ǰ�ѱ�����ַ�
			if (ch == huf[j].data) // �Ѿ����ڸýڵ�
			{
				huf[j].weight++;//Ƶ��+1
				flag = false;
			}
		//�����������нڵ㣬�ýڵ㲻����ʱ
		if (flag) {
			huf[node_number].data = ch;
			huf[node_number].weight = 1;
			huf[node_number].left = huf[node_number].right = huf[node_number].parent = NULL;
			node_number++;
		}
		tt++;
	}

	//cout << "�ַ�����:" << total << endl;
	//cout << "�ڵ�����:" << node_number << endl;

	huf[200].weight = 9999999;//��Ϊ�ó���Сֵ�ıȽ�ý��

	//������������
	int number = node_number;  //�̶��������ݵĽڵ�number,����Ч�Ľڵ��� ���չ������������ɵ� node_number��Ҷ�ӽڵ��Ӧnode_number-1����Ҷ�ڵ�
	for (int i = 0; i < number - 1; i++)
	{
		int ct1 = 200, ct2 = 200;//����±�

		//Ѱ�ҵ�ǰƵ����С�������ڵ�
		//��Сֵ
		for (int j = 0; j < node_number; j++)
			if (huf[j].parent == NULL && huf[j].weight < huf[ct1].weight)//����parentָ���Ŀ�ģ������жϽڵ��Ƿ��Ѿ���ȡ��
				ct1 = j;
		//��Сֵ
		for (int j = 0; j < node_number; j++) {
			if (j != ct1 && huf[j].parent == NULL && huf[j].weight < huf[ct2].weight)
				ct2 = j;
		}
		//�ӵײ��򶥲㽨��
		huf[node_number].weight = huf[ct1].weight + huf[ct2].weight;
		huf[node_number].data = '\0';//��Ҷ�ӽڵ� ����Ч����
		huf[node_number].left = &huf[ct1];
		huf[node_number].right = &huf[ct2];
		huf[node_number].parent = NULL;
		huf[ct1].parent = &huf[node_number];
		huf[ct2].parent = &huf[node_number];
		node_number++;
	}
	root = node_number - 1;//rootΪ��������ͷָ�룬ΪĿǰ������Ч�ڵ�����һ��

	//cout << "��������������Ӧ�����±�:" << root << endl;
	//cout << "������������Ȩֵ:" << huf[root].weight << endl;


	//����  ��0��1
	queue<HFM_node*> q;

	strcpy_s(huf[root].binary, "0");      //Ϊ�˷��� ��ͷ��� Ҳ����Ϊ��0��
	strcpy_s(huf[root].left->binary, "0");
	strcpy_s(huf[root].right->binary, "1");
	q.push(huf[root].left);
	q.push(huf[root].right);
	while (!q.empty())
	{
		HFM_node* k = q.front();
		q.pop();
		if (k->left != NULL)
		{
			strcpy_s(k->left->binary, k->binary);
			strcat_s(k->left->binary, "0");//�������ַ�������
			q.push(k->left);
		}
		if (k->right != NULL)
		{
			strcpy_s(k->right->binary, k->binary);
			strcat_s(k->right->binary, "1");
			q.push(k->right);
		}
	}
	/*
	//����ַ��������
	for (int i = 0; i < number; i++) {
		cout << huf[i].data << ":" << huf[i].binary << endl;
	}
	*/

	//����,����0/1���ܳ�
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
	//cout << "0/1���ܳ��ȣ�" << sum_length << endl;

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

	for (int n = 1; n <= total; n++)//�����ļ�ȫ���ַ�
	{
		int i;
		//fin.get(ch);
		ch = passage[n - 1];

		//���������ַ��ڵ㣬�ҵ���Ӧ��0/1��
		for (i = 0; i < node_number; i++)
			if (ch == huf[i].data)
				break;

		//������0/1������
		//����Ĺ��ɣ���Ϊ�Ӹ�λ���������ƴ�������ÿ��ͨ�����ƽ��Ѵ���õĶ����ƴ���ǰ�ƣ��������һλ��λ��
		//���½���һλΪ0��������ֱ�ӿ�����0�������������������½���һλΪ0��ͨ����1�߼��򣬰�1�������һλ��������

		for (int j = 0; j < strlen(huf[i].binary); j++)
		{
			if ((huf[i].binary[j] - '0') == 1) {//��Ϊ'1'
				a = a | 1;//ͨ���߼���תΪ�����Ƶ�1
			}
			counts++;
			if (counts == 8)//ÿ8λ��һ��  ��Ϊchar���͵�aΪ1���ֽ�8����λ
				//������֤�����֪����ʣ��λ�����Դ���һ��0/1��ʱ����ʣ��λ����0
				//ͬ������֤�����֪����Ϊÿ�ζ���������ƣ������һλʼ����0 ���ܱ�֤ÿ��j��forѭ�������󣬸ô�ǡΪ�Ѵ���Ĵ������0/1��
			{
				compress_passage += a;
				//���ô洢�����8λ������
				a = '\0';
				counts = 0;
			}
			a = a << 1;//����1λ,Ϊ��һ��bit�ڳ�λ��
		}
	}

	//�������ʣ��Ĳ���8λ������������0����
	for (; counts <= 6; counts++)     //֮ǰ��ִ����һ������,�Ѿ�ִ��count+1��   ����,����forѭ��δ�ı�counts��ֱ��ִ����һ��
		a = a << 1;                    //�൱�ڶ�ִ�����������ƣ��������Ϊ���6�� 
	compress_passage += a;

	return compress_passage;
}

string HFM::uncompress(string hufpassage)
{
	string outcome = "";
	//int total = (int)hufpassage[0];//��0/1������
	//int ch_num = (int)hufpassage[1];//ԭ�����ַ��� ��ֹ�����0����
	//int number = (int)hufpassage[2];//�ַ�������
	int total, ch_num, number;//�ֱ�ָʾ����0/1�����ȡ�ԭ�����ַ�������ֹ�����0���룩���ַ������� 
	int p = return_start(total, ch_num, number, hufpassage);//pָ��ѹ�����ݵ��ײ�
	//cout << "0/1���ܳ���" << total << endl;
	//cout << "ԭ�����ַ�����" << ch_num << endl;
	//cout << "�ַ��ڵ�����" << number << endl;
	int binary[10];
	string temp = "";
	int len;
	if (total % 8 == 0) {
		len = total / 8;
	}
	else {
		len = total / 8 + 1;
	}

	for (int i = 0; i < len; i++)
	{
		char ch = hufpassage[p + i];
		int t[10];
		for (int i = 1; i <= 8; i++) {
			if (ch & 1)
				t[i] = 1;
			else
				t[i] = 0;
			ch = ch >> 1;
		}
		//������Ϊ����,��Ҫ����
		for (int i = 1; i <= 8; i++)
			binary[i] = t[9 - i];

		for (int j = 1; j <= 8; j++)
		{
			if (binary[j] == 0)
				temp += "0";
			else if (binary[j] == 1)
				temp += "1";
			for (int k = 0; k < number; k++)
			{
				if (temp == huf[k].binary) {
					outcome += huf[k].data;
					ch_num--;
					if (ch_num == 0)
					{
						return outcome;
					}
					temp = "";
					break;
				}
			}
		}
	}
	return outcome;
}

int HFM::return_start(int& a, int& b, int& c, string m)
{
	//��total|ch_num|number|hufpassage�洢
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
			//cout << atoi(temp.c_str()) << endl;
			temp = "";
			if (time == 3)
			{
				return i + 1;//p+1ָ��ѹ�����ݵĵ�һλ
			}
		}
	}
}