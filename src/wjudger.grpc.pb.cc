// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: wjudger.proto

#include "wjudger.pb.h"
#include "wjudger.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace WJudger {

static const char* WJudger_method_names[] = {
  "/WJudger.WJudger/Judge",
  "/WJudger.WJudger/Simple",
};

std::unique_ptr< WJudger::Stub> WJudger::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< WJudger::Stub> stub(new WJudger::Stub(channel, options));
  return stub;
}

WJudger::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_Judge_(WJudger_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_Simple_(WJudger_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::ClientReader< ::WJudger::JudgeReply>* WJudger::Stub::JudgeRaw(::grpc::ClientContext* context, const ::WJudger::JudgeArgs& request) {
  return ::grpc::internal::ClientReaderFactory< ::WJudger::JudgeReply>::Create(channel_.get(), rpcmethod_Judge_, context, request);
}

void WJudger::Stub::async::Judge(::grpc::ClientContext* context, const ::WJudger::JudgeArgs* request, ::grpc::ClientReadReactor< ::WJudger::JudgeReply>* reactor) {
  ::grpc::internal::ClientCallbackReaderFactory< ::WJudger::JudgeReply>::Create(stub_->channel_.get(), stub_->rpcmethod_Judge_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::WJudger::JudgeReply>* WJudger::Stub::AsyncJudgeRaw(::grpc::ClientContext* context, const ::WJudger::JudgeArgs& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::WJudger::JudgeReply>::Create(channel_.get(), cq, rpcmethod_Judge_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::WJudger::JudgeReply>* WJudger::Stub::PrepareAsyncJudgeRaw(::grpc::ClientContext* context, const ::WJudger::JudgeArgs& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::WJudger::JudgeReply>::Create(channel_.get(), cq, rpcmethod_Judge_, context, request, false, nullptr);
}

::grpc::Status WJudger::Stub::Simple(::grpc::ClientContext* context, const ::WJudger::SimpleArgs& request, ::WJudger::SimpleReply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::WJudger::SimpleArgs, ::WJudger::SimpleReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Simple_, context, request, response);
}

void WJudger::Stub::async::Simple(::grpc::ClientContext* context, const ::WJudger::SimpleArgs* request, ::WJudger::SimpleReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::WJudger::SimpleArgs, ::WJudger::SimpleReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Simple_, context, request, response, std::move(f));
}

void WJudger::Stub::async::Simple(::grpc::ClientContext* context, const ::WJudger::SimpleArgs* request, ::WJudger::SimpleReply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Simple_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::WJudger::SimpleReply>* WJudger::Stub::PrepareAsyncSimpleRaw(::grpc::ClientContext* context, const ::WJudger::SimpleArgs& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::WJudger::SimpleReply, ::WJudger::SimpleArgs, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Simple_, context, request);
}

::grpc::ClientAsyncResponseReader< ::WJudger::SimpleReply>* WJudger::Stub::AsyncSimpleRaw(::grpc::ClientContext* context, const ::WJudger::SimpleArgs& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSimpleRaw(context, request, cq);
  result->StartCall();
  return result;
}

WJudger::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      WJudger_method_names[0],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< WJudger::Service, ::WJudger::JudgeArgs, ::WJudger::JudgeReply>(
          [](WJudger::Service* service,
             ::grpc::ServerContext* ctx,
             const ::WJudger::JudgeArgs* req,
             ::grpc::ServerWriter<::WJudger::JudgeReply>* writer) {
               return service->Judge(ctx, req, writer);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      WJudger_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< WJudger::Service, ::WJudger::SimpleArgs, ::WJudger::SimpleReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](WJudger::Service* service,
             ::grpc::ServerContext* ctx,
             const ::WJudger::SimpleArgs* req,
             ::WJudger::SimpleReply* resp) {
               return service->Simple(ctx, req, resp);
             }, this)));
}

WJudger::Service::~Service() {
}

::grpc::Status WJudger::Service::Judge(::grpc::ServerContext* context, const ::WJudger::JudgeArgs* request, ::grpc::ServerWriter< ::WJudger::JudgeReply>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status WJudger::Service::Simple(::grpc::ServerContext* context, const ::WJudger::SimpleArgs* request, ::WJudger::SimpleReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace WJudger

