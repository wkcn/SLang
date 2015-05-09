#include<iostream>
#include<iomanip>

#include "BigInt.h"
#include "SBuild.h"
#include "SVM.h"
#include <sstream>
using namespace std;

int main(){
	SVM vm;
	streamx ss;
	srand(unsigned int(time(0)));

	//帮助文件
	SBuild readme;
	streamx rs;
	rs << "(readme)";
	readme.SetStream(rs);
	vm.Eval(readme.Build());

	while (true){
		SBuild bu;
		bu.SetStream(ss);
		string sen;
		getline(cin, sen);
		ss.clear();
		ss << sen;
		SExp *e = bu.Build();
		BigInt value = vm.Eval(e);
		vm.SetVar("ans", value);
		cout << value << endl;
		DelSExp(e);
	}

	return 0;

	//以下是测试用例，现在不需要了

	BigInt a, b;
	a = 100319000;
	b = a * a;
	BigInt c = b / a;
	cout << a << endl;
	cout << b << endl;
	cout << c << endl;

	//a += "56"; 
	string z = "1";
	int JD = 8;
	for (int i = 0; i < JD; i++)z += '0';
	BigInt q = z.c_str();
	BigInt w = 4 * q;
	BigInt pi = 4 * q;
	//cout << "---wwww" << pi<<endl;
	for (int i = 0; i < 0; i++){
		pi -= w / (4 * i + 3) - w / (4 * i + 5);
		if (!(i % 500)){
			cout << "case:" << i << endl; cout << pi << endl;
		}
	}

	int ww = 0;
	while (false){
		ww+=1000;
		a = b = ww;
				if (a*b / b != a){
					cout << a << " + " << b << " = " << a + b << endl;
					cout << a << " - " << b << " = " << a - b << endl;
					cout << a << " * " << b << " = " << a * b << endl;
					cout << a << " / " << b << " = " << a / b << endl;
					cout << a << " % " << b << " = " << a % b << endl;
					break;
				}
	}
	while (true){
		cin >> a >> b;
		cout<<endl;
		cout << a << " + " << b << " = " << a + b << endl;
		cout << a << " - " << b << " = " << a - b << endl;
		cout << a << " * " << b << " = " << a * b << endl;
		cout << a << " / " << b << " = " << a / b << endl;
		cout << a << " % " << b << " = " << a % b << endl;
	}
	return 0;
}