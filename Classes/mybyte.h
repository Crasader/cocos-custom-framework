#include <iostream>  
using namespace std;
class mybyte{
public:
	mybyte();
	~mybyte();
	void assign(char*, int);
	void add(char*, int);
	void add(char);
	int remove(int, int);
	void clear();
	char* find(int, char*, int);
	char* find(int, char);
	int getlen();
	char* data();
private:
	char *d;
	int len;
};
mybyte::mybyte(){
	d = 0;
	len = 0;
}
mybyte::~mybyte(){
	delete[] d;
}
void mybyte::assign(char* p, int l){
	if (d){
		delete[] d;
		len = 0;
	}
	d = new char[l];
	len = l;
	memcpy(d, p, l);
}
void mybyte::add(char ch){
	len++;

	char* t = new char[len];
	if (d){
		memcpy(t, d, len - 1);
	}
	t[len - 1] = ch;
	delete[]d;
	d = t;

}
void mybyte::add(char* s, int l){
	len += l;

	char* t = new char[len];
	if (d){
		memcpy(t, d, len - l);
	}
	memcpy(t + len - l, s, l);
	delete[]d;
	d = t;
}
int mybyte::remove(int s, int l){
	if (s + l > len){
		return 0;
	}
	char* t = new char[len - l];
	int i1 = 0;
	for (int i = 0; i1 < len - l; i++){
		if (i<s || i>s + l - 1){
			t[i1++] = d[i];
		}
	}
	delete[] d;
	d = t;
	len = len - l;
	return 1;
}
char* mybyte::data(){
	return d;
}
int mybyte::getlen(){
	return len;
}
char* mybyte::find(int s, char* str, int l){
	if (s + l > len){
		return 0;
	}
	char* t = d + s;
	for (int i = 0; i <= len - l; i++){
		int flag = 0;
		for (int i1 = 0; i1 < l; i1++){
			if (t[i + i1] != str[i1]){
				flag = 1;
				break;
			}
		}
		if (flag == 0){
			return t + i;
		}
	}
	return 0;
}
char* mybyte::find(int s, char ch){
	if (s + 1 > len){
		return 0;
	}
	char* t = d + s;
	for (int i = 0; i < len; i++){
		if (t[i] == ch){
			return t + i;
		}
	}
	return 0;
}
void mybyte::clear(){
	delete[] d;
	d = 0;
	len = 0;
}