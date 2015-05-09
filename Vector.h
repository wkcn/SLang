#ifndef VECTOR_H
#define VECOTR_H

/*
	Vector��
	�����Vector�����Զ������±�����ռ��С
	���һ��Զ���ʼ������Ԫ��
*/


const int DEFAULT_CAPACITY = 8; //Ĭ�ϳ�ʼ����

template<typename T>
class Vector{
protected:
	T *_elem;
	int _size;	//��ǰʹ������
	int _cap;	//��ǰ��������
private:
	void CheckMem();//��鵱ǰ����
	void Copy(T* addr);	//������ǰ����

public:
	Vector();
	Vector(const Vector<T>&);
	Vector<T>& operator=(const Vector<T>&);	//��¡	������Ĺ��캯��ֻ���ڱ�����ʼ��ʱ���ã���ָ�����Ҫע��д�Ⱥ����أ�
	~Vector();

	void push_back(const T& e);
	T pop_back();

	T* c_array();	//����C���͵�����ָ��

	void clear();
	void resize(int newSize);

	int size() const;
	bool empty() const;

	T& operator[](int i);
	T& operator[](int i) const;	//����ģʽ�ķ���

};

/*
Vector��ʵ��
��һ���е�����
����C++��������֧��export��䣬ģ�������ʱ�������⡣
*/

template <typename T>
Vector<T>::Vector(){
	_size = 0;
	_cap = DEFAULT_CAPACITY;
	_elem = new T[_cap];
}

template <typename T>
Vector<T>::Vector(const Vector<T> &v){
	//Vector<T> a = bʱ����
	//����ǿ��ת��ʱ����
	_size = v.size();
	_cap = 0;
	_elem = 0;
	CheckMem();
	for (int i = 0; i < v.size(); i++){
		_elem[i] = v._elem[i];
	}
	//_size = v.size();
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T> &v){
	_size = v.size();
	CheckMem();
	for (int i = 0; i < v.size(); i++){
		_elem[i] = v._elem[i];
	}
	return *this;
}

template<typename T>
Vector<T>::~Vector(){
	if (_elem){
		//cout << "dele" << int(_elem) << endl;
		delete[] _elem;
	}
}

template<typename T>
void Vector<T>::CheckMem(){
	//ò����delete[]ɾ������Ϊ1������ʱ�ᱨ��
	//����
	if (_size >= _cap){
		int _newcap = _size << 1;//����2����Ȼ���е��񲻳�����Ż�
		if (_newcap < DEFAULT_CAPACITY)_newcap = DEFAULT_CAPACITY;
		T *q = new T[_newcap];
		Copy(q);
		if(_elem)delete[] _elem;
		_elem = q;
		_cap = _newcap;
		return;
	}
	//��С
	if (_size * 3 <= _cap && (_size * 2) >= DEFAULT_CAPACITY){
		int _newcap = _size << 1;
		T *q = new T[_newcap];
		Copy(q);
		if(_elem)delete[] _elem;
		_elem = q;
		_cap = _newcap;
		return;
	}
}

template<typename T>
void Vector<T>::Copy(T* addr){
	int s = _size < _cap ? _size : _cap;	//����resize��������
	for (int i = 0; i < s; i++){
		addr[i] = _elem[i];
	}
}

//������������
template<typename T>
T& Vector<T>::operator[](int i){
	if (i < 0){
		throw "ʹ���˸����±꣬����";
	}
	//����дӰ����Ч�ʣ���΢����������
	//������ʹ��Ƹ����㡣
	if (i >= _size){
		int oldsize = _size;
		_size = i + 1;
		CheckMem();
		for (int k = oldsize; k <= i; k++){
			//��ʼ��
			_elem[k] = T();
		}
	}
	return _elem[i];
}
//����ģʽ
template<typename T>
T& Vector<T>::operator[](int i) const{
	if (i >= _size){
		throw "�±�Խ�磡";
	}
	return _elem[i];
}

template<typename T>
void Vector<T>::push_back(const T& e){
	CheckMem();
	_elem[_size++] = e;
}

template<typename T>
T Vector<T>::pop_back(){
	if (_size <= 0){
		throw "POP�˿�Vector������";
	}
	return _elem[--_size];
}

template<typename T>
T* Vector<T>::c_array(){
	T* q = new T[_size];
	for (int i = 0; i < _size; i++){
		q[i] = _elem[i];
	}
	return q;
}

//��С
template<typename T>
int Vector<T>::size() const{
	return _size;
}

template<typename T>
bool Vector<T>::empty() const{
	return _size == 0;
}

//�����
template<typename T>
void Vector<T>::clear(){
	_size = 0;
	CheckMem();
}

template<typename T>
void Vector<T>::resize(int newSize){
	int oldSize = _size;
	_size = newSize;
	CheckMem();
	//���������
	for (int i = oldSize; i < newSize; ++i){
		_elem[i] = T();
	}
}

#endif