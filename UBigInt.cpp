#include "UBigInt.h"

UBigInt::UBigInt(){
	_len = 1;
	datas[0] = 0;
}

UBigInt::UBigInt(unsigned int u){
	_len = 0;
	if (u == 0){
		_len = 1;
		datas[0] = 0;
		return;
	}
	while(u > 0){
		datas[_len] = u % 10000;
		u /= 10000;
		++_len;
	}
}

UBigInt::UBigInt(const char * s){
	int slen = 0;
	while (s[slen] != '\0')++slen;
	int base = 1;	//����������ʹ��10000����
	int buf = 0;	//����4λ����
	_len = 0;

	for (int i = slen - 1; i >= 0; i--){
		if (s[i] == ' ' || s[i] == '\n' || s[i] == '\t' || s[i] == '\r' || s[i] == '\b')continue;	//�����ո�
		if (!(s[i] >= '0' && s[i] <= '9')){
			throw "������������˷������ַ���";
		}
		int k = s[i] - '0';
		buf += k * base;
		if (base == 1000){
			base = 1;
			datas[_len++] = buf;
			buf = 0;
		}
		else{
			base *= 10;
		}
	}
	if (buf > 0){
		datas[_len++] = buf;
	}
	//������ֳ�������0
	if (_len == 0){
		_len = 1;
		datas[0] = 0;
	}
}

//�Ƚ���
int UBigIntCmp(const UBigInt &a,const UBigInt &b){
	if (a._len != b._len)return a._len - b._len;
	for (int i = a._len - 1; i >= 0; --i){
		if (a.datas[i] != b.datas[i])return a.datas[i] - b.datas[i];
	}
	return 0;
}

bool operator==(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) == 0;
}

bool operator!=(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) != 0;
}

bool operator<(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) < 0;
}

bool operator<=(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) <= 0;
}

bool operator>(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) > 0;
}

bool operator>=(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) >= 0;
}
bool UBigInt::isZero(){
	return (_len == 1 && datas[0] == 0);
}
int UBigInt::GetInt(){
	if (_len == 1)return datas[0];
	return datas[0] + datas[1] * 10000;
}
//����ʵ�֣�

//��������

UBigInt& UBigInt::operator++(){
	//++a
	return ((*this) += 1);
}
UBigInt UBigInt::operator++(int dummy){
	//a++
	UBigInt old = (*this);
	(*this) += 1;
	return old;
}
UBigInt& UBigInt::operator--(){
	//--a
	return ((*this) -= 1);
}
UBigInt UBigInt::operator--(int dummy){
	//a--
	UBigInt old = (*this);
	(*this) -= 1;
	return old;
}

UBigInt& operator+=(UBigInt &a, const UBigInt &b){
	//��ȷ�����Ż�
	if (b._len == 1){
		if (b.datas[0] == 0){
			return a;
		}
	}
	int carry = 0;
	//�������һ�ֽϼ򵥵�д��
	//�������������������������
	//��ʵ�����Խ�һ���Ż������粻���ڽ�λ�Һ͵ĳ����ѳ�������һ������ʱ
	int maxLen = a._len > b._len ? a._len : b._len;
	int anum, bnum;
	for (int i = 0; i < maxLen; ++i){
		anum = i < a._len ? a.datas[i] : 0;
		bnum = i < b._len ? b.datas[i] : 0;
		a.datas[i] = anum + bnum + carry;
		carry = a.datas[i] / 10000;
		a.datas[i] %= 10000;
	}
	a._len = maxLen;
	if (carry > 0){
		a.datas[a._len++] = carry;	//�ɼӷ����ʣ�carry�������10000
	}
	return a;
}

