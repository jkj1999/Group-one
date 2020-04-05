/*
注释部分是改动前星棋大佬写的
根据之前hashTree的结构编写，改动思路为：
重新生成节点的hash值，与原节点已经生成好的hash值做对比，
不同的话则是经过篡改后的段落，应该能够比原先提升一部分效率
*/
//hash树中的查询函数
/*queue<int>**/void hashTree::findChanged(/*hashNode* example, hashNode* mayChanged*/)
{
	//queue<int>* changedParagraph = new queue<int>;
	queue<hashNode*>exampleQueue;//, changedQueue;
	if (/*!example*/isLeaf(root))
		return NULL;
	//if (example->hashCode == mayChanged->hashCode)
	//	return NULL;//没有篡改的段落
	exampleQueue.push(example);
	//changedQueue.push(mayChanged);
	while (exampleQueue.size())//直到队列为空
	{
		hashNode* tempExam = exampleQueue.front();
		exampleQueue.pop();
		//hashNode* tempChanged = changedQueue.front();
		//changedQueue.pop();
		if (tempExam->hashCode != md5.encrypt(tempExam))//重新生成节点的hash值与原先的hash值作比较
		{
			if (!isLeaf(tempChanged))
			{
				if (tempExam->left)
					exampleQueue.push(tempExam->left);
				if (tempExam->right)
					exampleQueue.push(tempExam->right);
				//if (tempChanged->left)
				//	changedQueue.push(tempChanged->left);
				//if (tempChanged->right)
				//	changedQueue.push(tempChanged->right);
			}
			else
			{
				changedParagraph->push(/*tempChanged*/tempExam->paragraph);
			}
		}
	}
	//return changedParagraph;
}
//区块链查询函数
void chain::findChanged(/*string passage, string time*/)
{
//	node* temp;
//	for (temp = head; temp; temp = temp->next)
//	{
//		if (temp->timeStamp == time)
//			break;
//	}
//	hashTree position;
//	position.createTree(passage);
//	queue<int>* changeParagraph = temp->htree.findChanged(position.root, temp->htree.root);
	node* temp = head;
	while (temp->next != NULL)//遍历链表并执行findChanged()检索函数
	{
		temp->htree.findChanged();
	}
	if (!changeParagraph)
		cout << "完全一致" << endl;
	else
	{
		while (changeParagraph->size())
		{
			cout << changeParagraph->front() + 1 << " ";
			changeParagraph->pop();
		}
		cout << "发生篡改" << endl;
	}
}