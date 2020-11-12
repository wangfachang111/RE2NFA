
#pragma once
#ifndef RE2NFA_
#define RE2NFA_
#include <string>
#include <stack>
#include <vector>
#include <queue>
#include <list>
#include <fstream>
#include <iostream>
using namespace std;


#define OP 0
#define OP_D 1
//边
typedef struct _EDGE {
public:
	int start;
	char symbol;
	int end;
	friend bool operator == (const _EDGE& a, const _EDGE& b)
	{
		return (a.start == b.start) && (a.symbol == b.symbol) && (a.end == b.end);
	}
}EDGE;

//正规式
typedef struct _RE
{
	//输入的正规式
	string inputString;
	//正规式的字符集
	vector<char> charSet;
}RE;

typedef struct _singleNFA
{
	int start;
	int end;
	/*void operator=(const _singleNFA& b)
	{
		this->start = b.start;
		this->end = b.end;
	}*/
}singleNFA;
//NFA
typedef struct _NFA {
	
	//起始状态集
	//vector<char> startStateSet;
	int startState;
	int endState;
	//终结状态集
	//vector<char> endStateSet;
	//边集
	vector<EDGE> edgeSet;
}NFA;
class RE2NFA_Converter{

private:
	//计数状态
	int state;
	//正规式
	RE re;
	//NFA
	NFA nfa;
public:
	void setRE(string inputString);
	//判断一个符号的类型
	int getType(char ch);
	void RE2NFA();

	void createSingleNFA(char syboml,singleNFA *nfa,vector<EDGE>& edge);
	void mergeNFA_OR(singleNFA* mergeNFA, singleNFA* stackTop1NFA, singleNFA* stackTop2NFA, vector<EDGE>edgeSet);
	void mergeNFA_AND(singleNFA* mergeNFA, singleNFA* stackTop1NFA, singleNFA* stackTop2NFA, vector<EDGE>edgeSet);
	void mergeNFA_Cycle(singleNFA* mergeNFA, singleNFA* stackTop1NFA, vector<EDGE>edgeSet);
	void outPutResult();
};
#endif