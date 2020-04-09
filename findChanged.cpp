void hashTree::findChanged()
{
	queue<hashNode*>exampleQueue;
	if (isLeaf(root))
		return NULL;
	exampleQueue.push(example);
	while (exampleQueue.size())//直到队列为空
	{
		hashNode* tempExam = exampleQueue.front();
		exampleQueue.pop();
		if (tempExam->hashCode != md5.encrypt(tempExam))//重新生成节点的hash值与原先的hash值作比较
		{
			if (!isLeaf(tempChanged))
			{
				if (tempExam->left)
					exampleQueue.push(tempExam->left);
				if (tempExam->right)
					exampleQueue.push(tempExam->right);
			}
			else
			{
				changedParagraph->push(tempExam->paragraph);
			}
		}
	}
}
//区块链查询函数
void chain::findChanged()
{
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
