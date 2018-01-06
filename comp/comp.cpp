#include"component.h"

class Comp:public Component
{
public:
    Comp()
    {
        register_call<int,float>(1, [this](int a, float b)->void {std::cout << a + b; } );
    }

    ~Comp()
    {
    }

private:

};
#define DLLEXPORT extern "C" _declspec(dllexport)
DLLEXPORT  Component *create() {
    return new Comp;
}
DLLEXPORT  Component *instance() {
    static auto ins= new Comp;
    return ins;
}