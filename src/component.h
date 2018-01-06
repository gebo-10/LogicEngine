#pragma once
#include<vector>
#include<any>
#include<map>
#include<iostream>
#include<functional>
typedef int CallType;
class Params
{
public:
    Params(){}
    ~Params(){ }
};

class Component
{
public:
	std::map<int, std::any> function;
	Component()
	{
		function[0] = std::function<void(int,float)>([this](int a, float b){
			std::cout << a << b << std::endl;
		});
	}

	~Component()
	{
	}

	virtual void init() {}

	template<typename... Args>
	void call(CallType type,Args... args) {
		
	}
    void on_call(CallType type, Params params) {

    }

};
