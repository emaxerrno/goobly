#include <iostream>
#include <grpc++/grpc++.h>
#include "goobly.grpc.pb.h"
int main(int args, char *argv[], char *env[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  std::cout << "Hello world" << std::endl;

}
