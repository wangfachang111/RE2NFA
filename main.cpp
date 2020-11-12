// RE2NFA_Converter.h: interface for the REManage class.
#include "RE2NFA_Converter.h"


/**检测是否是字母
是返回true,否则false
*/
bool is_letter(char check)
{
	if (check >= 'a' && check <= 'z' || check >= 'A' && check <= 'Z')
		return true;
	return false;
}
/**检测输入的正则表达式是否合法
*/
int check_character(string check_string)
{
	int length = check_string.size();
	for (int i = 0; i < length; i++)
	{
		char check = check_string.at(i);
		if (is_letter(check))//小写和大写之间有5个字符，故不能连续判断
		{
			//cout<<"字母 合法";
		}
		else if (check == '(' || check == ')' || check == '*' || check == '|')
		{
			//cout<<"操作符 合法";
		}
		else
		{
			cout << "含有不合法的字符!" << endl;
			cout << "请重新输入:" << endl;
			return false;
		}
	}
	return true;
}
/**先检查括号是否匹配
*合法返回true,非法返回false
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
				cerr << "有多余的右括号" << endl;//暂时不记录匹配位置location
				cout << "请重新输入:" << endl;
				return false;
			}
			else
				STACK.pop();
		}
	}
	if (!STACK.empty())
	{
		//暂时不记录匹配位置location
		cerr << "有多余的左括号" << endl;
		cout << "请重新输入:" << endl;
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







/**添加交操作符“+”，
例如 abb->a+b+b
*/
string add_join_symbol(string add_string)
{
	int length = add_string.size();
	int return_string_length = 0;
	char* return_string = new char[2 * length + 2];//最多是两倍

	char first, second=0;
	for (int i = 0; i < length - 1; i++)
	{
		first = add_string.at(i);
		second = add_string.at(i + 1);

		//先将字符加进去
		return_string[return_string_length++] = first;
		//要加的可能性如ab 、 *b 、 a( 、 )b 等情况
		//若第二个是字母、第一个不是'('、'|'都要添加
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
	//将最后一个字符写入
	return_string[return_string_length++] = second;
	return_string[return_string_length] = '\0';
	string STRING(return_string);
	cout << "加'+'后的表达式：" << STRING << endl;
	return STRING;
}

int main()
{
	string expr;
	//cout << "输入一个正规式：";
	//cin >> expr;
	
	fstream in;
	in.open("inputString.txt");
	if (in.fail())
	{
		cout << "inputString.txt文件打开失败" << endl;
		exit(0);
	}
	getline(in,expr);
	in.close();

	string expr2=add_join_symbol(expr);
	cout << "添加+号之后的表达式："<<expr2 << endl;


	RE2NFA_Converter converter;
	converter.setRE(expr2);

    converter.RE2NFA();
	cout << "处理完成了，结果如下" << endl;
	converter.outPutResult();
	return 0;

}