#include<stdio.h>
#include<iostream>
#include"logic_engine.h"
#include<Windows.h>
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

class Comp :public Component
{
public:
    Comp()
    {
        register_call<int, float>(1, [this](int a, float b)->void {std::cout << a + b; });
    }

    ~Comp()
    {
    }

private:

};
void gen_params(Params &params) { return; }
template <class T, class... Args>
void gen_params(Params &params, T head, Args... rest) {
    params.push_param(head);
    gen_params(params, rest...);
}
typedef Component* ( * AddProc)(); 
void main() {
	//p(123, 40.5);
    auto dll=LoadLibrary("comp.dll");
    AddProc instance = (AddProc)GetProcAddress(dll, "instance");
    auto com=instance();
    
    Comp comp;
    Params p;
    gen_params(p, 1, (float)2.5);
    comp.in((CallType)1, p);
    com->in((CallType)1, p);
	getchar();
}