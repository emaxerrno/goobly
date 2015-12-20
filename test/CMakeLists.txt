set (GBENCH_LIB_PATH ${PROJECT_SOURCE_DIR}/meta/tmp/google_benchmark)

link_directories (
  ${GBENCH_LIB_PATH}/src
  ${PROJECT_SOURCE_DIR}/meta/tmp/googletest/build/googlemock
  ${PROJECT_SOURCE_DIR}/meta/tmp/googletest/build/googlemock/gtest
  )

include_directories (
  SYSTEM
  ${PROJECT_SOURCE_DIR}/meta/tmp/grpc/include
  ${PROJECT_SOURCE_DIR}/meta/tmp/googletest/googletest/include
  ${PROJECT_SOURCE_DIR}/meta/tmp/googletest/googlemock/include
  ${GBENCH_LIB_PATH}/include
  )

add_executable (lua_simple_test
  ${PROJECT_SOURCE_DIR}/test/lua_simple/main.cc
  )
target_link_libraries (lua_simple_test
  :liblua5.2.so.0.0.0
  :libgtest.a
  :libgmock.a
  :libglog.so.0
  :libdl.so.2       #dyn loading
  :libpthread.so.0
  )
