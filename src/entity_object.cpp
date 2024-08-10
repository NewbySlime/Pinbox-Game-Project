#include "entity_object.h"
#include "entity_server.h"

#include "godot_cpp/classes/engine.hpp"


using namespace Game::Entity;
using namespace GameUtils::DataStorage;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(entity_object)
ATTR_BASE_INTERFACE_HELPER_DEFINE(entity_object)
ATTR_INTERFACE_HELPER_DEFINE(entity_object)


void entity_object::_on_child_entered(Node* child){
  uint64_t _child_id = child->get_instance_id();
  if(child->is_class(Node2D::get_class_static()))
    _space2d_child_list[_child_id] = (Node2D*)child;
  else if(child->is_class(Node3D::get_class_static()))
    _space3d_child_list[_child_id] = (Node3D*)child;
}

void entity_object::_on_child_exited(Node* child){
  auto _iter2 = _space2d_child_list.find(child->get_instance_id());
  if(_iter2 != _space2d_child_list.end())
    _space2d_child_list.erase(_iter2);

  auto _iter3 = _space3d_child_list.find(child->get_instance_id());
  if(_iter3 != _space3d_child_list.end())
    _space3d_child_list.erase(_iter3);
}


void entity_object::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(entity_object)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(entity_object)
  ATTR_INTERFACE_HELPER_BIND_GODOT(entity_object)

  ClassDB::bind_method(
    D_METHOD("_on_child_entered", "child"),
    &entity_object::_on_child_entered
  );

  ClassDB::bind_method(
    D_METHOD("_on_child_exited", "child"),
    &entity_object::_on_child_exited
  );
}


void entity_object::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(space2d_pivot_point.is_empty()){
      CREATE_GET_NODE_TESTER(_space2d_point, space2d_pivot_point, Node2D)
    }
    if(space3d_pivot_point.is_empty()){
      CREATE_GET_NODE_TESTER(_space3d_point, space3d_pivot_point, Node3D)
    }

    connect("child_entered_tree", Callable(this, "_on_child_entered"));
    connect("child_exited_tree", Callable(this, "_on_child_exited"));
  }
}


entity_object::entity_object(){
  _this_server = NULL;

  _space2d_point = NULL;
  _space3d_point = NULL;
}


void entity_object::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(_space2d_point){
      Vector2 _pos = _space2d_point->get_global_position();
      for(auto _pair: _space2d_child_list)
        _pair.second->set_global_position(_pos);
    }

    if(_space3d_point){
      Vector3 _pos = _space3d_point->get_global_position();
      for(auto _pair: _space3d_child_list)
        _pair.second->set_global_position(_pos);
    }
  }
}

 
void entity_object::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)

    switch(code){
      break; case NOTIFICATION_PREDELETE:{
        emit_signal(SIGNAL_ENTITY_OBJECT_ON_DESPAWNED);
      }
    }
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      if(_this_server)
        _this_server->_on_entity_deinstantiated(this);
    }
  }
}


Node3D* entity_object::get_space3d_pivot_node() const{
  return _space3d_point;
}

Node2D* entity_object::get_space2d_pivot_node() const{
  return _space2d_point;
}


entity_inst_id entity_object::get_instance_id() const{
  return _inst_id;
}

const entity_data* entity_object::get_entity_data() const{
  return _this_data;
}

const entity_resource_data* entity_object::get_entity_resource_data() const{
  return _this_res_data;
}

const object_subdata* entity_object::get_entity_subdata() const{
  return _entity_subdata;
}