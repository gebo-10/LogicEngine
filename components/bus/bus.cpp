#include"component.h"
#include "gebo_bus.h"
#include<vector>
class Bus:public Component
{
public:
	std::vector<Component *> components;
	std::function<int(CallType, Params)> out_pipe;
	Bus() {
		out_pipe = std::bind(&Bus::dispatch, this, std::placeholders::_1, std::placeholders::_2);
        register_call<Component *>(Gebo::ComponentBus::REGISTER, [this](Component * component) {
			component->set_out(out_pipe);
			components.push_back(component);
        });
    }

    ~Bus(){}

	int dispatch(CallType type, Params params) {
		for (auto component: components)
		{
			component->in(type, params);
		}
		return 0;
	}

};
EXPORT_COMPONENT(Bus,Gebo::COMPONENT_BUS)