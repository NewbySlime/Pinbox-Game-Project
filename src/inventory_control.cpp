#include "algorithm_node.h"
#include "defines.h"
#include "inventory_control.h"
#include "item_container_control.h"
#include "item_world_entity_2d_handler.h"
#include "predelete_notifier.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"

#include "functional"
#include "map"


using namespace Game;
using namespace Game::Misc::Algorithm;
using namespace Game::Item;
using namespace godot;


#define ANIMATION_SHOW_INV "show"
#define ANIMATION_HIDE_INV "hide"


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(inventory_control)
ATTR_BASE_INTERFACE_HELPER_DEFINE(inventory_control)
ATTR_INTERFACE_HELPER_DEFINE(inventory_control)


void inventory_control::_on_animation_finished(String name){
  const std::map<String, std::function<void(inventory_control*)>> _anim_handler = {
    {ANIMATION_SHOW_INV, [](inventory_control* _this){
      _this->emit_signal(SIGNAL_INVENTORY_CONTROL_DONE_TRANSITIONING);
    }},

    {ANIMATION_HIDE_INV, [](inventory_control* _this){
      _this->emit_signal(SIGNAL_INVENTORY_CONTROL_DONE_TRANSITIONING);
    }}
  };


  INIT_ASSERT()
  LOG_ASSERT("animation finished")

  auto _iter = _anim_handler.find(name);
  if(_iter == _anim_handler.end()){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(name);
    }

    WARN_ASSERT(String("No animation callback handler for '{0}'").format(_paramarr))
    return;
  }

  _iter->second(this);
}


void inventory_control::_on_popout_wait_timer_timeout(){
  if(!ITEM_POSITION_DATA_CHECK(_current_hover))
    return;

  I_item_container* _container = _item_server->get_item_container(_current_hover.container_id);
  if(!_container)
    return;

  const item_object* _item = _container->get_item(_current_hover.index_position);
  if(!_item)
    return;

  _cf_item_desc_popout->bind_item_data(_item);
  _cf_item_desc_popout->do_pop();
}


void inventory_control::_item_container_add(item_container_control* _control, item_container_inst_id id){
  if(id == ITEM_CONTAINER_ID_NULL)  
    return;

  _item_container_metadata* _metadata = NULL;

  auto _iter = _container_id_metadata_map.find(id);
  if(_iter != _container_id_metadata_map.end()){
    _metadata = _iter->second;
  }
  else{
    _metadata = new _item_container_metadata();
    _metadata->container_id = id;

    _container_id_metadata_map[id] = _metadata;
  }

  _item_container_metadata::_object_data _obj_data;
  _obj_data.control_node = _control;
  _obj_data.node_inst_id = _control->get_instance_id();

  _metadata->_container_control_map[_control->get_instance_id()] = _obj_data;
}

void inventory_control::_item_container_remove(item_container_control* _control, item_container_inst_id id){
  if(id == ITEM_CONTAINER_ID_NULL)
    return;

  auto _iter = _container_id_metadata_map.find(id);
  if(_iter == _container_id_metadata_map.end())
    return;

  auto _node_iter = _iter->second->_container_control_map.find(_control->get_instance_id());
  if(_node_iter != _iter->second->_container_control_map.end())
    _iter->second->_container_control_map.erase(_node_iter);

  if(_iter->second->_container_control_map.size() <= 0){
    _item_container_metadata* _metadata = _iter->second;

    _container_id_metadata_map.erase(_iter);
    delete _metadata;
  }
}

void inventory_control::_item_container_control_on_mouse_hovered(gdsignal_param_ref _param){
  item_container_control_on_mouse_hovered_param* param = (item_container_control_on_mouse_hovered_param*)_param;
  _current_hover = *param->get_pos_data();

  _popout_wait_timer->start(description_popout_idle_time);
}

void inventory_control::_item_container_control_on_mouse_exited(){
  ITEM_POSITION_DATA_CLEAR(_current_hover);
  _cf_item_desc_popout->do_hide();
}

