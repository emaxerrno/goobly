set (TEST_ROOT ${PROJECT_SOURCE_DIR}/test)
set (GBENCH_LIB_PATH ${PROJECT_SOURCE_DIR}/meta/tmp/google_benchmark)

function (register_test NAME DIR)
  # add the test w/ cmake & ctest
  add_test (
    NAME ${NAME}_runner
    COMMAND ${PROJECT_SOURCE_DIR}/test/run_test.sh ${DIR} $<TARGET_FILE:${NAME}>
    )
endfunction (register_test)

# Don't forget to add them to the run_test.sh
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

add_executable (libmemlog_simple
  ${PROJECT_SOURCE_DIR}/test/libmemlog_simple/main.cc
  )
target_link_libraries (libmemlog_simple
  :libgtest.a
  :libpmem.so.1.0.0
  :libpmemlog.so.1.0.0
  :libglog.so.0
  :libdl.so.2       #dyn loading
  :libpthread.so.0
  )
register_test(libmemlog_simple ${TEST_ROOT}/libmemlog_simple)
