
#include "RE2NFA_Converter.h"

//�ж��ַ��ǲ��������ǲ�����(��ĸ)
int RE2NFA_Converter::getType(char ch)
{
	if (ch == '*' || ch == '|' || ch=='+')
	{
		return OP;
	}
	else if (ch != '(' && ch != ')')
		return OP_D;
	else
		return -1;
}


//����RE
void RE2NFA_Converter::setRE(string inputString)
{
	this->re.inputString = inputString;
	//��ʼ������״̬
	this->state = 1;

	for (int i = 0; i < inputString.size(); i++)
	{
		//���Ϊ������(�ַ�)�����������ַ���
		if (getType(inputString[i]) == OP_D)
		{
			//�ַ����в���������ַ�
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

		state++;
	}
	//��nfa��������ʼ���ս�����ϱ�
	e.start = nfa1->start;
	e.end = nfa1->end;
	e.symbol = syboml;


	//�������߼��뵽����NFA�ı߼���
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
	mergeNFA->start = NFA2->start;
	mergeNFA->end = NFA1->end;

	singleNFA temp;
	temp.start = NFA2->end;
	temp.end = NFA1->start;
	createSingleNFA('#', &temp, nfa.edgeSet);
	
	//state++;

	
	for (unsigned int i = 0; i < edgeSet.size(); i++)
	{
		//���������ϲ���һ��
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
	mergeNFA->end = state;

	temp.start = mergeNFA->start;
	temp.end = mergeNFA->end;
	createSingleNFA('#', &temp, edgeSet);

	state++;
}

void RE2NFA_Converter::outPutResult()
{
	cout << "��������ˣ�������£�����" << endl;
	cout << "=================Start==================" << endl;	//������ 
	cout << endl;

	cout << "����ʽ��" << this->re.inputString << endl;					//����������������ʽ 
	cout << "����ʽ�������";
	for (unsigned int i = 0; i < this->re.charSet.size(); i++)				//��������ʽ����� 
		cout << this->re.charSet[i] << " ";

	cout << endl;
	cout << "==================NFA===================" << endl;		//ͬ�ϣ� �����̬������̬����NFA�߼� 
	cout << "��̬����" << nfa.startState << endl;
	cout << "��̬����" << nfa.endState << endl;

	cout << "NFA�ı߼���" << endl;
	for (unsigned int i = 0; i < this->nfa.edgeSet.size(); i++)
	{
		cout << "Index " << i << ": ";
		cout << this->nfa.edgeSet[i].start << "  "
			 << this->nfa.edgeSet[i].symbol << "  "
			 << this->nfa.edgeSet[i].end << endl;
	}

	//����������Result.txt�ļ�
	fstream out;
	out.open("Result.txt",ios::out);
	if (out.fail())
	{
		cout << "Result.txt�ļ���ʧ��" << endl;
		exit(0);
	}
	out << "digraph nfa{" << endl;
	out << "rankdir=LR;";
	for (unsigned int i = 0; i < this->nfa.edgeSet.size(); i++)
	{
		out << nfa.edgeSet[i].start << "->" << nfa.edgeSet[i].end << "[label=\"" << nfa.edgeSet[i].symbol << "\"];" << endl;
	}
	out << nfa.startState << "[color=green];" << endl;
	out << nfa.endState << "[color=red];" << endl;
	out << "}";

	out.close();
}
void RE2NFA_Converter::RE2NFA() {

	//��������ջ
	stack<char> OPStack;
	stack<singleNFA> singleNFAStack;
	singleNFA stackTop1NFA, stackTop2NFA;
	singleNFA currentNFA, mergeNFA;


	//���������ַ���
	for (unsigned int i = 0; i < re.inputString.size(); i++)
	{
		if (re.inputString[i] == '(')
		{
			OPStack.push(re.inputString[i]);
		}
		//����ǲ�����(�ַ�)
		else if (getType(re.inputString[i]) == OP_D)
		{
			currentNFA.start = 0;
			currentNFA.end = 0;
			createSingleNFA(re.inputString[i], &currentNFA, this->nfa.edgeSet);
			//����ǰ�ĵ���nfa���뵽ջ��
			singleNFAStack.push(currentNFA);

			//���ջ�д����������ϵĵ���nfa���ж��Ƿ���Խ��кϲ�
			if (singleNFAStack.size() >= 2)
			{
				//���ջ����|,�ͽ�������nfa��|�ĺϲ�����
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
				//��ջ���������ţ���ǰ�������ַ�Ӧ����������������and����
				else if (OPStack.size()>0 && OPStack.top() == '+' )//
				{
					OPStack.pop();//����

					stackTop1NFA = singleNFAStack.top();
					singleNFAStack.pop();
					stackTop2NFA = singleNFAStack.top();
					singleNFAStack.pop();

					mergeNFA_AND(&mergeNFA, &stackTop1NFA, &stackTop2NFA, this->nfa.edgeSet);

					singleNFAStack.push(mergeNFA);
				}
			}
		}
		//����ǲ�����
		else if (getType(re.inputString[i]) == OP)
		{
			//�հ�,ȡ��һ��NFA����հ�
			if (re.inputString[i] == '*')
			{
				stackTop1NFA = singleNFAStack.top();
				singleNFAStack.pop();
				mergeNFA_Cycle(&mergeNFA, &stackTop1NFA, this->nfa.edgeSet);
				singleNFAStack.push(mergeNFA);

			}
			//��Ҫ��һ���ſ��Խ��в���������ջ
			else if (re.inputString[i] == '|' || re.inputString[i]=='+')
			{
				OPStack.push(re.inputString[i]);
			}
		}
		else if (re.inputString[i] == ')')
		{
			OPStack.pop();
		}
	}

	//��ʣ�µ�NFA���кϲ�
	while (OPStack.size() != 0)
	{
		if (OPStack.top() == '+' && singleNFAStack.size() >= 2)
		{
			OPStack.pop();
			stackTop1NFA = singleNFAStack.top();
			singleNFAStack.pop();
			stackTop2NFA = singleNFAStack.top();
			singleNFAStack.pop();

			mergeNFA_AND(&mergeNFA, &stackTop1NFA, &stackTop2NFA, this->nfa.edgeSet);

			singleNFAStack.push(mergeNFA);
		}
	}
	if (singleNFAStack.size() > 0)
	{
		this->nfa.startState = singleNFAStack.top().start;
		this->nfa.endState = singleNFAStack.top().end;
	}
}