void inventory_control::_item_container_control_on_changed(gdsignal_param_ref param){
  item_container_control_on_container_changed* _param = SIGNAL_PARAM_GET_CUSTOMPTR(param, item_container_control_on_container_changed);
  
  _item_container_remove(_param->get_control_node(), _param->get_prev_cont_id());
  _item_container_add(_param->get_control_node(), _param->get_new_cont_id());
}

void inventory_control::_item_container_control_on_exiting(gdsignal_param_ref param){
  INIT_ASSERT()
  LOG_ASSERT("container on exiting")

  predelete_notifier_signal_param* _param = GDVAR_CUSTOM_TO(param, predelete_notifier_signal_param);
  Node* _node = _param->get_node();
  if(!_node->is_class("item_container_control"))
    return;

  item_container_control* _cont_control = (item_container_control*)_node;
  _item_container_remove(_cont_control, _cont_control->get_reference_container_id());
}


void inventory_control::_drop_area_on_mouse_hovered(){
  INIT_ASSERT()
  LOG_ASSERT("drop area on hovered")

  _is_drop_area_hovered = true;
}

void inventory_control::_drop_area_on_mouse_exited(){
  _is_drop_area_hovered = false;
}


void inventory_control::_drop_grabbed_item(){
  if(!ITEM_POSITION_DATA_CHECK(_current_grabbed) || !_is_drop_area_hovered)
    return;

  INIT_ASSERT()
  Array _paramarr;{
    _paramarr.append(_current_grabbed.container_id);
    _paramarr.append(_current_grabbed.index_position);
  }

  LOG_ASSERT(String("curr cont: {0}, idx: {1}").format(_paramarr))
  _item_server->drop_item(_current_grabbed);
}


void inventory_control::_on_item_moved(){
  if(!ITEM_POSITION_DATA_CHECK(_current_grabbed) || !ITEM_POSITION_DATA_CHECK(_current_hover))
    return;

  _item_server->swap_item(_current_grabbed, _current_hover);
}


// TODO unbind version of the function
void inventory_control::_bind_item_container_control(item_container_control* _control){
  _control->connect(SIGNAL_ITEM_CONTAINER_CONTROL_ON_MOUSE_HOVERED, Callable(this, "_item_container_control_on_mouse_hovered"));
  _control->connect(SIGNAL_ITEM_CONTAINER_CONTROL_ON_MOUSE_EXITED, Callable(this, "_item_container_control_on_mouse_exited"));

  _control->connect(SIGNAL_ITEM_CONTAINER_CONTROL_ON_CONTAINER_CHANGED, Callable(this, "_item_container_control_on_changed"));
  _control->connect(SIGNAL_PREDELETE_NOTIFIER_ON_PREDELETE, Callable(this, "_item_container_control_on_exiting"));

  _item_container_add(_control, _control->get_reference_container_id());
}


