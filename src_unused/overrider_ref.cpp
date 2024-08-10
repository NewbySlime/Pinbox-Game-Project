#include "overrider_ref.h"


using namespace Game;
using namespace Game::Overrider;
using namespace GameUtils;
using namespace godot;



void overrider_ref::_bind_methods(){
  node_holder_helper::_bind_methods<overrider_ref>();
}


bool overrider_ref::instantiate_node(){
  if(_override_node)
    return false;

  if(_overrider_pckscene.is_null())
    return false;

  Node* _new_node = _overrider_pckscene->instantiate();
  hold_location _node_location;{
    _node_location.this_node = _new_node;
    _node_location.variable_location = &_override_node;
  }

  hold_node(_node_location);
  return true;
}


bool overrider_ref::reparent_ref_to(Node* parent, bool keep_global_transform){
  if(!_override_node)
    instantiate_node();

  if(!_override_node)
    return false;
    
  _override_node->reparent(parent, keep_global_transform);
}