@0xcbaac9bc054f0dc5;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("srg::sim");
using IDMsg = import "/capnzero/ID.capnp";

struct SimCommandMsg {
  senderID @0 :IDMsg.ID;
  action @1 :Action;
  objectID @2 :IDMsg.ID;
  x @3: UInt32;
  y @4: UInt32;

  enum Action {
      spawnrobot @0;
      spawnhuman @1;
      goleft @2;
      goright @3;
      goup @4;
      godown @5;
      pickup @6;
      putdown @7;
      open @8;
      close @9;
  }
}