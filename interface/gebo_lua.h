#include"gebo.h"
namespace Gebo::Lua {
    enum {
        START = Gebo::LUA,
        RUN_STRING,
        RUN_FILE,
        GET_VALUE,
        RUN_FUNCTION,
        END = START + 100,
    };
}