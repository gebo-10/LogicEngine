#pragma once
#include"component.h"
#include<vector>
#include<any>
#include<map>
#include<iostream>
class LogicEngine
{
public:
	std::vector<Component *> coms;
	
	LogicEngine()
	{
		
	}

	~LogicEngine()
	{
	}
	template<typename... Args>
	void call(Args ...args) {
		auto com = new Component;
		
		auto fun = std::any_cast<std::function<void(Args...)> >(com->function[0]);
		fun(args...);
	}


};
