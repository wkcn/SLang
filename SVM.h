#ifndef SVM_H
#define SVM_H

//�������֧�������������������~
//��ʱ������������Ϊ����Ҫ�ж�������

#include "Defines.h"
#include "SPar.h"

typedef BigInt (*SFunc)(SPar);	//�ò���ʵ��Ϊ���������ܻ�Ӱ��Ч��

class SVM{
private:
	map<string, BigInt> vars;	//��������Ҿ����ڲ�Ҳ��ͨ��new�½�Ԫ�صģ����ԾͲ���ָ����
	map<string, Vector<BigInt>> arrays;	//������	//����������hash
	map<string, SFunc>funcs;	//C++����
	map<string, SExp*>sfuncs;	//Slang����
	Vector<string> path;		//����Ŀ¼
	SVM *parent;
private:
	BigInt& GetVar(const string &);
	SExp* SearchFile(const string &filename,SExp *exp = 0,bool *ismacro = 0);
public:
	SVM();
	~SVM();
	void SetParent(SVM *);

	bool debug;
	friend class SPar;
	void AddFunc(const string &name, SFunc f);
	void AddPath(const string &name);

	void SetVar(const string &name,const BigInt &value);
	void ClearVar(const string &name);	//��ĳ����������
	void SetArray(const string &name, int id, const BigInt &value);
	BigInt GetArray(const string &name,int id);
	int GetArraySize(const string &name);
	void ClearArray(const string &name);
	Vector<BigInt>& FindArray(const string &name);

	void Restart();	//���������
	BigInt GetValue(SExp *);	//��S���ʽ��ֵ,����ʹ�ÿ�����д����������Щ����Ȼ���õķ�ʽ��д����Ĵ�������
	BigInt Eval(SExp *);	//GetValue�ı���
};

#endif