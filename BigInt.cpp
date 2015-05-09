#include "BigInt.h"

BigInt::BigInt(){
	minus = false;
	//num���Զ���ʼ��
}

BigInt::BigInt(int u){
	if (u < 0){
		minus = true;
		u = -u;
	}
	else{
		minus = false;
	}
	num = (unsigned int)u;
}

BigInt::BigInt(const char * s){
	int slen = 0;
	while (s[slen] != '\0')++slen;
	//bool found = false;//�ҵ�����
	int i;
	for (i = 0; i < slen; ++i){
		if (s[i] != ' ')break;
	}
	if (i < slen && s[i] == '-'){
		minus = true;
		num = UBigInt(s + i + 1);
	}
	else{
		minus = false;
		num = UBigInt(s + i);
	}
}

//�Ƚ���
int BigIntCmp(const BigInt &a, const BigInt &b){
	if (!a.minus && !b.minus){
		return UBigIntCmp(a.num, b.num);
	}
	else{
		if (a.minus && b.minus){
			return -UBigIntCmp(a.num, b.num);
		}
		else{
			return a.minus ? -1 : 1;
		}
	}
	return 0;
}

bool operator==(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) == 0;
}

bool operator!=(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) != 0;
}

bool operator<(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) < 0;
}

bool operator<=(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) <= 0;
}

bool operator>(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) > 0;
}

bool operator>=(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) >= 0;
}
bool BigInt::isZero(){
	return num.isZero();
}
int BigInt::GetInt(){
	int w = num.GetInt();
	if (minus) w = -w;
	return w;
}
//����ʵ�֣�

//��������

BigInt BigInt::operator-(){
	BigInt temp = *this;
	temp.minus = !temp.minus;
	return temp;
}

BigInt& BigInt::operator++(){
	//++a
	return ((*this) += 1);
}
BigInt BigInt::operator++(int dummy){
	//a++
	BigInt old = (*this);
	(*this) += 1;
	return old;
}
BigInt& BigInt::operator--(){
	//--a
	return ((*this) -= 1);
}
BigInt BigInt::operator--(int dummy){
	//a--
	BigInt old = (*this);
	(*this) -= 1;
	return old;
}

BigInt& operator+=(BigInt &a, const BigInt &b){
	if (a.minus ^ b.minus){
		//���
		int w = UBigIntCmp(a.num, b.num);
		if (w > 0){
			a.num -= b.num;
		}
		else{
			if (w == 0){
				a.num = UBigInt();//��֪��Ϊʲô�ò���a.num = 0
			}
			else{
				a.num = b.num - a.num;
				a.minus = b.minus;
			}
		}
	}
	else{
		a.num += b.num;
	}
	return a;
}
BigInt& operator-=(BigInt &a, const BigInt &b){
	if (a.minus ^ b.minus){
		//���
		a.num += b.num;
	}
	else{
		int w = UBigIntCmp(a.num, b.num);
		if (w > 0){
			a.num -= b.num;
		}
		else{
			if (w == 0){
				a.num = UBigInt();
			}
			else{
				a.num = b.num - a.num;
				a.minus = !a.minus;
			}
		}
	}
	return a;
}
BigInt& operator*=(BigInt &a, const BigInt &b){
	a.minus = a.minus ^ b.minus;
	a.num *= b.num;
	if (a.num.isZero())a.minus = false;
	return a;
}
BigInt& operator/=(BigInt &a, const BigInt &b){
	//�����������ȡ��
	a.minus = a.minus ^ b.minus;
	a.num /= b.num;
	if (a.num.isZero())a.minus = false;
	return a;
}
BigInt& operator%=(BigInt &a, const BigInt &b){
	//����ȡģ�ķ�ʽ
	//Ϊģ�ľ���ֵ = |a| % |b|��ģ�ķ��ŵ��ڱ���������
	//��ʵ������������ģ���ʹ��C++����ͬ��������ȡģ��ʽ����һ��
	a.num %= b.num;
	if (a.num.isZero())a.minus = false;
	return a;
}
BigInt operator+(const BigInt &a, const BigInt &b){
	BigInt temp = a;
	temp += b;
	return temp;
}
BigInt operator-(const BigInt &a, const BigInt &b){
	BigInt temp = a;
	temp -= b;
	return temp;
}
BigInt operator*(const BigInt &a, const BigInt &b){
	BigInt temp = a;
	temp *= b;
	return temp;
}
BigInt operator/(const BigInt &a, const BigInt &b){
	BigInt temp = a;
	temp /= b;
	return temp;
}
BigInt operator%(const BigInt &a, const BigInt &b){
	BigInt temp = a;
	temp %= b;
	return temp;
}


//IO��
ostream& operator<<(ostream &os, BigInt &u){
	if (u.minus)os << "-";//�������-0���
	os << u.num;
	return os;
}

istream& operator>>(istream &is, BigInt &u){
	//�������ַ�������ʽ���룬������Ч�ַ�ʱֹͣ
	//�����ո�Ҳ��ֹͣ
	char c;
	Vector<char> buf;
	bool read = false;//���ٶ�һ��
	while (is.peek() == ' ' || is.peek() == '\n' || is.peek() == '\t'){
		is.get();
	}
	while ((c = is.peek()) && ((c >= '0' && c <= '9') || c == '-')){
		is.get();
		read = true;
		buf.push_back(c);
	}
	if (c == '\r'){
		is.get();
		read = true;
		c = is.peek();	//�����ļ�����\r\n�����
	}
	if (c == '\n' || c == ' ' || c == '\t' || c == '\b'){
		is.get();
		read = true;
	}
	if (!read)is.get();
	buf.push_back('\0');
	char *cstr = buf.c_array();
	u = BigInt(cstr);
	delete[] cstr;
	return is;
}