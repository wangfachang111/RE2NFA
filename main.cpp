// RE2NFA_Converter.h: interface for the REManage class.
#include "RE2NFA_Converter.h"


int main()
{
	string expr;
	cout << "����һ������ʽ��";
	cin >> expr;
	RE2NFA_Converter converter;
	converter.setRE(expr);


	converter.RE2NFA();
	cout << "��������ˣ��������" << endl;
	converter.outPutResult();
	return 0;

}