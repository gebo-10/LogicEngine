#ifndef LUAENGINE_IMPL_H_
#define LUAENGINE_IMPL_H_

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
#include <string>

#define SOL_USING_CXX_LUA
#define SOL_NO_EXCEPTIONS
#include "sol.hpp"
class LuaEngine{
  public:
  sol::state sol;
  std::string search_path;
  LuaEngine();
  ~LuaEngine();

  void init();

  void destory();

};
#endif