UBigInt& operator-=(UBigInt &a, const UBigInt &b){
	//��ȷ�����Ż�
	if (b._len == 1){
		if (b.datas[0] == 0){
			return a;
		}
	}
	//����ʽ�������ӵ�λ��ʼ��
	//���ǵ�Ч�ʣ����Ե�����ٱȽ�a,b��С
	//�򵥵ĳ��ȱȽ�,a<bʱ
	if (a._len < b._len){
		throw "�޷�����������a-b��,a���ܱ�bС";
	}
	for (int i = 0; i < b._len; ++i){
		a.datas[i] -= b.datas[i];
		if (a.datas[i] < 0){
			//���λ��λ
			//assert(i + 1 < a._len);	//���Դ��ڸ�λ������˵��a<b
			if (i + 1 >= a._len){
				throw "�޷�������a-b�У�a���ܱ�bС";
			}
			--a.datas[i + 1];
			a.datas[i] += 10000;
		}
	}
	while (a.datas[a._len - 1] == 0 && a._len > 1)--a._len;//���³���
	//���ﴦ�����
	//��Ϊ�����ĸ�λ�������0������ڸ�λ��b>a�������֮ǰ�Ľ�λ�����б���
	return a;
}

UBigInt& operator*=(UBigInt &a, const UBigInt &b){
	//��ȷ�����Ż�
	if (b._len == 1){
		if (b.datas[0] == 0){
			a.datas.clear();
			a.datas[0] = 0;
			a._len = 1;
			return a;
		}else if (b.datas[0] == 1){
			return a;
		}
	}
	Vector<int> temp;//��ʱ�ۼ���
	for (int i = 0; i < b._len; ++i){
		for (int j = 0; j < a._len; ++j){
			temp[i + j] += b.datas[i] * a.datas[j];
			temp[i + j + 1] += temp[i + j] / 10000;
			temp[i + j] %= 10000;
			//����򵥵����˵�һ�ν�λ������ֹ���
		}
	}
	//wΪ����������һ������λ����СֵΪ0
	int w;
	for (w = temp.size() - 1; w > 0; --w){
		if (temp[w] != 0)break;
	}
	a._len = w + 1;
	a.datas.resize(a._len);
	int carry = 0;
	for (int i = 0; i <= w; ++i){
		temp[i] += carry;
		a.datas[i] = temp[i] % 10000;
		carry = temp[i] / 10000;
	}
	if (carry > 0){
		a.datas[a._len++] = carry;
	}
	return a;
}

