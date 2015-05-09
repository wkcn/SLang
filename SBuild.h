#ifndef SBUILD_H
#define SBUILD_H

#include "Defines.h"

//��������
class streamx{
private:
	queue<string> buf;
	unsigned int pointer;
	bool eofed;
	void Update();
public:
	streamx();
	char get();
	char peek();
	bool eof();
	void clear();
	streamx& operator<<(const string&);
};

//Sugar�����﷨�Ǻúó�
//�﷨��#(3+1)*5	-> (* (+ 3 1) 5)
//		!help\n		-> (help)

class SSugar{
private:
	streamx *is;	//���������
	queue<char> buf;
	bool updated;
	map<string,int> level;
	char nowchar;
	string word;
private:
	bool isOP(char c);
	bool isBlank(char c);
	string NewSExp(string name, string left, string right);
	char GetC(bool &can,bool del = true);
	void PushS(stack<string> &res, stack<string> &op);
	inline int ISP(string &op);	//ջ�����ȼ�
	inline int ICP(string &op); //�������ȼ�
	void Update();
public:
	SSugar();
	string Formula2Sexp(string);	//��׺���ʽתS���ʽ
	string peek();
	string next();
	bool eof();
	void SetStream(streamx *is);
};

//S���ʽ����
class SBuild{
private:
	SSugar sugar;	//������Reader�ģ�������Sugar��������
	SExp* macro;
	bool* ismacro;
private:
	void MarkParent(SExp *);
public:
	SBuild();
	void SetStream(streamx &is);
	void SetMacro(SExp *,bool *ismacro);
	SExp* Build(bool first = true);
};

#endif