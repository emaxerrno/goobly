cmake_minimum_required (VERSION 2.6)
set(GOOBLY_VERSION_NUMBER "0.0.1")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

enable_testing ()


# libgflags-dev libgtest-dev
find_package (GTest REQUIRED)

set (PROTOBUF_LIB_PATH ${PROJECT_SOURCE_DIR}/meta/tmp/grpc/libs/opt)
set (PROTOBUF_3RD_PARTY_LIB_PATH ${PROJECT_SOURCE_DIR}/meta/tmp/grpc/third_party)

link_directories (
  ${PROTOBUF_LIB_PATH}
  ${PROTOBUF_LIB_PATH}/protobuf
  ${PROTOBUF_LIB_PATH}/openssl
  /usr/local/lib # must be last if locals aren't used
  )

include_directories (
  ${PROJECT_SOURCE_DIR}/meta/tmp/grpc/include
  ${PROJECT_SOURCE_DIR}/meta/tmp/grpc/third_party/protobuf/src
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/proto
  )

set (PROTO_SOURCES
  ${PROJECT_SOURCE_DIR}/proto/common.pb.cc
  ${PROJECT_SOURCE_DIR}/proto/common.pb.h
  ${PROJECT_SOURCE_DIR}/proto/common.proto
  ${PROJECT_SOURCE_DIR}/proto/consensus.pb.cc
  ${PROJECT_SOURCE_DIR}/proto/consensus.pb.h
  ${PROJECT_SOURCE_DIR}/proto/consensus.proto
  ${PROJECT_SOURCE_DIR}/proto/goobly.pb.cc
  ${PROJECT_SOURCE_DIR}/proto/goobly.pb.h
  ${PROJECT_SOURCE_DIR}/proto/goobly.pb.cc
  )

set (PROTO_GRPC_SOURCES
  ${PROJECT_SOURCE_DIR}/proto/common.grpc.pb.cc
  ${PROJECT_SOURCE_DIR}/proto/common.grpc.pb.h
  ${PROJECT_SOURCE_DIR}/proto/consensus.grpc.pb.cc
  ${PROJECT_SOURCE_DIR}/proto/consensus.grpc.pb.h
  ${PROJECT_SOURCE_DIR}/proto/goobly.grpc.pb.cc
  ${PROJECT_SOURCE_DIR}/proto/goobly.grpc.pb.h
  )

set (LIB_GOOBLY_SOURCES
  )

function (join VALUES SEP OUTPUT)
  string (REPLACE ";" "${SEP}" _TMP_STR "${VALUES}")
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction ()


set(BASE_FLAGS
  "-fPIC"
  "-fuse-ld=gold"
  "-Wall"
  "-Werror"
  "-Wextra"
  "-Wformat"
  "-pedantic"
  "-Wmissing-braces"
  "-Wparentheses"
  "-Wpointer-arith"
  "-Wformat-security"
  "-Wunused"
  "-Wno-unused-parameter"
  "-Wcast-align"
  "-Wmissing-field-initializers"
  "-o2"
  "-ggdb"
  )

set (CXX_FLAG_LIST
  ${BASE_FLAGS}
  "-std=c++1y"
  "-ftemplate-depth-256"
  "-Woverloaded-virtual"
  "-Wsign-promo"
  "-Wreorder"
  )

# set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
# set(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -v" )

join ("${CXX_FLAG_LIST}" " " CMAKE_CXX_FLAGS)
join ("${BASE_FLAGS}" " " CMAKE_C_FLAGS)
set (CMAKE_CXX_COMPILER clang++-3.6)
set (CMAKE_C_COMPILER clang-3.6)
set (PROTOC ${PROJECT_SOURCE_DIR}/meta/tmp/grpc/bins/opt/protobuf/protoc)
set (PROTOC_GRPC_PLUGINS_PATH ${PROJECT_SOURCE_DIR}/meta/tmp/grpc/bins/opt)

add_custom_command (
  OUTPUT ${PROTO_SOURCES} ${PROTO_GRPC_SOURCES}
  DEPENDS
  ${PROJECT_SOURCE_DIR}/proto/goobly.proto
  ${PROJECT_SOURCE_DIR}/proto/common.proto
  ${PROJECT_SOURCE_DIR}/proto/consensus.proto

  COMMAND ${PROTOC} -I${PROJECT_SOURCE_DIR}/proto --cpp_out=${PROJECT_SOURCE_DIR}/proto ${PROJECT_SOURCE_DIR}/proto/common.proto
  COMMAND ${PROTOC} -I${PROJECT_SOURCE_DIR}/proto --plugin=protoc-gen-grpc=${PROTOC_GRPC_PLUGINS_PATH}/grpc_cpp_plugin --grpc_out=${PROJECT_SOURCE_DIR}/proto ${PROJECT_SOURCE_DIR}/proto/common.proto

  COMMAND ${PROTOC} -I${PROJECT_SOURCE_DIR}/proto --cpp_out=${PROJECT_SOURCE_DIR}/proto ${PROJECT_SOURCE_DIR}/proto/goobly.proto
  COMMAND ${PROTOC} -I${PROJECT_SOURCE_DIR}/proto --plugin=protoc-gen-grpc=${PROTOC_GRPC_PLUGINS_PATH}/grpc_cpp_plugin --grpc_out=${PROJECT_SOURCE_DIR}/proto ${PROJECT_SOURCE_DIR}/proto/goobly.proto

  COMMAND ${PROTOC} -I${PROJECT_SOURCE_DIR}/proto --cpp_out=${PROJECT_SOURCE_DIR}/proto ${PROJECT_SOURCE_DIR}/proto/consensus.proto
  COMMAND ${PROTOC} -I${PROJECT_SOURCE_DIR}/proto --plugin=protoc-gen-grpc=${PROTOC_GRPC_PLUGINS_PATH}/grpc_cpp_plugin --grpc_out=${PROJECT_SOURCE_DIR}/proto ${PROJECT_SOURCE_DIR}/proto/consensus.proto
  )



set (GOOBLY_STATIC_LIB_DEPS
  :libgrpc++_unsecure.a
  :libgrpc.a
  :libgpr.a
  :libprotobuf.a
  :libssl.a
  :libcrypto.a
  )

set (GOOBLY_DYN_LIBS_DEPS
  :libjemalloc.so.1 # TODO(agallego) - add it to the apt.yml repo
  :libglog.so.0.0.0
  z
  pthread
  rt
  dl
  )

set (GOOBLY_LIBRARIES
  ${GOOBLY_DYN_LIBS_DEPS}
  )
add_library (goobly SHARED
  ${LIB_GOOBLY_SOURCES}
  )
target_link_libraries(goobly ${GOOBLY_STATIC_LIB_DEPS})

add_executable (goobly_db
  ${PROJECT_SOURCE_DIR}/src/goobly_db/main.cc
  )
target_link_libraries (goobly_db ${GOOBLY_DYN_LIBS_DEPS} goobly)


add_subdirectory (test)
