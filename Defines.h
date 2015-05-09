#ifndef DEFINES_H
#define DEFINES_H
#define _CRT_SECURE_NO_WARNINGS
#include "BigInt.h"

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <cstdlib>
#include <ctime>
#include <cstdio>
using namespace std;

struct SExp{
	static enum SEXP_TYPE{
		VAR, FUNC, COMMAND, IF, WHILE,STR
	};
	SEXP_TYPE type;
	string name;
	SExp *parent;
	int id;
	Vector<SExp*> elems;	//��֪��Ϊʲô���Լ�д��Vector���ܲ鿴��ϸԪ�ظ�ֵ
	SExp();
	void Copy(SExp *);
};

void DelSExp(SExp *);

#endif