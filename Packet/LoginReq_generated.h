// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_LOGINREQ_PACKETS_H_
#define FLATBUFFERS_GENERATED_LOGINREQ_PACKETS_H_

#include "flatbuffers/flatbuffers.h"

namespace packets {

struct LoginReq;

struct LoginReq FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_PASS = 6
  };
  const flatbuffers::String *id() const {
    return GetPointer<const flatbuffers::String *>(VT_ID);
  }
  const flatbuffers::String *pass() const {
    return GetPointer<const flatbuffers::String *>(VT_PASS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ID) &&
           verifier.VerifyString(id()) &&
           VerifyOffset(verifier, VT_PASS) &&
           verifier.VerifyString(pass()) &&
           verifier.EndTable();
  }
};

struct LoginReqBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(flatbuffers::Offset<flatbuffers::String> id) {
    fbb_.AddOffset(LoginReq::VT_ID, id);
  }
  void add_pass(flatbuffers::Offset<flatbuffers::String> pass) {
    fbb_.AddOffset(LoginReq::VT_PASS, pass);
  }
  explicit LoginReqBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  LoginReqBuilder &operator=(const LoginReqBuilder &);
  flatbuffers::Offset<LoginReq> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<LoginReq>(end);
    return o;
  }
};

inline flatbuffers::Offset<LoginReq> CreateLoginReq(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> id = 0,
    flatbuffers::Offset<flatbuffers::String> pass = 0) {
  LoginReqBuilder builder_(_fbb);
  builder_.add_pass(pass);
  builder_.add_id(id);
  return builder_.Finish();
}

inline flatbuffers::Offset<LoginReq> CreateLoginReqDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *id = nullptr,
    const char *pass = nullptr) {
  return packets::CreateLoginReq(
      _fbb,
      id ? _fbb.CreateString(id) : 0,
      pass ? _fbb.CreateString(pass) : 0);
}

inline const packets::LoginReq *GetLoginReq(const void *buf) {
  return flatbuffers::GetRoot<packets::LoginReq>(buf);
}

inline const packets::LoginReq *GetSizePrefixedLoginReq(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<packets::LoginReq>(buf);
}

inline bool VerifyLoginReqBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<packets::LoginReq>(nullptr);
}

inline bool VerifySizePrefixedLoginReqBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<packets::LoginReq>(nullptr);
}

inline void FinishLoginReqBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<packets::LoginReq> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedLoginReqBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<packets::LoginReq> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace packets

#endif  // FLATBUFFERS_GENERATED_LOGINREQ_PACKETS_H_