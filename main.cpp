// RE2NFA_Converter.h: interface for the REManage class.
#include "RE2NFA_Converter.h"


int main()
{
	string expr;
	cout << "输入一个正规式：";
	cin >> expr;
	RE2NFA_Converter converter;
	converter.setRE(expr);


	converter.RE2NFA();
	cout << "处理完成了，结果如下" << endl;
	converter.outPutResult();
	return 0;

}