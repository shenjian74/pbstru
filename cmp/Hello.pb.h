// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Hello.proto

#ifndef PROTOBUF_Hello_2eproto__INCLUDED
#define PROTOBUF_Hello_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf_Hello_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
void InitDefaultsHelloImpl();
void InitDefaultsHello();
inline void InitDefaults() {
  InitDefaultsHello();
}
}  // namespace protobuf_Hello_2eproto
class Hello;
class HelloDefaultTypeInternal;
extern HelloDefaultTypeInternal _Hello_default_instance_;

// ===================================================================

class Hello : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Hello) */ {
 public:
  Hello();
  virtual ~Hello();

  Hello(const Hello& from);

  inline Hello& operator=(const Hello& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Hello(Hello&& from) noexcept
    : Hello() {
    *this = ::std::move(from);
  }

  inline Hello& operator=(Hello&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Hello& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Hello* internal_default_instance() {
    return reinterpret_cast<const Hello*>(
               &_Hello_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(Hello* other);
  friend void swap(Hello& a, Hello& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Hello* New() const PROTOBUF_FINAL { return New(NULL); }

  Hello* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Hello& from);
  void MergeFrom(const Hello& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Hello* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string str = 3;
  bool has_str() const;
  void clear_str();
  static const int kStrFieldNumber = 3;
  const ::std::string& str() const;
  void set_str(const ::std::string& value);
  #if LANG_CXX11
  void set_str(::std::string&& value);
  #endif
  void set_str(const char* value);
  void set_str(const char* value, size_t size);
  ::std::string* mutable_str();
  ::std::string* release_str();
  void set_allocated_str(::std::string* str);

  // required int32 id = 1;
  bool has_id() const;
  void clear_id();
  static const int kIdFieldNumber = 1;
  ::google::protobuf::int32 id() const;
  void set_id(::google::protobuf::int32 value);

  // required float flt = 2;
  bool has_flt() const;
  void clear_flt();
  static const int kFltFieldNumber = 2;
  float flt() const;
  void set_flt(float value);

  // optional int32 opt = 4;
  bool has_opt() const;
  void clear_opt();
  static const int kOptFieldNumber = 4;
  ::google::protobuf::int32 opt() const;
  void set_opt(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Hello)
 private:
  void set_has_id();
  void clear_has_id();
  void set_has_flt();
  void clear_has_flt();
  void set_has_str();
  void clear_has_str();
  void set_has_opt();
  void clear_has_opt();

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr str_;
  ::google::protobuf::int32 id_;
  float flt_;
  ::google::protobuf::int32 opt_;
  friend struct ::protobuf_Hello_2eproto::TableStruct;
  friend void ::protobuf_Hello_2eproto::InitDefaultsHelloImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Hello

// required int32 id = 1;
inline bool Hello::has_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Hello::set_has_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Hello::clear_has_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Hello::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 Hello::id() const {
  // @@protoc_insertion_point(field_get:Hello.id)
  return id_;
}
inline void Hello::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:Hello.id)
}

// required float flt = 2;
inline bool Hello::has_flt() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Hello::set_has_flt() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Hello::clear_has_flt() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Hello::clear_flt() {
  flt_ = 0;
  clear_has_flt();
}
inline float Hello::flt() const {
  // @@protoc_insertion_point(field_get:Hello.flt)
  return flt_;
}
inline void Hello::set_flt(float value) {
  set_has_flt();
  flt_ = value;
  // @@protoc_insertion_point(field_set:Hello.flt)
}

// required string str = 3;
inline bool Hello::has_str() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Hello::set_has_str() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Hello::clear_has_str() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Hello::clear_str() {
  str_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_str();
}
inline const ::std::string& Hello::str() const {
  // @@protoc_insertion_point(field_get:Hello.str)
  return str_.GetNoArena();
}
inline void Hello::set_str(const ::std::string& value) {
  set_has_str();
  str_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Hello.str)
}
#if LANG_CXX11
inline void Hello::set_str(::std::string&& value) {
  set_has_str();
  str_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Hello.str)
}
#endif
inline void Hello::set_str(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_str();
  str_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Hello.str)
}
inline void Hello::set_str(const char* value, size_t size) {
  set_has_str();
  str_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Hello.str)
}
inline ::std::string* Hello::mutable_str() {
  set_has_str();
  // @@protoc_insertion_point(field_mutable:Hello.str)
  return str_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Hello::release_str() {
  // @@protoc_insertion_point(field_release:Hello.str)
  clear_has_str();
  return str_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Hello::set_allocated_str(::std::string* str) {
  if (str != NULL) {
    set_has_str();
  } else {
    clear_has_str();
  }
  str_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), str);
  // @@protoc_insertion_point(field_set_allocated:Hello.str)
}

// optional int32 opt = 4;
inline bool Hello::has_opt() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Hello::set_has_opt() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Hello::clear_has_opt() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Hello::clear_opt() {
  opt_ = 0;
  clear_has_opt();
}
inline ::google::protobuf::int32 Hello::opt() const {
  // @@protoc_insertion_point(field_get:Hello.opt)
  return opt_;
}
inline void Hello::set_opt(::google::protobuf::int32 value) {
  set_has_opt();
  opt_ = value;
  // @@protoc_insertion_point(field_set:Hello.opt)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Hello_2eproto__INCLUDED