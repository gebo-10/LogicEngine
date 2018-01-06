#include<stdio.h>
#include<iostream>
#include"logic_engine.h"
using namespace std;

void p2(int a, float b) {
	cout << a << b << endl;
}
template<typename T>
void pone(T t){
	cout << t << endl;
}
template<typename... Args>
void p(Args... args) {
	//(pone(args),...);
	p2(args...);
}
void main() {
	//p(123, 40.5);
	LogicEngine le;
	le.call(1, (float)2.8);
	getchar();
}