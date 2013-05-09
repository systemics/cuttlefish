#include <map>
#include "lua.h"
#include "lua.hpp"
#include <assert.h>
using namespace std;

int main() {
  lua_State *L = luaL_newstate();
  if (L == NULL) {
    printf("FAIL: no valid lua state!\n");
    return 1;
  }

  luaL_openlibs(L);

  if (0 == luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON)) {
    printf("FAIL: could not turn on jit\n");
    return 1;
  }
}
