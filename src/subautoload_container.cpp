#include "godot_var_extended.h"

#include "subautoload_container.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"


using namespace Game;
using namespace godot;



void subautoload_container::_check_autoloads(){
  for(int i = 0; i < get_child_count(); i++){
    Node* _current_child = get_child(i);
    _bind_autoload(_current_child);
  }

  for(int i = 0; i < get_child_count(); i++){
    Node* _current_child = get_child(i);
    _child_call_bind(_current_child);
    _child_call_ready(_current_child);
  }
}

void subautoload_container::_bind_autoload(Node* node){
  INIT_ASSERT()
  Array _paramarr;{
    _paramarr.append(node->get_class());
    _paramarr.append((uint64_t)node);
  }

  LOG_ASSERT(String("autoload: {0} address: {1}").format(_paramarr))

  _autoload_metadata _metadata;{
    _metadata._this_node = node;
    _metadata._node_inst_id = node->get_instance_id();
  }

  _nodes_map[node->get_class()] = _metadata;
}


void subautoload_container::_child_call_bind(godot::Node* node){
  node->call(SUBAUTOLOAD_USER_BINDFUNCSTR, GDVAR_CUSTOM_FROM_PTR(this));
}

void subautoload_container::_child_call_ready(godot::Node* node){
  node->call(SUBAUTOLOAD_USER_READYFUNCSTR);
}


void subautoload_container::_check_child_recursively(Node* from, bool is_autoload_layer){
  for(int i = 0; i < from->get_child_count(); i++){
    Node* _child_node = from->get_child(i);
    if(!is_autoload_layer)
      _child_call_bind(_child_node);

    _check_child_recursively(_child_node);
  }

  if(!is_autoload_layer){
    for(int i = 0; i < from->get_child_count(); i++){
      Node* _child_node = from->get_child(i);
      _child_call_ready(_child_node);
    }
  }
}


void subautoload_container::_on_autoload_exiting_tree(Node* node){
  auto _iter = _nodes_map.find(node->get_class());
  if(_iter == _nodes_map.end() || _iter->second._node_inst_id != node->get_instance_id())
    return;

  _nodes_map.erase(_iter);
}


void subautoload_container::_on_child_added(godot::Node* child){
  
}


void subautoload_container::_bind_methods(){
  ClassDB::bind_method(
    D_METHOD("_on_autoload_exiting_tree",
      "Node"
    ),

    &subautoload_container::_on_autoload_exiting_tree
  );

  ADD_SIGNAL(MethodInfo(
    SIGNAL_SUBAUTOLOAD_ON_READY
  ));
}

void subautoload_container::_bind_node(Node* node){
  _child_call_bind(node);
  _child_call_ready(node);
}


void subautoload_container::_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _check_autoloads();
    _check_child_recursively(this);

    emit_signal(SIGNAL_SUBAUTOLOAD_ON_READY);
  }
}

void subautoload_container::add_child_node(Node* parent_node, Node* target_child_node, bool force_readable_name, InternalMode internal){
  parent_node->add_child(target_child_node, force_readable_name, internal);
  _bind_node(target_child_node);
}