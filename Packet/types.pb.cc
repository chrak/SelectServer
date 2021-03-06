// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: types.proto

#if WITH_GOOGLE_PROTOBUF_BINDING == 1
#include "AllowWindowsPlatformTypes.h"
#endif // WITH_GOOGLE_PROTOBUF_BINDING

#if _MSC_VER >= 1300
#ifndef _PS3
#pragma warning(push)
#pragma warning(disable : 4530)	// warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc (disabled due to std headers having exception syntax)
#pragma warning(disable : 4512)	// warning C4512: assignment operator could not be generated
#pragma warning(disable : 4244)	// warning C4244:  warning C4244: '=' : conversion from '__w64 int' to 'int', possible loss of data
#pragma warning(disable : 4267)	// warning C4267: 'argument' : conversion from 'size_t' to 'int', possible loss of data
#pragma warning(disable : 4125)	// warning C4125: decimal digit terminates octal escape sequence
#pragma warning(disable : 4127)	// warning C4127: conditional expression is constant
#pragma warning(disable : 4100)	// warning C4100: 'op' : unreferenced formal parameter
#endif // _PS3
#endif // _MSC_VER
#if _GNUC
#ifndef _PS3
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#endif // _PS3
#endif // _GNUC

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "types.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace packets {

namespace {

const ::google::protobuf::EnumDescriptor* PacketCode_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* ResultType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_types_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_types_2eproto() {
  protobuf_AddDesc_types_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "types.proto");
  GOOGLE_CHECK(file != NULL);
  PacketCode_descriptor_ = file->enum_type(0);
  ResultType_descriptor_ = file->enum_type(1);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_types_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_types_2eproto() {
}

void protobuf_AddDesc_types_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AddDesc_types_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\013types.proto\022\007packets*L\n\nPacketCode\022\r\n\t"
    "LOGIN_REQ\020\000\022\r\n\tLOGIN_ACK\020\001\022\017\n\013MESSAGE_RE"
    "Q\020\002\022\017\n\013MESSAGE_NFY\020\003*3\n\nResultType\022\022\n\016RE"
    "SULT_SUCCESS\020\000\022\021\n\rRESULT_FAILED\020\001b\006proto"
    "3", 161);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "types.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_types_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_types_2eproto {
  StaticDescriptorInitializer_types_2eproto() {
    protobuf_AddDesc_types_2eproto();
  }
} static_descriptor_initializer_types_2eproto_;
const ::google::protobuf::EnumDescriptor* PacketCode_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PacketCode_descriptor_;
}
bool PacketCode_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* ResultType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ResultType_descriptor_;
}
bool ResultType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace packets

// @@protoc_insertion_point(global_scope)
