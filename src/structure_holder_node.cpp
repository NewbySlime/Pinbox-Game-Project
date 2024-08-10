#include "structure_holder_node.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/typed_array.hpp"


using namespace GameUtils::Helper;
using namespace godot;


void structure_holder_node::_bind_methods(){
  GD_PROPERTY_BIND(structure_holder_node, Variant::BOOL, reparent_retain_global_position);
}


structure_holder_node::structure_holder_node(){
  reparent_retain_global_position = false;
}


void structure_holder_node::_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    Node* _parent_node = get_parent();

    int _child_count = get_child_count();
    for(int i = 0; i < _child_count; i++){
      Node* _child = get_child(i);
      _child->reparent(_parent_node, reparent_retain_global_position);
    }

    queue_free();
  }
}