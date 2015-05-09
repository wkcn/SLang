#include "SVM.h"
#include "SBuild.h"

//һЩ���ú���
BigInt Sprint(SPar par){
	bool first = true;
	for (int i = 0; i < par.size(); ++i){
		if (!first)cout << " ";
		first = false;
		if (par.GetType(i) == SExp::STR){
			cout << par.GetStr(i);
		}
		else{
			cout << par.GetNum(i);
		}
	}
	cout << endl;
	return BigInt();
}

BigInt Sprintf(SPar par){
	for (int i = 0; i < par.size(); ++i){
		if (par.GetType(i) == SExp::STR){
			cout << par.GetStr(i);
		}
		else{
			cout << par.GetNum(i);
		}
	}
	return BigInt();
}

BigInt SRestart(SPar par){
	par.vm->Restart();
	cout << "�Ѿ��ɹ����������" << endl;
	return BigInt();
}

BigInt SClear(SPar par){
	for (int i = 0; i < par.size(); ++i){
		par.vm->ClearVar(par.GetStr(i));
	}
	return BigInt();
}

BigInt SInput(SPar par){
	for (int i = 0; i < par.size(); ++i){
		cin >> par[i];
	}
	return BigInt();
}

BigInt SRand(SPar par){
	return rand();
}

BigInt STime(SPar par){
	return int(time(0));
}

BigInt SClock(SPar par){
	return clock() * 1000 / CLOCKS_PER_SEC;
}

BigInt SAddPath(SPar par){
	for (int i = 0; i < par.size(); ++i){
		par.vm->AddPath(par.GetStr(i));
	}
	return BigInt();
}

BigInt SArrayGet(SPar par){
	//(array_get "a" 0)	��ȡa[0]
	return par.vm->GetArray(par.GetStr(0), par.GetNum(1).GetInt());
}

BigInt SArraySet(SPar par){
	//(array_set "a" 0 11)
	par.vm->SetArray(par.GetStr(0), par.GetNum(1).GetInt(), par.GetNum(2));
	return par.GetNum(2);
}

BigInt SArraySize(SPar par){
	return par.vm->GetArraySize(par.GetStr(0));
}

BigInt SArrayClear(SPar par){
	for (int i = 0; i < par.size(); ++i){
		par.vm->ClearArray(par.GetStr(i));
	}
	return BigInt();
}

BigInt SDebug(SPar par){
	par.vm->debug = !(par.GetNum(0).isZero());
	string msg[2] = { "�ر�", "����" };
	cout << msg[par.vm->debug] << "Debugģʽ";
	return BigInt();
}

SVM::SVM(){
	debug = false;
	parent = 0;

	AddPath("./");	//��Ӹ�Ŀ¼
	AddPath("./data/");

	AddFunc("print", Sprint);
	AddFunc("printf", Sprintf);
	AddFunc("restart", SRestart);
	AddFunc("re", SRestart);
	AddFunc("clear", SClear);
	AddFunc("input", SInput);
	AddFunc("rand", SRand);
	AddFunc("time", STime);
	AddFunc("clock", SClock);
	AddFunc("addpath", SAddPath);
	AddFunc("array_set", SArraySet);
	AddFunc("array_get", SArrayGet);
	AddFunc("array_size", SArraySize);
	AddFunc("array_clear", SArrayClear);
	AddFunc("debug", SDebug);
}

SVM::~SVM(){
	for (map<string, SExp*>::iterator iter = sfuncs.begin(); iter != sfuncs.end(); ++iter){
		DelSExp(iter->second);
	}
}

void SVM::AddFunc(const string &name, SFunc f){
	funcs[name] = f;
}

void SVM::AddPath(const string &name){
	path.push_back(name);
}

void SVM::Restart(){
	vars.clear();
	arrays.clear();
	sfuncs.clear();
}

Vector<BigInt>& SVM::FindArray(const string &name){

	return arrays[name];

	//�����ݲ����ǣ�����ʵ��ȫ�ֱ���
	if (name[0] == '_')return arrays[name];
	SVM *node = this;
	while (node){
		if (node->arrays.count(name))return node->arrays[name];
		node = node->parent;
	}
	return arrays[name];
}

