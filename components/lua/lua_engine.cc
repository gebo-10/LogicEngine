#include <stdarg.h>
#include <iostream>
#include "lua_engine.h"


inline void my_panic(sol::optional<std::string> maybe_msg) {
  std::cout << "Lua is in a panic state and will now abort() the application"
            << std::endl;
  if (maybe_msg) {
    const std::string& msg = maybe_msg.value();
    std::cout << "\terror message: " << msg << std::endl;
  }
}

LuaEngine::LuaEngine(): sol(sol::c_call<decltype(&my_panic), &my_panic>) {}

LuaEngine::~LuaEngine() {

}

void LuaEngine::init() {
  sol.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math,
                     sol::lib::table);
  //register_model(sol);

  std::string path = ".\\";
  search_path = path; //+ "\\assets\\scripts\\lua\\";
  std::string package_path = sol["package"]["path"];
  sol["package"]["path"] = package_path + ";" + search_path + "?.lua";

  //sol.script_file(search_path + "main.lua");
  //on_event = sol["OnEvent"];
  //on_event.error_handler = sol["OnError"];


  return;
}

//void LuaEngine::reload() {
//  sol::protected_function lua_reload = sol["Reload"];
//  lua_reload.error_handler = sol["OnError"];
//  lua_reload();
//}
//
//bool LuaEngine::start() {
//  sol::protected_function lua_start = sol["Start"];
//  lua_start.error_handler = sol["OnError"];
//  lua_start();
//  runing = true;
//  return true;
//}
//
//void LuaEngine::update() {
//  if (!runing) {
//    return;
//  }
//  lua_update;
//}

void LuaEngine::destory() {
  //sol::protected_function lua_stop = sol["Stop"];
  //lua_stop.error_handler = sol["OnError"];
  //lua_stop();
}
