#ifndef UBIGINT_H
#define UBIGINT_H

#include "Vector.h"

#include <iostream>
#include <iomanip>

using namespace std;

/*
	�޷��Ŵ�������
	����ʹ��10000����
	�и����⣬������ʹ��UBigInt a = 0,ԭ���Ǳ������жϲ���0����unsigned int����const char * 
*/

class UBigInt{
protected:
	Vector<int> datas;	//��10000���Ƽ�¼����
	int _len;	//�����������datas��Ԫ����,�Ƿ������datas.size()������һ����ֵ�ÿ���
public:
	//����
	UBigInt();
	UBigInt(unsigned int);
	//UBigInt(const UBigInt&);	//��Ϊ�ڲ�������ʵ�����
	UBigInt(const char *);
	//UBigInt(const string&);	//��������̫��������

	//�����(����ʹ����Ԫ�����ʺ�ǿ��ת����
	//һԪ����
	
	UBigInt& operator++();//++a
	UBigInt operator++(int dummy);//a++
	UBigInt& operator--();//--a
	UBigInt operator--(int dummy);//a--

	friend UBigInt& operator+=(UBigInt&,const UBigInt&);
	friend UBigInt& operator-=(UBigInt&, const UBigInt&);
	friend UBigInt& operator*=(UBigInt&, const UBigInt&);
	friend UBigInt& operator/=(UBigInt&, const UBigInt&);
	friend UBigInt& operator%=(UBigInt&, const UBigInt&);

	friend UBigInt& UBigIntDivide(UBigInt &a, const UBigInt &b, bool mod);//������ȡģ��

	//��Ԫ����
	friend UBigInt operator+(const UBigInt&, const UBigInt &);
	friend UBigInt operator-(const UBigInt&, const UBigInt &);
	friend UBigInt operator*(const UBigInt&, const UBigInt &);
	friend UBigInt operator/(const UBigInt&, const UBigInt &);
	friend UBigInt operator%(const UBigInt&, const UBigInt &);

	//�Ƚ�,ʹ����Ԫ��ʵ��int < UBigInt
	friend int UBigIntCmp(const UBigInt&,const UBigInt&);	//�Ƚ��ã����ظ���ʱa<b;	0ʱa=b;	����ʱa>b
	friend bool operator==(const UBigInt&,const UBigInt&);
	friend bool operator!=(const UBigInt&,const UBigInt&);
	friend bool operator<(const UBigInt&, const UBigInt&);
	friend bool operator<=(const UBigInt&, const UBigInt&);
	friend bool operator>(const UBigInt&, const UBigInt&);
	friend bool operator>=(const UBigInt&, const UBigInt&);
	bool isZero();//�ж��Ƿ�Ϊ0�����ڿ����ж�
	int GetInt();	//ȡģ1 0000 0000
public:
	friend ostream& operator<<(ostream &,UBigInt &);
	friend istream& operator>>(istream &, UBigInt &);
};

#endif