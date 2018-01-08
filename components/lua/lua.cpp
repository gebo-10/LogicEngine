#include"component.h"
#include "gebo_lua.h"
#include"lua_engine.h"

class LuaComp:public Component
{
private:
    LuaEngine lua_engine;
public:
    LuaComp() {
        register_call<std::string *, std::any *, std::vector<Contain>* >(Gebo::Lua::RUN_FUNCTION, [this](std::string * fun, std::any * resualt, std::vector<Contain>* params) {
            sol::protected_function lua_fun = lua_engine.sol[*fun];
            //lua_fun.error_handler = lua_engine.sol["OnError"];
            sol::table lua_table = lua_engine.sol.create_table_with();
            for (auto item : *params) {
                switch (item.type())
                {
                case INT:
                    lua_table.add(std::any_cast<int>(item.data()));
                    break;
                case FLOAT:
                    lua_table.add(std::any_cast<float>(item.data()));
                    break;
                case DOUBLE:
                    lua_table.add(std::any_cast<double>(item.data()));
                    break;
                case STRING:
                    lua_table.add(std::any_cast<std::string>(item.data()));
                    break;
                case PTR:
                    lua_table.add(std::any_cast<void *>(item.data()));
                    break;
                default:
                    break;
                }
            }
            *resualt=lua_fun(lua_table);
        });

        register_call<std::string *>(Gebo::Lua::RUN_FILE, [this](std::string * file) {
            lua_engine.sol.script_file(lua_engine.search_path + file->c_str());
        });

        register_call<std::string *, Contain *>(Gebo::Lua::GET_VALUE, [this](std::string * file, Contain * resualt) {
            Result = lua_engine.get_value();
        });
    }

    ~LuaComp()
    {
    }

    void run_file(std::string * file) {
        lua_engine.sol.script_file(lua_engine.search_path + file->c_str());
    }

};
EXPORT_COMPONENT(LuaComp)