void SVM::SetArray(const string &name, int id, const BigInt &value){
	FindArray(name)[id] = value;
}

BigInt SVM::GetArray(const string &name, int id){
	return FindArray(name)[id];
}

int SVM::GetArraySize(const string &name){
	return FindArray(name).size();
}

void SVM::ClearArray(const string &name){
	arrays.erase(name);
	return;


	SVM *node = this;
	while (node){
		if (node->arrays.count(name)){ node->arrays.erase(name); break; }
		node = node->parent;
	}
	//arrays.erase(name);
}

void SVM::ClearVar(const string &v){
	vars.erase(v);	//ֱ��ɾ������
}

BigInt& SVM::GetVar(const string &name){
	if (name[0]=='_'){
		int id;
		sscanf(name.c_str(), "_%d", &id);
		return arrays["_"][id];
	}
	if (!vars.count(name)){
		if ((name[0] >= '0' && name[0] <= '9')||name[0] == '-'){
			vars[name] = BigInt(name.c_str());
			//���֣������ҹ�����һ��'Bug'��������Լ�����ĳ�����ֵ���ֵ��Ϊ�˺���
		}
		else{
			vars[name] = BigInt();	//һ�����
		}
	}
	return (vars[name]);
}

BigInt SVM::Eval(SExp *e){
	return GetValue(e);
}

void SVM::SetParent(SVM *p){
	parent = p;
}

SExp* SVM::SearchFile(const string &filename,SExp *exp,bool *ismacro){
	string name;
	ifstream fin;
	bool can = false;
	for(int i=0;i<path.size();++i){
		name = path[i] + filename + ".txt";
		fin.open(name);
		if (!fin.fail()){
			can = true;
			break;
		}
	}
	if (!can)return 0;
	SBuild bu;	//build����һ���Եģ����Բ��õ���һЩӰ��δ��������
	streamx ss;
	if(exp)bu.SetMacro(exp,ismacro);	//��
	bu.SetStream(ss);
	string buf;
	while (!fin.eof()){
		getline(fin, buf);	//eof�÷�
		ss << buf;
	}
	fin.close();
	return bu.Build();
}

void SVM::SetVar(const string &name, const BigInt &value){
	vars[name] = value;
}

