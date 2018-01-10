#include"component.h"
#include "gebo.h"
#include<boost/filesystem.hpp>
#include<Windows.h>
typedef Component* (*ReturnCompFun)();
typedef Component* ComponentPtr;
class Launcher:public Component
{
	public:
	struct Moduel {
		std::string name;
		Gebo::Interface type;
		std::string path;
		ReturnCompFun instance;
		ReturnCompFun create;
		HMODULE module;
	};
	std::vector<Moduel> moduels;

	Launcher()
	{
		register_call<std::string,ReturnCompFun *, ReturnCompFun *>(Gebo::GET_MODEL_BY_NAME, [this](std::string name,ReturnCompFun * instance, ReturnCompFun * create) {
			for (auto moduel:moduels)
			{
				if (moduel.name==name)
				{
					*instance = moduel.instance;
					*create = moduel.create;
					return;
				}
			}
		});

		register_call<std::string, ComponentPtr*>(Gebo::GET_SINGLETON_BY_NAME, [this](std::string name, ComponentPtr* ptr) {
			for (auto moduel : moduels)
			{
				if (moduel.name == name)
				{
					*ptr = moduel.instance();
					return;
				}
			}
		});

		register_call<std::string, ComponentPtr*>(Gebo::GET_INSTANCE_BY_NAME, [this](std::string name, ComponentPtr* ptr) {
			for (auto moduel : moduels)
			{
				if (moduel.name == name)
				{
					*ptr = moduel.create();
					return;
				}
			}
		});
	}

	~Launcher()
	{
	}

	virtual void init() {
		namespace bf = boost::filesystem;
		//bf::path path("/tmp/test");
		bf::path current_cpath = bf::current_path(); //取得当前目录 
		bf::directory_iterator end;
		for (bf::directory_iterator pos(current_cpath); pos != end; pos++)
		{
			std::cout << *pos << std::endl;
			std::string file = pos->path().string();
			auto dll = LoadLibrary(file.c_str());
			ReturnCompFun instance = (ReturnCompFun)GetProcAddress(dll, "instance");
			ReturnCompFun create = (ReturnCompFun)GetProcAddress(dll, "create");
			if (instance != NULL && create != NULL) {
				Moduel moduel;
				moduel.module = dll;
				moduel.name = pos->path().filename().stem().string();
				moduel.path= pos->path().string();
				moduel.instance = instance;
				moduel.create = create;
				moduels.push_back(moduel);
			}
		}

		first_heartbeat();
	}

	virtual void destory() {
	
	}

	void first_heartbeat() {
		for (auto moduel : moduels) {
			auto component = moduel.instance();
			//component->init();
			Params p;
			component->in(Gebo::FIRST_HEARTBEAT, p);
		}
	}
};

int main() {
	auto launcher = new Launcher;
	launcher->init();
	getchar();
    return 0;
}
