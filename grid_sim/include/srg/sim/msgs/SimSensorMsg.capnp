@0xcbaac9bc054f0dc5;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("srg::sim");
using IDMsg = import "/capnzero/ID.capnp";

struct SimSensorMsg {
  receiverID @0 :IDMsg.ID;
  action @1 :List(Perception);
  x @2: UInt32;
  y @3: UInt32;

  enum Perception {
      location @0;
      object @1;
  }
}