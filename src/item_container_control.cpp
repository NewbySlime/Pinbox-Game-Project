#include "logger.h"

#include "godot_macro.h"
#include "defines.h"

#include "item_container_control.h"

#include "godot_cpp/core/math.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/resource_loader.hpp"


using namespace Game;
using namespace Game::Item;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(item_container_control)
ATTR_BASE_INTERFACE_HELPER_DEFINE(item_container_control)
ATTR_INTERFACE_HELPER_DEFINE(item_container_control)
ATTR_PREDELETE_NOTIFIER_DEFINE(item_container_control)


void item_container_control::_item_container_on_index_changed(uint64_t _param){
  INIT_ASSERT()
  Array _paramarr;

  item_container_on_index_changed_param* param = (item_container_on_index_changed_param*)_param;
  const item_position_data* _pos_data = param->get_item_pos();

  _update_item_data(_pos_data->index_position);
}

void item_container_control::_item_container_on_container_resized(){
  _update_all_item_data();
}


void item_container_control::_item_frame_control_on_mouse_entered(int index){
  item_container_control_on_mouse_hovered_param _param;
  _param._pos_data.container_id = _bound_item_container->get_container_id();
  _param._pos_data.index_position = index;

  emit_signal(SIGNAL_ITEM_CONTAINER_CONTROL_ON_MOUSE_HOVERED, (uint64_t)&_param);
}

void item_container_control::_item_frame_control_on_mouse_exited(){
  emit_signal(SIGNAL_ITEM_CONTAINER_CONTROL_ON_MOUSE_EXITED);
}


void item_container_control::_update_container_size(){
  Vector2 _container_size = get_size();
  int new_size = _container_size.x / frame_horizontal_count;
  if(new_size > max_size)
    new_size = max_size;

  if(new_size == _frame_size)
    return;

  for(item_frame_control* _frame: _item_frames)
    _frame->set_min_size(new_size);

  _frame_size = new_size;
}


VBoxContainer* item_container_control::_create_new_vbox_cont(){
  VBoxContainer* _res = new VBoxContainer();
  _res->set_h_size_flags(Control::SIZE_EXPAND_FILL);
  _res->set_v_size_flags(Control::SIZE_EXPAND || Control::SIZE_SHRINK_CENTER);

  return _res;
}

HBoxContainer* item_container_control::_create_new_hbox_cont(){
  HBoxContainer* _res = new HBoxContainer();
  _res->set_alignment(BoxContainer::ALIGNMENT_CENTER);
  _res->set_h_size_flags(Control::SIZE_SHRINK_CENTER);

  return _res;
}


void item_container_control::_delete_vbox_cont(VBoxContainer* _v_cont){
  delete _v_cont;
}

void item_container_control::_delete_hbox_cont(HBoxContainer* _h_cont){
  delete _h_cont;
}


item_frame_control* item_container_control::_create_new_item_frame_control(HBoxContainer* parent_hbox){
  item_frame_control* _new_item_frame = (item_frame_control*)_item_frame_control_scene->instantiate();
  _new_item_frame->bind_frame_position(_item_frames.size());

  _item_frames.insert(_item_frames.end(), _new_item_frame);
  get_subautoload_root()->add_child_node(parent_hbox, _new_item_frame);

  _new_item_frame->connect(SIGNAL_ITEM_FRAME_CONTROL_ON_MOUSE_ENTERED, Callable(this, "_item_frame_control_on_mouse_entered"));
  _new_item_frame->connect(SIGNAL_ITEM_FRAME_CONTROL_ON_MOUSE_EXITED, Callable(this, "_item_frame_control_on_mouse_exited"));

  return _new_item_frame;
}

void item_container_control::_delete_item_frame_control(item_frame_control* item_frame){
  item_frame->queue_free();
}


void item_container_control::_disconnect_container_signal(Node* _container){
  if(!_container)
    return;

  _container->disconnect(SIGNAL_ITEM_CONTAINER_ON_INDEX_CHANGED, Callable(this, "_item_container_on_index_changed"));
  _container->disconnect(SIGNAL_ITEM_CONTAINER_ON_CONTAINER_RESIZED, Callable(this, "_item_container_on_container_resized"));
}


