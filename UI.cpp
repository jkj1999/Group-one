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
	cout << "欢迎使用nbplus数据管理系统";
	cout<<endl;
	for(int m=0;m<main_width;m++)
		cout<<"█";
	cout<<endl<<endl;
}
void UI::footer()
{
	cout<<endl;
	for(int m=0;m<main_width;m++)
		cout<<"█";
}
void UI::main_window()
{
	while(1)
	{
		system("cls");
		header();
		for(int m=0;m<main_width/2+5;m++)
			cout<<" ";
		cout<<"1.存储新闻"<<endl<<endl;
		for(int m=0;m<main_width/2+5;m++)
			cout<<" ";
		cout<<"2.篡改检查"<<endl<<endl;
		for(int m=0;m<main_width/2+4;m++)
			cout<<" ";
		cout<<"3.关键字查找"<<endl<<endl;
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
	cout<<"请输入日期:";
	cin>>data;
	cout<<"请输入文章:"<<endl;
	passage = read_passage();
	block_chain.addNode(passage,data);
	cout<<"存储完成!"<<endl;
}
string UI::read_passage()
{
	getchar();//过滤换行符
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
	passage.pop_back();//去掉最后的回车
	return passage;
}
void UI::find_difference()
{
	system("cls");
	string passage,time;
	cout<<"请输入日期:"<<endl;
	cin>>time;
	cout<<"请输入文章:"<<endl;
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