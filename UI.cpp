#include "UI.h"
UI::UI()
{
	main_width = 25;
	main_height = 30;
}
void UI::header()
{
	for (int m = 0;m < main_width / 2;m++)
		cout<<" ";
	cout << "��ӭʹ��nbplus���ݹ���ϵͳ";
	cout<<endl;
	for(int m=0;m<main_width;m++)
		cout<<"��";
	cout<<endl<<endl;
}
void UI::footer()
{
	cout<<endl;
	for(int m=0;m<main_width;m++)
		cout<<"��";
}
void UI::main_window()
{
	default_set();
	while(1)
	{
		system("cls");
		header();
		for(int m=0;m<main_width/2+5;m++)
			cout<<" ";
		cout<<"1.�洢����"<<endl<<endl;
		for(int m=0;m<main_width/2+5;m++)
			cout<<" ";
		cout<<"2.�۸ļ��"<<endl<<endl;
		for(int m=0;m<main_width/2+4;m++)
			cout<<" ";
		cout<<"3.�ؼ��ֲ���"<<endl<<endl;
		footer();
		char choose;
		choose = _getch();
		switch(choose)
		{
			case '1':
				store_news();
				break;
			case '2':
				find_difference();
				break;
			case '3':
				key_search();
				break;
		}
	}
}
void UI::store_news()
{
	system("cls");
	string data,passage;
	cout<<"����������:";
	cin>>data;
	cout<<"����������:"<<endl;
	passage = read_passage();
	int juge = usermanager.sendMessageBySignal(passage,data);
	if(!juge)
	{
		cout<<"�����֤ʧ�ܻ���Ϣ�ѱ��۸ģ���˲�������ٽ��з���!"<<endl;
		_getch();
		return;
	}
	block_chain.addNode(passage,data,usermanager.getMac());
	cout<<"�洢���!"<<endl;
}
string UI::read_passage()
{
	getchar();//���˻��з�
	string passage;
	string paragraph;
	char word;
	word = getchar();
	while(word != '\n')
	{
		getline(cin,paragraph);
		passage += word+paragraph+'\n';
		word = getchar();
	}
	passage.pop_back();//ȥ�����Ļس�
	return passage;
}
void UI::find_difference()
{
	system("cls");
	string passage,time;
	cout<<"����������:"<<endl;
	cin>>time;
	cout<<"����������:"<<endl;
	passage = read_passage();
	block_chain.findChanged(passage,time);
	_getch();
}
void UI::key_search()
{
	system("cls");
	block_chain.keymatching();
	_getch();
}
void UI::default_set()
{
	block_chain.addNodeFromFile();
	usermanager.userLogin();
}