void inventory_control::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(inventory_control)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(inventory_control)
  ATTR_INTERFACE_HELPER_BIND_GODOT(inventory_control)

  GD_PROPERTY_BIND_HINT(inventory_control, Variant::STRING, UI_item_sprite_template_scene, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(inventory_control, Variant::STRING, item_description_popout_scene, PropertyHint::PROPERTY_HINT_FILE)

  GD_PROPERTY_BIND(inventory_control, Variant::FLOAT, description_popout_idle_time)
  GD_PROPERTY_BIND(inventory_control, Variant::INT, cursor_follower_item_sprite_size)

  GD_PROPERTY_BIND(inventory_control, Variant::NODE_PATH, animation_player)

  GD_PROPERTY_BIND(inventory_control, Variant::NODE_PATH, item_deleter_control)
  GD_PROPERTY_BIND(inventory_control, Variant::NODE_PATH, item_drop_area)
  GD_PROPERTY_BIND(inventory_control, Variant::NODE_PATH, main_icontainer_control)
  GD_PROPERTY_BIND(inventory_control, Variant::NODE_PATH, loadout_icontainer_control)

  ClassDB::bind_method(
    D_METHOD("_on_animation_finished",
      "anim_name"
    ),
    
    &inventory_control::_on_animation_finished
  );


  ClassDB::bind_method(
    D_METHOD("_on_popout_wait_timer_timeout"),
    &inventory_control::_on_popout_wait_timer_timeout
  );


  ClassDB::bind_method(
    D_METHOD("_item_container_control_on_mouse_hovered",
      "param_obj"
    ),

    &inventory_control::_item_container_control_on_mouse_hovered
  );

  ClassDB::bind_method(
    D_METHOD("_item_container_control_on_mouse_exited"),
    &inventory_control::_item_container_control_on_mouse_exited
  );

  ClassDB::bind_method(
    D_METHOD("_item_container_control_on_changed",
      "param"
    ),
    
    &inventory_control::_item_container_control_on_changed
  );

  ClassDB::bind_method(
    D_METHOD("_item_container_control_on_exiting",
      "param"
    ),

    &inventory_control::_item_container_control_on_exiting
  );


  ClassDB::bind_method(
    D_METHOD("_drop_area_on_mouse_hovered"),
    &inventory_control::_drop_area_on_mouse_hovered
  );

  ClassDB::bind_method(
    D_METHOD("_drop_area_on_mouse_exited"),
    &inventory_control::_drop_area_on_mouse_exited
  );

  ADD_SIGNAL(MethodInfo(
    SIGNAL_INVENTORY_CONTROL_DONE_TRANSITIONING
  ));
}


void inventory_control::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(cursor_follower_control, sa_container, _cursor_follower)
    GET_SUBAUTOLOAD(item_server, sa_container, _item_server)
    GET_SUBAUTOLOAD(time_scaler, sa_container, _time_scaler)
  }
}

void inventory_control::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    CREATE_GET_NODE_TESTER(_animation_player, animation_player, AnimationPlayer)
    
    _animation_player->connect("animation_finished", Callable(this, "_on_animation_finished"));

    Control* _drop_area = NULL;
    CREATE_GET_NODE_TESTER(_drop_area, item_drop_area, Control);
    _drop_area->connect("mouse_entered", Callable(this, "_drop_area_on_mouse_hovered"));
    _drop_area->connect("mouse_exited", Callable(this, "_drop_area_on_mouse_exited"));

    CREATE_GET_NODE_TESTER(_icontainer_main, main_icontainer_control, item_container_control)
    CREATE_GET_NODE_TESTER(_icontainer_loadout, loadout_icontainer_control, item_container_control)

    _bind_item_container_control(_icontainer_main);
    _bind_item_container_control(_icontainer_loadout);

    _popout_wait_timer = new Timer();
    _popout_wait_timer->set_autostart(false);
    _popout_wait_timer->set_one_shot(true);
    _popout_wait_timer->connect("timeout", Callable(this, "_on_popout_wait_timer_timeout"));
    add_child_sa(_popout_wait_timer);

    CREATE_LOAD_PACKED_SCENE_CHECKER(_item_sprite_template_scene, UI_item_sprite_template_scene, TextureRect)
    CREATE_LOAD_PACKED_SCENE_CHECKER(_item_description_popout_scene, item_description_popout_scene, item_description_popout)

    _cf_item_desc_popout = (item_description_popout*)_item_description_popout_scene->instantiate();
    _cf_item_desc_popout->set_visible(false);
    _cursor_follower->add_child_node(_cf_item_desc_popout);

    _cf_sprite_renderer = (TextureRect*)_item_sprite_template_scene->instantiate();
    _cf_sprite_renderer->set_custom_minimum_size(Vector2(1, 1) * cursor_follower_item_sprite_size);
    _cursor_follower->add_child_node(_cf_sprite_renderer);

    _cf_sprite_instancer = find_child_node_type<sprite_resource_instancer>(_cf_sprite_renderer);
    if(!_cf_sprite_instancer){
      _paramarr.clear();{
        _paramarr.append(sprite_resource_instancer::get_class_static());
      }

      ERR_ASSERT(String("Cannot get '{0}' in the Sprite Renderer.").format(_paramarr))
      throw Error::ERR_UNCONFIGURED;
    }

    connect("child_entered_tree", Callable(this, "_on_child_entered_tree"));
  }
}


