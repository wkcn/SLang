#include "SBuild.h"

streamx::streamx(){
	pointer = 0;
	eofed = true;
}

void streamx::Update(){
	while (!buf.empty()){
		if (pointer >= buf.front().size()){
			buf.pop();
			pointer = 0;
		}
		else{
			eofed = false;
			return;
		}
	}
	pointer = 0;
	eofed = true;
}

char streamx::peek(){
	Update();
	return buf.front()[pointer];
}

char streamx::get(){
	Update();
	return buf.front()[pointer++];
}

bool streamx::eof(){
	Update();
	return eofed;
}

void streamx::clear(){
	while (!buf.empty()){
		buf.pop();
	}
	pointer = 0;
	eofed = true;
}

streamx& streamx::operator<<(const string &s){
	eofed = false;
	buf.push(s);
	return *this;
}

SSugar::SSugar(){
	//���ȼ������ﲻ�����ȽϷ�
	level["+"] = 10;
	level["-"] = 10;
	level["*"] = 11;
	level["/"] = 11;
	level["%"] = 11;
	level["("] = 0;
	level[")"] = 0;
	level["=="] = 3;
	level["!="] = 3;
	level["<"] = 3;
	level["<="] = 3;
	level[">"] = 3;
	level[">="] = 3;
	level["~"] = 15;

	//���ֽڲ���������ͷ
	level["="] = 1;
	level["!"] = 1;
	updated = false;
}

bool SSugar::isOP(char c){
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '(' || c == ')' || c == '~');
}

void SSugar::SetStream(streamx *is){
	this->is = is;
}

string SSugar::NewSExp(string name, string left, string right){
	string se = "( ";
	se += name;
	se += ' ';
	se += left;
	se += " ";
	se += right;
	se += " )";
	return se;
}

void SSugar::PushS(stack<string> &res, stack<string> &op){
	string u = op.top();
	if (u == "~"){
		if (res.size() < 1)throw "��ʽ����";
		string right = res.top(); res.pop();
		res.push(NewSExp("-", "0", right));
	}
	else{
		if (res.size() < 2)throw "��ʽ����";
		string right = res.top(); res.pop();
		string left = res.top(); res.pop();
		res.push(NewSExp(u, left, right));
	}
	op.pop();
}

int SSugar::ISP(string &op){
	return level[op];
}

int SSugar::ICP(string &op){
	if (op[0] == '(')return 999;
	return level[op];
}

string SSugar::Formula2Sexp(string s){
	//��֧�ָ�������~~~
	//��ʱ���������Ⱥ�
	//s�����ո�,\n�ȷ���
	stack<string> res;
	stack<string> op;
	unsigned int i = 0;
	try{
		//���ﴦ��һԪ���ŵĹ����ǣ�ֻҪ�ø���ǰ���Ƿ�������Ϊ��һԪ����
		bool minus = true;
		for (unsigned int w = 0; w < s.size(); ++w){
			if (s[w] == '-' && minus){
				//s += "0";	//�Զ����㼴��
				s[w] = '~';
			}
			//s += q[w];
			minus = (isOP(s[w]) || s[w] == '<' || s[w] == '>' || s[w] == '=' || s[w] == '~');
		}
		while (i < s.size()){
			string c;
			c += s[i];
			if (i + 1 < s.size() && level.count(c + s[i + 1]))c += s[++i];
			if (level.count(c)){
				if (c == ")"){
					string u;
					while (!op.empty() && (u = op.top()) != "("){
						PushS(res, op);
					}
					if (op.empty())throw "����ı��ʽ���Ų�ƥ��!";
					op.pop();	//������
				}
				else{
					while (!op.empty() && ISP(op.top()) >= ICP(c)){
						PushS(res, op);
					}
					op.push(c);
				}
				++i;
			}
			else{
				string vname;
				vname += c;
				++i;
				while (i < s.size()){
					c.clear();
					c = s[i];
					//if (i + 1 < s.size() && level.count(c + s[i + 1]))c += s[++i];
					if (level.count(c)){
						break;
					}
					vname += c;
					++i;
				}
				res.push(vname);
			}
		}
		while (!op.empty()){
			PushS(res, op);
		}
	}
	catch (const char* msg){
		cout << msg << endl;
		throw "ʹ��#չ��ʱ�����˴��󣬿���������Ҫչ���Ĺ�ʽ����û�пո���";
	}
	catch (...){
		throw "ʹ��#չ��ʱ�����˴��󣬿���������Ҫչ���Ĺ�ʽ����û�пո���";
	}
	return res.top();
}


