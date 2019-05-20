// Generated by Cap'n Proto compiler, DO NOT EDIT
// source: Command.capnp

#ifndef CAPNP_INCLUDED_cbaac9bc054f0dc5_
#define CAPNP_INCLUDED_cbaac9bc054f0dc5_

#include <capnp/generated-header-support.h>

#if CAPNP_VERSION != 6001
#error "Version mismatch between generated code and library headers.  You must use the same version of the Cap'n Proto compiler and library."
#endif


namespace capnp {
namespace schemas {

CAPNP_DECLARE_SCHEMA(b0b58100e8a72c7f);
CAPNP_DECLARE_SCHEMA(da8910b893745a9e);
enum class Action_da8910b893745a9e: uint16_t {
  SPAWN,
  GOLEFT,
  GORIGHT,
  GOUP,
  GODOWN,
  PICKUP,
  PUTDOWN,
  OPEN,
  CLOSE,
};
CAPNP_DECLARE_ENUM(Action, da8910b893745a9e);

}  // namespace schemas
}  // namespace capnp

namespace srgsim {

struct Command {
  Command() = delete;

  class Reader;
  class Builder;
  class Pipeline;
  typedef ::capnp::schemas::Action_da8910b893745a9e Action;


  struct _capnpPrivate {
    CAPNP_DECLARE_STRUCT_HEADER(b0b58100e8a72c7f, 2, 1)
    #if !CAPNP_LITE
    static constexpr ::capnp::_::RawBrandedSchema const* brand() { return &schema->defaultBrand; }
    #endif  // !CAPNP_LITE
  };
};

// =======================================================================================

class Command::Reader {
public:
  typedef Command Reads;

  Reader() = default;
  inline explicit Reader(::capnp::_::StructReader base): _reader(base) {}

  inline ::capnp::MessageSize totalSize() const {
    return _reader.totalSize().asPublic();
  }

#if !CAPNP_LITE
  inline ::kj::StringTree toString() const {
    return ::capnp::_::structString(_reader, *_capnpPrivate::brand());
  }
#endif  // !CAPNP_LITE

  inline bool hasSenderId() const;
  inline  ::capnp::Data::Reader getSenderId() const;

  inline  ::srgsim::Command::Action getAction() const;

  inline  ::uint32_t getX() const;

  inline  ::uint32_t getY() const;

private:
  ::capnp::_::StructReader _reader;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::List;
  friend class ::capnp::MessageBuilder;
  friend class ::capnp::Orphanage;
};

class Command::Builder {
public:
  typedef Command Builds;

  Builder() = delete;  // Deleted to discourage incorrect usage.
                       // You can explicitly initialize to nullptr instead.
  inline Builder(decltype(nullptr)) {}
  inline explicit Builder(::capnp::_::StructBuilder base): _builder(base) {}
  inline operator Reader() const { return Reader(_builder.asReader()); }
  inline Reader asReader() const { return *this; }

  inline ::capnp::MessageSize totalSize() const { return asReader().totalSize(); }
#if !CAPNP_LITE
  inline ::kj::StringTree toString() const { return asReader().toString(); }
#endif  // !CAPNP_LITE

  inline bool hasSenderId();
  inline  ::capnp::Data::Builder getSenderId();
  inline void setSenderId( ::capnp::Data::Reader value);
  inline  ::capnp::Data::Builder initSenderId(unsigned int size);
  inline void adoptSenderId(::capnp::Orphan< ::capnp::Data>&& value);
  inline ::capnp::Orphan< ::capnp::Data> disownSenderId();

  inline  ::srgsim::Command::Action getAction();
  inline void setAction( ::srgsim::Command::Action value);

  inline  ::uint32_t getX();
  inline void setX( ::uint32_t value);

  inline  ::uint32_t getY();
  inline void setY( ::uint32_t value);

private:
  ::capnp::_::StructBuilder _builder;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  friend class ::capnp::Orphanage;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
};

#if !CAPNP_LITE
class Command::Pipeline {
public:
  typedef Command Pipelines;

  inline Pipeline(decltype(nullptr)): _typeless(nullptr) {}
  inline explicit Pipeline(::capnp::AnyPointer::Pipeline&& typeless)
      : _typeless(kj::mv(typeless)) {}

private:
  ::capnp::AnyPointer::Pipeline _typeless;
  friend class ::capnp::PipelineHook;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
};
#endif  // !CAPNP_LITE

// =======================================================================================

inline bool Command::Reader::hasSenderId() const {
  return !_reader.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS).isNull();
}
inline bool Command::Builder::hasSenderId() {
  return !_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS).isNull();
}
inline  ::capnp::Data::Reader Command::Reader::getSenderId() const {
  return ::capnp::_::PointerHelpers< ::capnp::Data>::get(_reader.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS));
}
inline  ::capnp::Data::Builder Command::Builder::getSenderId() {
  return ::capnp::_::PointerHelpers< ::capnp::Data>::get(_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS));
}
inline void Command::Builder::setSenderId( ::capnp::Data::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::Data>::set(_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS), value);
}
inline  ::capnp::Data::Builder Command::Builder::initSenderId(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::Data>::init(_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS), size);
}
inline void Command::Builder::adoptSenderId(
    ::capnp::Orphan< ::capnp::Data>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::Data>::adopt(_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::Data> Command::Builder::disownSenderId() {
  return ::capnp::_::PointerHelpers< ::capnp::Data>::disown(_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS));
}

inline  ::srgsim::Command::Action Command::Reader::getAction() const {
  return _reader.getDataField< ::srgsim::Command::Action>(
      ::capnp::bounded<0>() * ::capnp::ELEMENTS);
}

inline  ::srgsim::Command::Action Command::Builder::getAction() {
  return _builder.getDataField< ::srgsim::Command::Action>(
      ::capnp::bounded<0>() * ::capnp::ELEMENTS);
}
inline void Command::Builder::setAction( ::srgsim::Command::Action value) {
  _builder.setDataField< ::srgsim::Command::Action>(
      ::capnp::bounded<0>() * ::capnp::ELEMENTS, value);
}

inline  ::uint32_t Command::Reader::getX() const {
  return _reader.getDataField< ::uint32_t>(
      ::capnp::bounded<1>() * ::capnp::ELEMENTS);
}

inline  ::uint32_t Command::Builder::getX() {
  return _builder.getDataField< ::uint32_t>(
      ::capnp::bounded<1>() * ::capnp::ELEMENTS);
}
inline void Command::Builder::setX( ::uint32_t value) {
  _builder.setDataField< ::uint32_t>(
      ::capnp::bounded<1>() * ::capnp::ELEMENTS, value);
}

inline  ::uint32_t Command::Reader::getY() const {
  return _reader.getDataField< ::uint32_t>(
      ::capnp::bounded<2>() * ::capnp::ELEMENTS);
}

inline  ::uint32_t Command::Builder::getY() {
  return _builder.getDataField< ::uint32_t>(
      ::capnp::bounded<2>() * ::capnp::ELEMENTS);
}
inline void Command::Builder::setY( ::uint32_t value) {
  _builder.setDataField< ::uint32_t>(
      ::capnp::bounded<2>() * ::capnp::ELEMENTS, value);
}

}  // namespace

#endif  // CAPNP_INCLUDED_cbaac9bc054f0dc5_
