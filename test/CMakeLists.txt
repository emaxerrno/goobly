set (GOOBLY_TEST_LIBRARIES
  ${GTEST_BOTH_LIBRARIES}
  :${PROTOBUF_LIB_PATH}/libgpr.so.0.11.0.0
  :${PROTOBUF_LIB_PATH}/libgrpc++_unsecure.so.0.11.0.0
  goobly
  ${GOOBLY_LIBRARIES}
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

register_test (simple_rpc_test
  ${TEST_ROOT}/simple_rpc_test
  main.cc)

add_custom_target (check
  COMMAND ctest --output-on-failure -j5
  DEPENDS ${GOOBLY_TESTS})