bool SSugar::isBlank(char c){
	//Ϊʲô��Ҳ��Сд��������= =
	return (c == ' ' || c == '\t' || c == '\n' || c == '\b' || c == '\r' || c == -1);
}

char SSugar::GetC(bool& can,bool del){
	char c;
	if (buf.empty()){
		if (is->eof()){
			can = false;
			return 0;
		}
		c = is->peek();
		if (del)is->get();
	}
	else{
		c = buf.front();
		if(del)buf.pop();
	}
	can = true;
	return c;
}

void SSugar::Update(){
	updated = true;
	char c;
	bool can;
	word.clear();
	while (true){
		c = GetC(can);
		if (!can)break;
		if (isBlank(c))continue;	//�����հ��ַ�
		if (c == '#'){
			string vn;
			int left = 0;
			while (can){
				c = GetC(can,false);
				if (!can || isBlank(c)){
					break;
				}
				if (c == ')'){
					if (left == 0){
						//����������
						break;
					}
					else{
						--left;
					}
				}else if (c == '('){
					++left;
				}
				GetC(can, true);	//����
				vn += c;
			}
			if (left != 0)throw "���Ų�ƥ��";
			try{
				string la = Formula2Sexp(vn);
				for (unsigned int w = 0; w < la.size(); ++w){
					buf.push(la[w]);
				}
			}
			catch (...){
				cout << "���빫ʽ����" << endl;
				throw "���빫ʽ����";
			}
		}
		else{
			//֮ǰ��!�Ὣ!=�˵�
			if (c == '$'){
				string vn;
				while (can){
					c = GetC(can);
					if (!can || c == '\n' || c == '-1'){
						break;
					}
					vn += c;
				}
				string la = "( ";
				la += vn;
				la += " )";
				for (unsigned int w = 0; w < la.size(); ++w){
					buf.push(la[w]);
				}
			}
			else if (0&&c == '_'){
				//��ʱ��SVM��ʵ��_չ��
				//_xxҲ������(array_get "_" xx)���﷨��
				string vn;
				while (can){
					c = GetC(can,false);
					if (!can || c == '\n' || !(c>='0' && c<='9')){
						break;
					}
					GetC(can,true);
					vn += c;
				}
				string la = " (array_get  \"_\"  ";
				la += vn;
				la += ") ";
				for (unsigned int w = 0; w < la.size(); ++w){
					buf.push(la[w]);
				}
			}
			else{
				if (c == '\"'){
					//�ַ���
					string vn;
					while (can){
						c = GetC(can);
						//cout << c;
						if (!can || c == '\"'){
							break;
						}
						vn += c;
					}
					word = "@";
					word += vn;
					break;
				}
				else{
					//ʣ�¿��ܣ����ţ�����������
					if (c == '(' || c == ')'){
						word = c;
						break;
					}
					else{
						string vn;
						vn += c;
						while (can){
							c = GetC(can, false);
							if (!can || isBlank(c)){
								break;
							}
							if (isOP(c))break;
							GetC(can);	//����

							vn += c;
						}
						word = vn;	//������Ż�
						break;
					}
				}
			}
		}
	}
}

string SSugar::peek(){
	if (!updated || word.empty()){
		Update();
	}
	return word;
}

