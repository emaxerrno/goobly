#pragma once
#include <gflags/gflags.h>
#include <grpc++/grpc++.h>

#include "goobly.grpc.pb.h"
DEFINE_uint16(port, 11222, "Default port for raft service");
namespace goobly {
class AsyncRaftServer {
public:
  void foo() {
  // CallData(Greeter::AsyncService *service, ServerCompletionQueue *cq)
  // PutReply put(const PutRequest& put){
    auto id = ::folly::hash::fnv32_buf(put.table());
    if(dispatcher_->find(id) == dispatcher_.end()){
      dispatcher_->insert({id, queue_t(1024)});
    }
    auto & q = dispatcher_->find(id);
    while (!q.write(put)) {
      //spin until we get a value
      continue;
    }
  }
private:
  using queue_t = folly::ProducerConsumerQueue<PutRequest>;
  using ahm_t =  AtomicHashMap<int64_t, queue_t>;

  ahm_t dispatcher_(1024);
  // 1. IoThreadPool
};
}
