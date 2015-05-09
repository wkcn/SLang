#ifndef BIGINT_H
#define BIGINT_H

#include "UBigInt.h"
//��֪���Ƿ����ü̳�UBigInt�ķ�ʽʵ�֣�
class BigInt{
protected:
	UBigInt num;
	bool minus;	//����
public:
	//����
	BigInt();
	BigInt(int);
	BigInt(const char *);

	//�����(����ʹ����Ԫ�����ʺ�ǿ��ת����
	//һԪ����
	BigInt operator-();

	BigInt& operator++();//++a
	BigInt operator++(int dummy);//a++
	BigInt& operator--();//--a
	BigInt operator--(int dummy);//a--


	friend BigInt& operator+=(BigInt&, const BigInt&);
	friend BigInt& operator-=(BigInt&, const BigInt&);
	friend BigInt& operator*=(BigInt&, const BigInt&);
	friend BigInt& operator/=(BigInt&, const BigInt&);
	friend BigInt& operator%=(BigInt&, const BigInt&);

	friend BigInt& BigIntDivide(BigInt &a, const BigInt &b, bool mod);//������ȡģ��

	//��Ԫ����
	friend BigInt operator+(const BigInt&, const BigInt &);
	friend BigInt operator-(const BigInt&, const BigInt &);
	friend BigInt operator*(const BigInt&, const BigInt &);
	friend BigInt operator/(const BigInt&, const BigInt &);
	friend BigInt operator%(const BigInt&, const BigInt &);

	//�Ƚ�,ʹ����Ԫ��ʵ��int < UBigInt
	friend int BigIntCmp(const BigInt&, const BigInt&);	//�Ƚ��ã����ظ���ʱa<b;	0ʱa=b;	����ʱa>b
	friend bool operator==(const BigInt&, const BigInt&);
	friend bool operator!=(const BigInt&, const BigInt&);
	friend bool operator<(const BigInt&, const BigInt&);
	friend bool operator<=(const BigInt&, const BigInt&);
	friend bool operator>(const BigInt&, const BigInt&);
	friend bool operator>=(const BigInt&, const BigInt&);
	bool isZero();//�ж��Ƿ�Ϊ0�����ڿ����ж�
	int GetInt();	//��ȡintֵ,��ȡģ1 0000 0000
public:
	friend ostream& operator<<(ostream &, BigInt &);
	friend istream& operator>>(istream &, BigInt &);
};

#endif