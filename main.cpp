

#include "RE2NFA_Converter.h"
#include <windows.system.h>

/*
*����Ƿ�����ĸ
*�Ƿ���true,����false
*/
bool is_letter(char check)
{
	if (check >= 'a' && check <= 'z' || check >= 'A' && check <= 'Z')
		return true;
	return false;
}
/**��������������ʽ�Ƿ�Ϸ�
*/
int check_character(string check_string)
{
	int length = check_string.size();
	for (int i = 0; i < length; i++)
	{
		char check = check_string.at(i);
		if (is_letter(check))//Сд�ʹ�д֮����5���ַ����ʲ��������ж�
		{
			//cout<<"��ĸ �Ϸ�";
		}
		else if (check == '(' || check == ')' || check == '*' || check == '|')
		{
			//cout<<"������ �Ϸ�";
		}
		else
		{
			cout << "���в��Ϸ����ַ�!" << endl;
			cout << "����������:" << endl;
			return false;
		}
	}
	return true;
}
/*
*�ȼ�������Ƿ�ƥ��
*�Ϸ�����true,�Ƿ�����false
*/
int check_parenthesis(string check_string)
{
	int length = check_string.size();

	string check = check_string;
	stack<int> STACK;
	for (int i = 0; i < length; i++)
	{
		if (check[i] == '(')
			STACK.push(i);
		else if (check[i] == ')')
		{
			if (STACK.empty())
			{
				cerr << "�ж����������" << endl;//��ʱ����¼ƥ��λ��location
				cout << "����������:" << endl;
				return false;
			}
			else
				STACK.pop();
		}
	}
	if (!STACK.empty())
	{
		//��ʱ����¼ƥ��λ��location
		cerr << "�ж����������" << endl;
		cout << "����������:" << endl;
		return false;
	}

	return true;
}
int check_legal(string check_string)
{
	if (check_character(check_string) && check_parenthesis(check_string))
	{
		return true;
	}
	return false;
}


//��Ӳ�������+����
/**********************************************
      a    *     (		)	 |
a     +    ��    +     ��    ��
*     +    ��    +     ��    ��
(     ��   �Ƿ�  ��    ��    �Ƿ�
)     +    ��    +     ��    �Ƿ�
|     ��   �Ƿ�  �Ƿ�  �Ƿ�  �Ƿ�
***********************************************
*/
string add_join_symbol(string add_string)
{
	int length = add_string.size();
	int return_string_length = 0;
	char* return_string = new char[2 * length + 2];//���������

	char first, second=0;
	for (int i = 0; i < length - 1; i++)
	{
		first = add_string.at(i);
		second = add_string.at(i + 1);

		//�Ƚ��ַ��ӽ�ȥ
		return_string[return_string_length++] = first;
		
		if (is_letter(first) && (is_letter(second) || second == '('))
		{
			return_string[return_string_length++] = '+';
		}
		else if (first == '*' && (is_letter(second) || second == '('))
		{
			return_string[return_string_length++] = '+';
		}
		else if (first == ')' && (is_letter(second) || second == '('))
		{
			return_string[return_string_length++] = '+';
		}
	}
	//�����һ���ַ�д��
	return_string[return_string_length++] = second;
	return_string[return_string_length] = '\0';
	string STRING(return_string);
	//cout << "��'+'��ı��ʽ��" << STRING << endl;
	return STRING;
}

int main()
{
	string expr;
	//cout << "����һ������ʽ��";
	//cin >> expr;
	
	fstream in;
	in.open("inputString.txt");
	if (in.fail())
	{
		cout << "inputString.txt�ļ���ʧ��" << endl;
		exit(0);
	}
	getline(in,expr);
	in.close();

	if (check_legal(expr) == false)
	{
		cout << "���벻�Ϸ�����������"<<endl;
		return 0;
	}
	string expr2=add_join_symbol(expr);

	RE2NFA_Converter converter;
	converter.setRE(expr2);
    converter.RE2NFA();
	cout << "��������ˣ��������" << endl;
	converter.outPutResult();

	//ִ�����������н�Result.txtת����ͼƬ�������ڵ�ǰ��Ŀ��Ŀ¼��
	system("dot -Tpng -o Result.png Result.txt");
	return 0;
}