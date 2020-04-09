void chain::findChanged(string passage, string time)
{
	node* temp;
	for (temp = head; temp; temp = temp->next)
	{
		if (temp->timeStamp == time)
			break;
	}
	hashTree position;
	position.createTree(passage);
	queue<int>* changeParagraph = temp->htree.findChanged(position.root, temp->htree.root);
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
queue<int> hashTree::findChanged(hashNode* example, hashNode* mayChanged)
{
	queue<int>* changedParagraph = new queue<int>;
	queue<hashNode*>exampleQueue, changedQueue;
	if (!example)
		return NULL;
	if (example->hashCode == mayChanged->hashCode)
		return NULL;
	exampleQueue.push(example);
	changedQueue.push(mayChanged);
	while (exampleQueue.size())//直到队列为空
	{
		hashNode* tempExam = exampleQueue.front();
		exampleQueue.pop();
		hashNode* tempChanged = changedQueue.front();
		changedQueue.pop();
		if (tempExam->hashCode != tempChanged->hashCode)//重新生成节点的hash值与原先的hash值作比较
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