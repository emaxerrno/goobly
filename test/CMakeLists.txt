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

set (GOOBLY_TEST_LIBRARIES
  :libgtest.a
  :libgmock.a
  :libbenchmark.a
  )

set (GOOBLY_TESTS "")

function (register_test NAME DIR SOURCE)
  set (GOOBLY_TESTS ${GOOBLY_TESTS} ${NAME} PARENT_SCOPE)
  set (TEST_EXECUTABLE_SOURCES
    ${DIR}/${SOURCE}
    )
  add_executable (${NAME} ${TEST_EXECUTABLE_SOURCES})
  target_link_libraries (${NAME} ${GOOBLY_TEST_LIBRARIES})
  add_test (
    NAME ${NAME}_runner
    COMMAND ${PROJECT_SOURCE_DIR}/test/run_test.sh $<TARGET_FILE:${NAME}> ${DIR}
    )
endfunction ()

set (TEST_ROOT ${PROJECT_SOURCE_DIR}/test)

# register_test (simple_rpc_test
#   ${TEST_ROOT}/simple_rpc_test
#   main.cc)

# register_test (async_rpc_test
#   ${TEST_ROOT}/async_rpc_test
#   main.cc)

add_custom_target (check
  COMMAND ctest --output-on-failure -j5
  DEPENDS ${GOOBLY_TESTS})
