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
set (CMAKE_CXX_COMPILER g++-5)
set (CMAKE_C_COMPILER gcc-5)
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


set (LIBFOLLY
  :libboost_filesystem.so.1.55.0
  :libboost_system.so.1.55.0
  :libboost_regex.so.1.55.0
  :libpthread.so.0
  :liblzma.so.5
  :libz.so.1
  :libsnappy.so.1
  :liblz4.so.1
  :libjemalloc.so.1
  :libevent-2.0.so.5
  :libdouble-conversion.so.1
  :libssl.so.1.0.0
  :libgflags.so.2
  :libglog.so.0
  :libstdc++.so.6   #g++'s runtime
  :libm.so.6        #math for libc
  :libc.so.6        #c lib
  :libgcc_s.so.1    #gcc's runtime
  :libicuuc.so.52   #unicode
  :libicui18n.so.52 #int'lization
  :libdl.so.2       #dyn loading
  :libcrypto.so.1.0.0
  :libunwind.so.8   #stack unwind
  :libicudata.so.52 #not sure
  )

set (LIBGRPC
  :libgrpc++.so.0.12.0.0
  :libgrpc.so.0.12.0.0
  :libgpr.so.0.12.0.0
  :libprotobuf.a
  :libssl.so.1.0.0
  )

add_executable (gbly
  ${PROJECT_SOURCE_DIR}/src/goobly_db/main.cc
  ${PROJECT_SOURCE_DIR}/proto/common.pb.cc
  ${PROJECT_SOURCE_DIR}/proto/consensus.pb.cc
  ${PROJECT_SOURCE_DIR}/proto/goobly.pb.cc

  ${PROJECT_SOURCE_DIR}/proto/common.grpc.pb.cc
  ${PROJECT_SOURCE_DIR}/proto/consensus.grpc.pb.cc
  ${PROJECT_SOURCE_DIR}/proto/goobly.grpc.pb.cc
  )
target_link_libraries (gbly ${LIBFOLLY} ${LIBGRPC})


#add_subdirectory (test)
