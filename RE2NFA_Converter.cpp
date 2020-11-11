
#include "RE2NFA_Converter.h"

//判断字符是操作符还是操作数(字母)
int RE2NFA_Converter::getType(char ch)
{
	if (ch == '*' || ch == '|')
	{
		return OP;
	}
	else if (ch != '(' && ch != ')')
		return OP_D;
	else
		return -1;
}


//设置RE
void RE2NFA_Converter::setRE(string inputString)
{
	this->re.inputString = inputString;
	//初始化计数状态
	this->state = 1;

	for (int i = 0; i < inputString.size(); i++)
	{
		//如果为操作数(字符)，更新输入字符集
		if (getType(inputString[i]) == OP_D)
		{
			//字符集中不存在这个字符
			if (find(re.charSet.begin(), re.charSet.end(), inputString[i]) == re.charSet.end())
			{
				re.charSet.push_back(inputString[i]);
			}
		}
	}
};
void RE2NFA_Converter::createSingleNFA(char syboml, singleNFA* nfa1, vector<EDGE>& edge)
{
	EDGE e;
	if (nfa1->start == 0 && nfa1->end == 0)
	{
		nfa1->start = this->state;
		nfa1->end = ++state;
	}
	//给nfa的两个初始和终结结点加上边
	e.start = nfa1->start;
	e.end = nfa1->end;
	e.symbol = syboml;


	//将这条边加入到最后的NFA的边集中
	this->nfa.edgeSet.push_back(e);
}

void RE2NFA_Converter::mergeNFA_OR(singleNFA* mergeNFA, singleNFA* NFA1, singleNFA* NFA2, vector<EDGE>edgeSet)
{
	singleNFA temp;
	temp.start = state;
	temp.end = NFA1->start;
	createSingleNFA('#', &temp, edgeSet);
	mergeNFA->start = temp.start;

	temp.start = state;
	temp.end = NFA2->start;
	createSingleNFA('#', &temp, edgeSet);

	++state;
	temp.start = NFA1->end;
	temp.end = state;
	createSingleNFA('#', &temp, edgeSet);

	temp.start = NFA2->end;
	temp.end = state;
	createSingleNFA('#', &temp, edgeSet);

	mergeNFA->end = temp.end;

	++state;

}

void RE2NFA_Converter::mergeNFA_AND(singleNFA* mergeNFA, singleNFA* NFA1, singleNFA* NFA2, vector<EDGE>edgeSet)
{
	mergeNFA->start = NFA1->start;
	mergeNFA->end = NFA2->end;
	for (unsigned int i = 0; i < edgeSet.size(); i++)
	{
		//将两个结点合并成一个
		if (edgeSet[i].start == NFA2->start)
		{
			edgeSet[i].start = NFA1->end;
		}
	}
}

void RE2NFA_Converter::mergeNFA_Cycle(singleNFA* mergeNFA, singleNFA* NFA1, vector<EDGE>edgeSet)
{
	singleNFA temp;
	mergeNFA->start = NFA1->start;
	temp.start = NFA1->end;
	temp.end = NFA1->start;
	createSingleNFA('#', &temp, edgeSet);

	temp.start = NFA1->end;
	temp.end = state;
	createSingleNFA('#', &temp, edgeSet);


	temp.start = mergeNFA->start;
	temp.end = mergeNFA->end;
	createSingleNFA('#', &temp, edgeSet);
}

void RE2NFA_Converter::outPutResult()
{
	//fstream out("Result.txt");
	cout << "=================Start==================" << endl;	//输入流 
	cout << endl;

	cout << "正规式：" << this->re.inputString << endl;					//输入流，输入正规式 
	cout << "正规式输入符：";
	for (unsigned int i = 0; i < this->re.charSet.size(); i++)				//输入正规式输入符 
		cout << this->re.charSet[i] << " ";
	cout << endl;
	cout << "==================NFA===================" << endl;		//同上， 输入初态集、终态集、NFA边集 
	cout << "初态集：";
	for (unsigned int i = 0; i < this->nfa.startStateSet.size(); i++)
		cout << this->nfa.startStateSet[i] << " ";
	cout << endl;
	cout << "终态集：";
	for (unsigned int i = 0; i < this->nfa.endStateSet.size(); i++)
		cout << this->nfa.endStateSet[i] << " ";
	cout << endl;

	cout << "NFA的边集：" << endl;
	for (unsigned int i = 0; i < this->nfa.edgeSet.size(); i++)
	{
		cout << "Index " << i << ": ";
		cout << this->nfa.edgeSet[i].start << "  "
			<< this->nfa.edgeSet[i].symbol << "  "
			<< this->nfa.edgeSet[i].end << endl;
	}
	cout << endl;
}
void RE2NFA_Converter::RE2NFA() {

	//操作符堆栈
	stack<char> OPStack;
	stack<singleNFA> singleNFAStack;
	singleNFA stackTop1NFA, stackTop2NFA;
	singleNFA currentNFA, mergeNFA;
	//
	//stack<>


	//遍历输入字符串
	for (unsigned int i = 0; i < re.inputString.size(); i++)
	{
		if (re.inputString[i] == '(')
		{
			OPStack.push(re.inputString[i]);
		}
		//如果是操作数(字符)
		else if (getType(re.inputString[i]) == OP_D)
		{
			currentNFA.start = 0;
			currentNFA.end = 0;
			createSingleNFA(re.inputString[i], &currentNFA, this->nfa.edgeSet);
			//将当前的单个nfa加入到栈中
			singleNFAStack.push(currentNFA);
			//如果栈中存在两个以上的单个nfa，就进行合并
			if (singleNFAStack.size() >= 2)
			{
				//如果栈顶是|,就进行两个nfa的|的合并操作
				if (OPStack.size() > 0 && OPStack.top() == '|')
				{
					OPStack.pop();

					stackTop1NFA = singleNFAStack.top();
					singleNFAStack.pop();

					stackTop2NFA = singleNFAStack.top();
					singleNFAStack.pop();

					mergeNFA_OR(&mergeNFA, &stackTop1NFA, &stackTop2NFA, this->nfa.edgeSet);
					singleNFAStack.push(mergeNFA);
				}
				//堆栈中是左括号，则当前的两个字符应该连接起来，进行and操作
				else if (OPStack.size()>0 && OPStack.top() == '(')
				{
					stackTop1NFA = singleNFAStack.top();
					singleNFAStack.pop();
					stackTop2NFA = singleNFAStack.top();
					singleNFAStack.pop();

					mergeNFA_AND(&mergeNFA, &stackTop1NFA, &stackTop2NFA, this->nfa.edgeSet);

					singleNFAStack.push(mergeNFA);
				}
			}
		}
		//如果是操作符
		else if (getType(re.inputString[i]) == OP)
		{
			//闭包,取出一个NFA构造闭包
			if (re.inputString[i] == '*')
			{
				stackTop1NFA = singleNFAStack.top();
				singleNFAStack.pop();
				mergeNFA_Cycle(&mergeNFA, &stackTop1NFA, this->nfa.edgeSet);
				singleNFAStack.push(mergeNFA);

			}
			else if (re.inputString[i] == '|')
			{
				OPStack.push(re.inputString[i]);
			}
		}
		else if (re.inputString[i] == ')')
		{
			OPStack.pop();
		}
	}

	//全部合并结束后,最后入栈的那个就是最后的NFA的状态。
	if (singleNFAStack.size() > 0)
	{
		this->nfa.startStateSet.push_back(singleNFAStack.top().start);
		this->nfa.endStateSet.push_back(singleNFAStack.top().end);
	}
}