BigInt SVM::GetValue(SExp *s){
	if (s == 0)return BigInt();
	if (debug){
		string msg[] = { "VAR", "FUNC", "COMMAND", "IF", "WHILE", "STR" };
		if(s->type == SExp::FUNC || s->type == SExp::IF || s->type == SExp::WHILE)cout << msg[s->type];
		cout << s->name << " ";
		//if (s->type == SExp::FUNC || s->type == SExp::IF || s->type == SExp::WHILE)cout << endl;
	}
	try{
		if (s->type == SExp::COMMAND){
			if (s->elems.size() == 0)return BigInt();	//�����
			for (int i = 0; i < s->elems.size() - 1; ++i){
				GetValue(s->elems[i]);
			}
			return GetValue(s->elems[s->elems.size() - 1]);
		}
		else if (s->type == SExp::FUNC){
			if (s->name == "+"){
				BigInt count;//�ۼ���
				if (s->elems.size() > 0)count = GetValue(s->elems[0]);
				for (int i = 1; i < s->elems.size(); ++i){
					count += GetValue(s->elems[i]);
				}
				return count;
			}
			else if (s->name == "-"){
				BigInt count;//�ۼ���
				if (s->elems.size() > 0)count = GetValue(s->elems[0]);
				for (int i = 1; i < s->elems.size(); ++i){
					count -= GetValue(s->elems[i]);
				}
				return count;
			}
			else if (s->name == "*"){
				BigInt count;//�ۼ���
				if (s->elems.size() > 0)count = GetValue(s->elems[0]);
				for (int i = 1; i < s->elems.size(); ++i){
					count *= GetValue(s->elems[i]);
				}
				return count;
			}
			else if (s->name == "/"){
				BigInt count;//�ۼ���
				if (s->elems.size() > 0)count = GetValue(s->elems[0]);
				for (int i = 1; i < s->elems.size(); ++i){
					count /= GetValue(s->elems[i]);
				}
				return count;
			}
			else if (s->name == "%"){
				BigInt count;//�ۼ���
				if (s->elems.size() > 0)count = GetValue(s->elems[0]);
				for (int i = 1; i < s->elems.size(); ++i){
					count %= GetValue(s->elems[i]);
				}
				return count;
			}
			else if (s->name == "="){
				//��ֵ
				return (GetVar(s->elems[0]->name) = GetValue(s->elems[1]));
			}
			else if (s->name == "=="){
				return (GetValue(s->elems[0]) == GetValue(s->elems[1]));
			}
			else if (s->name == "!="){
				return (GetValue(s->elems[0]) != GetValue(s->elems[1]));
			}
			else if (s->name == "<"){
				return (GetValue(s->elems[0]) < GetValue(s->elems[1]));
			}
			else if (s->name == "<="){
				return (GetValue(s->elems[0]) <= GetValue(s->elems[1]));
			}
			else if (s->name == ">"){
				return (GetValue(s->elems[0]) > GetValue(s->elems[1]));
			}
			else if (s->name == ">="){
				return (GetValue(s->elems[0]) >= GetValue(s->elems[1]));
			}
			else{
				//��������
				if (funcs.count(s->name) == 0){
					SExp *e = 0;
					if (sfuncs.count(s->name) == 0){
						/*
						if (s->name[0] == '`'){
							//�꺯������ǿ��
							e = SearchFile(s->name.substr(1), s);
							if(s->parent)s->parent->elems[s->id] = e;	//���滻
							//sfuncs[s->name] = e;
							DelSExp(s);	//������Դ�����滻ʹ�õ������
							return GetValue(e);
						}
						else{
							//��ͨ����
							e = SearchFile(s->name,s);
							sfuncs[s->name] = e;	//��¼
						}*/
						//����ͨ�����ͺ����һ���.
						//���ڵĺ�չ��Ч�ʲ��ߣ���Ϊ�кܶ��ظ��������Щ����Ŀ��ʵ�����½����ƶ�
						bool ismacro = false;
						e = SearchFile(s->name, s,&ismacro);
						if (ismacro){
							if (s->parent)s->parent->elems[s->id] = e;	//���滻
							DelSExp(s);	//������Դ�����滻ʹ�õ������
						}
						else{
							sfuncs[s->name] = e;
						}
						if(ismacro)return GetValue(e);
					}
					else{
						e = sfuncs[s->name];
					}
					if (e){
						SVM fvm;
						//���ݲ������µ������
						char temp[64];
						fvm.debug = debug;
						fvm.SetParent(this);

						//fvm.vars["_0"] = BigInt(s->elems.size());	//_0����Ϊ������������һ��������_1
						fvm.SetArray("_", 0, BigInt(s->elems.size()));
						for (int i = 0; i < s->elems.size(); ++i){
							sprintf(temp, "_%d", i + 1);
							//fvm.vars[temp] = GetValue(s->elems[i]);
							fvm.SetArray("_", i + 1, GetValue(s->elems[i]));
						}
						return fvm.Eval(e);
					}
					cout << "���� �����ں�����" << s->name << "\n";
					return BigInt();
				}
				return funcs[s->name](SPar(this, s));
			}
		}
		else if (s->type == SExp::VAR){
			return GetVar(s->name);
		}
		else if (s->type == SExp::IF){
			if ((GetValue(s->elems[0])).isZero()){
				if (s->elems.size() == 3)return GetValue(s->elems[2]);
				return BigInt();
			}
			else{
				return GetValue(s->elems[1]);
			}
		}
		else if (s->type == SExp::WHILE){
			while (!(GetValue(s->elems[0])).isZero()){
				for (int i = 1; i < s->elems.size(); ++i){
					GetValue(s->elems[i]);
				}
			}
			return BigInt();//ѭ���������ص���0
		}
	}
	catch (const char *msg){
		cout << msg << endl;
		cout << "����������������������Ƿ���ȷ"<<endl;
	}
	catch (...){
		cout << "�������쳣�����������������Ƿ���ȷ" << endl;
	}
	return BigInt();	//��Ч����
}