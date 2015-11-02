#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <grpc++/grpc++.h>

#include "goobly.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using goobly::HelloRequest;
using goobly::HelloReply;
using goobly::Greeter;

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext *context,
                  const HelloRequest *request,
                  HelloReply *reply) override {
    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50052");
  GreeterServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
class GreeterClient {
  public:
  GreeterClient(std::shared_ptr<Channel> channel)
    : stub_(Greeter::NewStub(channel)) {}

  // Assambles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string &user) {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SayHello(&context, request, &reply);

    // Act upon its status.
    if(status.ok()) {
      return reply.message();
    } else {
      return "RPC failed";
    }
  }

  private:
  std::unique_ptr<Greeter::Stub> stub_;
};


#include <benchmark/benchmark.h>
#include <glog/logging.h>

static void BM_sync_client_requests(benchmark::State &state) {
  std::thread([] { RunServer(); }).detach();
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50052). We indicate that the channel isn't authenticated
  // (use of InsecureCredentials()).
  GreeterClient greeter(
    grpc::CreateChannel("localhost:50052", grpc::InsecureCredentials()));
  std::string user("world");
  while(state.KeepRunning()) {
    // discard the reply
    greeter.SayHello(user);
  }
}
// Register the function as a benchmark
BENCHMARK(BM_sync_client_requests);

int main(int argc, char **argv) {
  google::InstallFailureSignalHandler();
  google::InitGoogleLogging(argv[0]);
  benchmark::Initialize(&argc, (const char **)argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}