inventory_control::inventory_control(){
  _cf_sprite_renderer = NULL;
  _cf_item_desc_popout = NULL;
  
  _popout_wait_timer = NULL;

  _is_drop_area_hovered = false;
}


void inventory_control::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      if(_cf_sprite_renderer)
        _cf_sprite_renderer->queue_free();

      if(_cf_item_desc_popout)
        _cf_item_desc_popout->queue_free();

      if(_popout_wait_timer)
        delete _popout_wait_timer;
    }
  }
}


void inventory_control::_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    delta = _time_scaler->get_process_time_bypass();
    _animation_player->advance(delta);
  }
}


void inventory_control::_gui_input(const Ref<InputEvent>& _event){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(_event->is_class("InputEventMouseButton")){
      if(_event->is_action_pressed("ui_click")){
        _current_grabbed = _current_hover;
        if(ITEM_POSITION_DATA_CHECK(_current_grabbed)){
          I_item_container* _cont = _item_server->get_item_container(_current_grabbed.container_id);
          if(!_cont)
            return;
          
          const item_object* _item = _cont->get_item(_current_grabbed.index_position);
          if(!_item){
            INIT_ASSERT()
            Array _paramarr;{
              _paramarr.append(_cont->get_container_id());
              _paramarr.append(_current_grabbed.index_position);
            }

            ERR_ASSERT(String("Cannot get item in; container_id: '{0}' index: '{1}'").format(_paramarr))
          }

          auto _metadata_iter = _container_id_metadata_map.find(_current_grabbed.container_id);
          if(_metadata_iter != _container_id_metadata_map.end()){
            for(auto _obj: _metadata_iter->second->_container_control_map)
              _obj.second.control_node->hide_item(_current_grabbed.index_position);
          }
          else{
            INIT_ASSERT()
            Array _paramarr;{
              _paramarr.append(_current_grabbed.container_id);
            }

            WARN_ASSERT(String("Cannot find metadata for container_id: {0}").format(_paramarr))
          }
          
          _cf_sprite_instancer->set_item_object(_item);
          _cf_sprite_renderer->set_texture(_cf_sprite_instancer->as_viewport_texture());
        }
      }
      else if(_event->is_action_released("ui_click")){
        INIT_ASSERT()

        LOG_ASSERT("test input1")
        auto _metadata_iter = _container_id_metadata_map.find(_current_grabbed.container_id);
        if(_metadata_iter != _container_id_metadata_map.end()){
          for(auto _obj: _metadata_iter->second->_container_control_map)
            _obj.second.control_node->show_item(_current_grabbed.index_position);
        }

        LOG_ASSERT("test input2")
        if(_is_drop_area_hovered)
          _drop_grabbed_item();
        else
          _on_item_moved();

        LOG_ASSERT("test input3")
        _cf_sprite_renderer->set_texture(Ref<Texture2D>());
        LOG_ASSERT("test input4")
        ITEM_POSITION_DATA_CLEAR(_current_grabbed);
        LOG_ASSERT("test input5")
      }
    }
  }
}


void inventory_control::bind_icontainer_main(item_container_inst_id inst_id){
  _icontainer_main->set_reference(inst_id);
}

void inventory_control::bind_icontainer_loadout(item_container_inst_id inst_id){
  _icontainer_loadout->set_reference(inst_id);
}


void inventory_control::show(){
  _animation_player->play(ANIMATION_SHOW_INV);
}

void inventory_control::hide(){
  _animation_player->play(ANIMATION_HIDE_INV);
} 