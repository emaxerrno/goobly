// testing utils
#include <glog/logging.h>
// lua
#include <lua5.2/lua.hpp>
#include <lua5.2/lauxlib.h>


// TEST(lua_simple, load_variable_from_file) {
//   lua_State* state = luaL_newstate();
//   //EXPECT_EQ(1,1);
//   lua_close(state);
// }

int main(int argc, char *argv[], char* env[]) {
  ::google::InstallFailureSignalHandler();
  ::google::InitGoogleLogging(argv[0]);

  // ::testing::InitGoogleTest(&argc, argv);
  // return RUN_ALL_TESTS();

  LOG(INFO) << "Initializing state";
  lua_State* state = luaL_newstate();
  luaL_openlibs(state);
  luaL_dofile(state, "test.lua");
  lua_getglobal(state, "x");
  auto xlua = lua_tonumber(state, -1);
  LOG(INFO) << "Got number from lua: " << xlua;
  lua_close(state);
}