string SSugar::next(){
	Update();
	return word;
}

bool SSugar::eof(){
	return is->eof() && buf.empty();
}

SBuild::SBuild(){
	macro = 0;
}

void SBuild::SetStream(streamx &is){
	sugar.SetStream(&is);
}

void SBuild::SetMacro(SExp *m,bool *ismacro){
	macro = m;
	this->ismacro = ismacro;
	*ismacro = false;
}

void SBuild::MarkParent(SExp *parent){
	if (!parent)return;
	for (int i = 0; i < parent->elems.size(); ++i){
		if (parent->elems[i] == 0)continue;
		parent->elems[i]->parent = parent;
		parent->elems[i]->id = i;
	}
}

SExp* SBuild::Build(bool first){
	try{
		//sugar�Ѿ���ȥ�ո���ַ�
		SExp *root = new SExp;
		if (first){
			//Ϊ��ʵ�ֺ�չ����ÿ�������COMMAND������
			//Build��д�������ʵ��ֳ�������
			root->type = SExp::COMMAND;
			root->elems.push_back(Build(false));
			if (1){
				//Ϊ�˱�����󣬽��������
				bool err = false;
				if (!sugar.eof()){
					err = true;
					cout << "����Ϊû�б�����ַ���" << endl;
				}
				while (!sugar.eof()){
					cout << sugar.next();
				}
				if (err)cout << endl;
			}
			MarkParent(root);
			return root;
		}
		if (sugar.eof())return 0;
		string s = sugar.peek();
		if (s != "("){
			//����
			if (s[0] == '@'){
				//�ַ���
				root->type = SExp::STR;
				root->name = s.substr(1);
				sugar.next();
				return root;
			}if (s[0] == ')'){
				return 0;	//������if��else���Ϊ��ʱ�����
			}if (s[0] == '`'){
				//���滻
				if (s[1] == '0'){
					root->type = SExp::VAR;
					char temp[32];
					sprintf(temp, "%d", macro->elems.size());
					root->name = s;
				}
				else{
					string num = s.substr(1);
					int u;
					sscanf(num.c_str(), "%d", &u);
					--u;	//`1���ǵ�һ������,����תΪ0�±�
					if (u >= macro->elems.size() || u < 0){
						throw "ʹ���˲����ڵĺ����";
					}
					root->Copy(macro->elems[u]);	//����Ϊ��
					*ismacro = true;
					//Copyʱ�Ѿ�markparent��
				}
				sugar.next();
				return root;
			}
			else{
				//һ�����
				root->type = SExp::VAR;
				root->name = s;
				sugar.next();
				return root;
			}
		}
		else{
			//��ȡ��ǩ
			string label = sugar.next();	//�ض�����
			if (label != "(")sugar.next();//������ǩ,�����������Ŷ���Ƕ��
			if (label == "if"){
				root->type = SExp::IF;
				root->elems[0] = Build(false);//condition
				root->elems[1] = Build(false);//true
				SExp *e = Build(false);//false
				if (e)root->elems[2] = e;
			}
			else if (label == "while"){
				root->type = SExp::WHILE;
				root->elems[0] = Build(false);//condition
				SExp *r;
				while (r = Build(false)){
					root->elems.push_back(r);
				}
			}
			else if (label == "\\" || label == "("){
				//COMMAND
				root->type = SExp::COMMAND;
				SExp *r;
				bool first = true;
				while (r = Build(false)){
					if (r->type == SExp::STR && first){
						first = false;
						continue;	//�����ַ���
					}
					root->elems.push_back(r);
				}
			}
			else{
				//Func
				root->type = SExp::FUNC;
				root->name = label;
				SExp *r;
				while (r = Build(false)){
					root->elems.push_back(r);
				}
			}
			sugar.next();	//��������
		}
		MarkParent(root);
		return root;
	}
	catch (...){
		return 0;
	}
}