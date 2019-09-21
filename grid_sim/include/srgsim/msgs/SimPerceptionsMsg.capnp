@0xf338162d11fbd927;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("srgsim");
using IDMsg = import "/capnzero/ID.capnp";

struct SimPerceptionsMsg {
  receiverID @0 :IDMsg.ID;
  perceptions @1 :List(Perception);

  struct Perception {
      type @0: Type;
      objectID @1 :IDMsg.ID;
      x @2: UInt32;
      y @3: UInt32;

    enum Type {
      doorclosed @0;
      dooropen @1;
      cupred @2;
      cupblue @3;
      cupyellow @4;
      robot @5;
    }
  }
}