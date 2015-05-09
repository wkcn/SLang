#include "Defines.h"

SExp::SExp(){
	parent = 0;
}

void SExp::Copy(SExp *e){
	if (!e)return;
	//��Ҫ����parent��id��elems
	(*this) = *e;//ǳ����
	parent = 0;
	for (int i = 0; i < e->elems.size(); ++i){
		elems[i] = new SExp();
		elems[i]->Copy(e->elems[i]);
		elems[i]->parent = this;
		elems[i]->id = i;
	}
}

void DelSExp(SExp *e){
	if (e == 0)return;
	for (int i = 0; i < e->elems.size(); ++i){
		DelSExp(e->elems[i]);
	}
	delete e;
}