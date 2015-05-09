#ifndef SPAR_H
#define SPAR_H

#include "Defines.h"
//#include "SVM.h"

class SVM;

//�����࣬����C++��Slang�Ľ���^-^
class SPar{
public:
	SVM *vm;
	SExp *exp;	//��ǰ���ʽ
public:
	SPar(SVM *, SExp *);
	int size();
	BigInt& operator[](int i);	//���ض�ӦBigInt����(������ʵ����),��һ�������±�Ϊ0������~���������±�0���������С����
	string GetStr(int i);
	BigInt GetNum(int i);
	SExp::SEXP_TYPE GetType(int i);
};


#endif