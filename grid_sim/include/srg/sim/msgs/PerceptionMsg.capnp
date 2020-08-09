@0xf338162d11fbd927;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("srg::sim");
using IDMsg = import "/capnzero/ID.capnp";

struct PerceptionMsg {
  receiverID @0 :IDMsg.ID;
  cellPerceptions @1 :List(CellPerception);
  timestamp @2: Int64;

  struct CellPerception {
      x @0: UInt32;
      y @1: UInt32;
      objects @2 :List(Object);
      time @3 :Int64;
  }

  struct Object {
    type @0: Type;
    state @1: State;
    id @2 :IDMsg.ID;
    objects @3 :List(Object);

    enum Type {
      door @0;
      cupred @1;
      cupblue @2;
      cupyellow @3;
      robot @4;
      human @5;
      unknown @6;
    }

    enum State {
      open @0;
      closed @1;
      undefined @2;
    }
  }
}