void item_container_control::_connect_container_signal(Node* _container){
  if(!_container)
    return;

  _container->connect(SIGNAL_ITEM_CONTAINER_ON_INDEX_CHANGED, Callable(this, "_item_container_on_index_changed"));
  _container->connect(SIGNAL_ITEM_CONTAINER_ON_CONTAINER_RESIZED, Callable(this, "_item_container_on_container_resized"));
}


void item_container_control::_resize_to_new_container(I_item_container* _container){
  int _new_container_size = 0;
  int _current_container_size = 0;

  if(_container){
    _new_container_size = _container->get_container_size();
  }

  if(_bound_item_container){
    _current_container_size = _bound_item_container->get_container_size();
  }

  int _last_h_count = _current_container_size % frame_horizontal_count;
  int _last_h_missing = _last_h_count != 0? (frame_horizontal_count - _last_h_count): 0;

  int _container_size_diff = _new_container_size - _current_container_size;
  if(_container_size_diff > 0){
    int _new_v_count = (int)ceil(
      (double)Math::max((_container_size_diff - _last_h_missing), 0) / frame_horizontal_count
    );

    if(_h_containers.size() > 0){
      HBoxContainer* _last_hbox = _h_containers[_h_containers.size()-1];
      for(int i = 0; i < _last_h_missing; i++){
        _create_new_item_frame_control(_last_hbox);
        _container_size_diff--;
      }
    }

    for(int i = 0; i < _new_v_count; i++){
      HBoxContainer* _new_cont = _create_new_hbox_cont();
      get_subautoload_root()->add_child_node(_v_container, _new_cont);
      for(int i = 0; i < frame_horizontal_count && _container_size_diff > 0; i++){
        _create_new_item_frame_control(_new_cont);
        _container_size_diff--;
      }
    }
  }
  else if(_container_size_diff < 0){
    _container_size_diff = abs(_container_size_diff);
    int _count = _container_size_diff-_last_h_count;
    int _remove_v_count = _count / frame_horizontal_count;
    if(_last_h_count > 0)
      _remove_v_count++;

    for(int i = 0; i < _container_size_diff; i++)
      _delete_item_frame_control(_item_frames[i]);

    for(int i = 0; i < _remove_v_count; i++)
      _delete_hbox_cont(_h_containers[i]);
  }

  _update_container_size();
}

void item_container_control::_update_item_data(int index){
  if(!_bound_item_container || index < 0 || index >= _bound_item_container->get_container_size() || index >= _item_frames.size())
    return;

  item_frame_control* _frame_control = _item_frames[index];
  uint64_t _item_id = _bound_item_container->get_item_id(index);
  if(_item_id == ITEM_ID_NULL){
    _frame_control->bind_item_object(NULL);
    return;
  }

  const item_object* _item = _item_server->get_item_object(_item_id);
  _frame_control->bind_item_object(_item);
}

void item_container_control::_update_all_item_data(){
  if(!_bound_item_container)
    return;

  for(int i = 0; i < _bound_item_container->get_container_size(); i++){
    _update_item_data(i);
  }
}


void item_container_control::_on_resized(){
  _update_container_size();
}


