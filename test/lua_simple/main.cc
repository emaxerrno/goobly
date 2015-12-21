// testing utils
#include <glog/logging.h>
#include <gtest/gtest.h>
// lua
#include <lua5.2/lua.hpp>
#include <lua5.2/lauxlib.h>


TEST(lua_simple, load_variable_from_file) {
  lua_State* state = luaL_newstate();
  luaL_openlibs(state);
  luaL_dofile(state, "test.lua");
  lua_getglobal(state, "x");
  int xlua = lua_tonumber(state, -1);
  lua_close(state);
  EXPECT_EQ(xlua, 5);
}

int main(int argc, char *argv[], char* env[]) {
  ::google::InstallFailureSignalHandler();
  ::google::InitGoogleLogging(argv[0]);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
