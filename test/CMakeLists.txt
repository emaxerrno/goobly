set (TEST_ROOT ${PROJECT_SOURCE_DIR}/test)
set (GBENCH_LIB_PATH ${PROJECT_SOURCE_DIR}/meta/tmp/google_benchmark)

function (register_test NAME DIR)
  # add the test w/ cmake & ctest
  add_test (
    NAME ${NAME}_runner
    COMMAND ${PROJECT_SOURCE_DIR}/test/run_test.sh ${DIR} $<TARGET_FILE:${NAME}>
    )
endfunction (register_test)

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

add_executable (lua_simple
  ${PROJECT_SOURCE_DIR}/test/lua_simple/main.cc
  )
target_link_libraries (lua_simple
  :${PROJECT_SOURCE_DIR}/meta/tmp/googletest/build/googlemock/gtest/libgtest.a
  :liblua5.2.so.0.0.0
  :libglog.so.0
  :libdl.so.2       #dyn loading
  :libpthread.so.0
  )

register_test(lua_simple ${TEST_ROOT}/lua_simple)