void item_container_control::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(item_container_control)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(item_container_control)
  ATTR_INTERFACE_HELPER_BIND_GODOT(item_container_control)
  ATTR_PREDELETE_NOTIFIER_BIND_GODOT(item_container_control)

  GD_PROPERTY_BIND(item_container_control, Variant::INT, max_size)
  GD_PROPERTY_BIND(item_container_control, Variant::INT, frame_horizontal_count)
  GD_PROPERTY_BIND(item_container_control, Variant::NODE_PATH, scroll_container_path);

  GD_PROPERTY_BIND_HINT(item_container_control, Variant::STRING, item_frame_control_scene, PropertyHint::PROPERTY_HINT_FILE)

  ADD_SIGNAL(MethodInfo(
    SIGNAL_ITEM_CONTAINER_CONTROL_ON_MOUSE_HOVERED,
    PropertyInfo(SIGNAL_PARAM_CUSTOM_VARIANT_TYPE , "param")
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_ITEM_CONTAINER_CONTROL_ON_MOUSE_EXITED
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_ITEM_CONTAINER_CONTROL_ON_CONTAINER_CHANGED,
    PropertyInfo(SIGNAL_PARAM_CUSTOM_VARIANT_TYPE, "param")
  ));

  ClassDB::bind_method(
    D_METHOD("_item_container_on_index_changed",
      "func_param"
    ),

    &item_container_control::_item_container_on_index_changed
  );

  ClassDB::bind_method(
    D_METHOD("_item_container_on_container_resized"),

    &item_container_control::_item_container_on_container_resized
  );

  ClassDB::bind_method(
    D_METHOD("_item_frame_control_on_mouse_entered",
      "index"
    ),

    &item_container_control::_item_frame_control_on_mouse_entered
  );

  ClassDB::bind_method(
    D_METHOD("_item_frame_control_on_mouse_exited"),
    &item_container_control::_item_frame_control_on_mouse_exited
  );

  ClassDB::bind_method(
    D_METHOD("_on_resized"),
    &item_container_control::_on_resized
  );
}


void item_container_control::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(item_server, sa_container, _item_server)
  }
}

void item_container_control::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    CREATE_LOAD_PACKED_SCENE_CHECKER(_item_frame_control_scene, item_frame_control_scene, item_frame_control)

    _scroll_container = get_node<ScrollContainer>(scroll_container_path);
    
    _v_container = _create_new_vbox_cont();
    get_subautoload_root()->add_child_node(_scroll_container, _v_container);

    connect("resized", Callable(this, "_on_resized"));
  }
}


item_container_control::item_container_control(){
  _bound_item_container = NULL;
  _bound_item_container_node = NULL;
  _v_container = NULL;

  max_size = 0;
  frame_horizontal_count = 0;
}


void item_container_control::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_PREDELETE_NOTIFIER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      while(_item_frames.size() > 0){
        auto _iter = _item_frames.begin();
        _delete_item_frame_control(*_iter);
        _item_frames.erase(_iter);
      }

      while(_h_containers.size() > 0){
        auto _iter = _h_containers.begin();
        _delete_hbox_cont(*_iter);
        _h_containers.erase(_iter);
      }

      if(_v_container)
        _delete_vbox_cont(_v_container);
    }
  }
}


void item_container_control::set_reference(Node* container_node, I_item_container* container){
  item_container_inst_id _prev_id = get_reference_container_id();
  _resize_to_new_container(container);

  _disconnect_container_signal(_bound_item_container_node);
  _connect_container_signal(container_node);
  _bound_item_container_node = container_node;
  _bound_item_container = container;

  item_container_inst_id _new_id = ITEM_CONTAINER_ID_NULL;
  if(container_node && container){
    _update_all_item_data();
    _new_id = container->get_container_id();
  }

  item_container_control_on_container_changed _param;
  _param._control = this;
  _param.new_cont = _new_id;
  _param.prev_cont = _prev_id;
  emit_signal(SIGNAL_ITEM_CONTAINER_CONTROL_ON_CONTAINER_CHANGED, SIGNAL_PARAM_TO_CUSTOM(_param));
}

void item_container_control::set_reference(item_container_inst_id id){
  Node* _container_node = _item_server->get_item_container_node(id);
  I_item_container* _container_item = _item_server->get_item_container(id);

  set_reference(_container_node, _container_item);
}


item_container_inst_id item_container_control::get_reference_container_id(){
  if(!_bound_item_container)
    return ITEM_CONTAINER_ID_NULL;

  return _bound_item_container->get_container_id();
}


void item_container_control::hide_item(int index){
  if(index < 0 || index >= _item_frames.size())
    return;

  _item_frames[index]->hide_item();
}

void item_container_control::show_item(int index){
  if(index < 0 || index >= _item_frames.size())
    return;

  _item_frames[index]->show_item();
}