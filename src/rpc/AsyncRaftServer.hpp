#pragma once
#include <gflags/gflags.h>
#include <grpc++/grpc++.h>

#include "goobly.grpc.pb.h"
DEFINE_uint16(port, 11222, "Default port for raft service");
namespace goobly {
class AsyncRaftServer {};
}