//Ϊ�˸������дdiv��mod,�����װΪһ������
UBigInt& UBigIntDivide(UBigInt &a, const UBigInt &b, bool mod){
	//ʹ�ö��ַ��жϳ���ĳλ
	//a��λ��С��b��λ�������������a<b��
	if (a._len < b._len){
		if (mod){
			return a;
		}
		else{
			a.datas.clear();
			a.datas[0] = 0;
			a._len = 1;
			return a;
		}
	}
	//��������Ż������Ǻ�ȷ������Ƶ��
	if (b._len == 1){
		if (b.datas[0] == 0){
			throw "��������Ϊ0";
		}
		if (b.datas[0] == 1){
			//����1
			if (!mod){
				return a;
			}
			else{
				a.datas.clear();
				a.datas[0] = 0;
				a._len = 1;
				return a;
			}
		}
	}
	//a��λ�����ڵ���b��λ��
	Vector<int> res;	//�������
	int notZero = a._len - 1;//a������ķ���λ
	int qlen = a._len - b._len;//for�еļ��������еı�����ʱ�̸��µ�
	for (int q = 0; q <= qlen; ++q){
		//q������������������ƶ��ĸ���
		int delta = a._len - b._len - q;//���λ��ƫ��
		//�ö���ȡ�̵�ĳλ�����ֵ(����������ƣ�����hiȡ10000)
		int j = b._len - 1 + delta;	//��ǰa����������λ

		int big = 0;
		int base = 1;
		for (int u = j; u <= notZero; ++u){
			big += a.datas[u] * base;	//����д��дΪ��=
			base *= 10000;
		}

		int lo = big / (b.datas[b._len -1] + 1), hi = big / (b.datas[b._len - 1]) + 1;//[lo,hi)
		//int lo = 0, hi = 10000;
		UBigInt temp;	//��Ҫ��ȥ��ƫ����
		while (lo < hi){
			int mi = (lo + hi) >> 1;
			temp = b * mi;
			bool isLarge = false;//����
			//cout << temp << "-" << a << endl;
			if (temp._len - 1 + delta < notZero){
				isLarge = false;
				//�����a���Ǳ�temp��һλ
			}
			else{
				for (int i = temp._len - 1; i >= 0; --i){
					//cout << "cmp" << temp << "_--" << temp.datas[i] << " " << a.datas[i + delta] << endl;
					if (temp.datas[i] > a.datas[i + delta]){
						isLarge = true;
						break;
					}
					else{
						if (temp.datas[i] < a.datas[i + delta])break;
					}
				}
			}
			isLarge ? hi = mi : lo = mi + 1;
		}
		//cout << "--" << temp << endl;
		//--loΪ����
		res[delta] = --lo;
		temp = b * res[delta];	//������ֹ����е�temp��һ�������ս�� = = ������Ҫ��һ��
		//����λ��ȥtemp
		//cout << a << "===" << temp << "[][]" << delta << endl;
		for (int i = 0; i < temp._len; ++i){
			a.datas[i + delta] -= temp.datas[i];
			//����Ƿ���Ҫ��ǰ��λ
			for (int w = i + delta; w <= notZero; ++w){
				if (a.datas[w] < 0){
					a.datas[w] += 10000;
					--a.datas[w + 1];
				}
				else{
					break;
				}
			}
		}
		//����a����߷���λ
		for (; notZero > 0; --notZero){
			if (a.datas[notZero])break;
		}
		//cout <<"=="<< a << endl;
	}
	if (mod){
		//ȡģ
		int w;
		//ȥ��a�����ǰ׺0,��ģ�����ʣ�a%b��λ�� <= b��λ��
		for (w = b._len - 1; w > 0; --w){
			if (a.datas[w] != 0)break;
		}
		a._len = w + 1;
		a.datas.resize(a._len);
	}
	else{
		//wΪ����������һ������λ����СֵΪ0
		int w;
		for (w = res.size() - 1; w > 0; --w){
			if (res[w] != 0)break;
		}
		a._len = w + 1;
		a.datas.resize(a._len);
		for (int i = 0; i <= w; ++i){
			a.datas[i] = res[i];
		}
	}
	return a;
}

UBigInt& operator/=(UBigInt &a, const UBigInt &b){
	return UBigIntDivide(a, b, false);
}
UBigInt& operator%=(UBigInt &a, const UBigInt &b){
	return UBigIntDivide(a, b, true);
}

UBigInt operator+(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp += b;
	return temp;
}
UBigInt operator-(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp -= b;
	return temp;
}
UBigInt operator*(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp *= b;
	return temp;
}
UBigInt operator/(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp /= b;
	return temp;
}
UBigInt operator%(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp %= b;
	return temp;
}


//IO��
ostream& operator<<(ostream &os, UBigInt &u){
	//�Ҽǵ�C++ IO������һ�ֺܼ򵥵Ĳ��㷽���������Ҳ�����
	os.fill('0');
	os << u.datas[u._len - 1];
	for (int i = u._len - 2; i >= 0; i--){
		os << setw(4) << u.datas[i];
	}
	os.fill();
	return os;
}

istream& operator>>(istream &is, UBigInt &u){
	//�������ַ�������ʽ���룬������Ч�ַ�ʱֹͣ
	//�����ո�Ҳ��ֹͣ
	char c;
	Vector<char> buf;
	bool read = false;//���ٶ�һ��
	while (is.peek() == ' ' || is.peek() == '\n' || is.peek() == '\t'){
		is.get();
	}
	while ((c = is.peek()) && c >= '0' && c <= '9'){
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
	u = UBigInt(cstr);
	delete[] cstr;
	return is;
}