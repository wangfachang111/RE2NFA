// RE2NFA_Converter.h: interface for the REManage class.
#include "RE2NFA_Converter.h"


/**����Ƿ�����ĸ
�Ƿ���true,����false
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
/**�ȼ�������Ƿ�ƥ��
*�Ϸ�����true,�Ƿ�����false
*/
int check_parenthesis(string check_string)
{
	int length = check_string.size();
	//char * check = new char[length+1];
	//wcscpy(check, length+1, check_string.c_str());
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







/**��ӽ���������+����
���� abb->a+b+b
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
		//Ҫ�ӵĿ�������ab �� *b �� a( �� )b �����
		//���ڶ�������ĸ����һ������'('��'|'��Ҫ���
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
	cout << "��'+'��ı��ʽ��" << STRING << endl;
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

	string expr2=add_join_symbol(expr);
	cout << "���+��֮��ı��ʽ��"<<expr2 << endl;


	RE2NFA_Converter converter;
	converter.setRE(expr2);

    converter.RE2NFA();
	cout << "��������ˣ��������" << endl;
	converter.outPutResult();
	return 0;

}