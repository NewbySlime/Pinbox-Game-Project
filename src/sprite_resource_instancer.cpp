#include "algorithm_node.h"
#include "logger.h"
#include "sprite_resource_instancer.h"
#include "structure_holder_node.h"

#include "godot_cpp/classes/animation_library.hpp"
#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/animated_sprite2d.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/sprite_frames.hpp"
#include "godot_cpp/classes/sprite2d.hpp"
#include "godot_cpp/classes/sub_viewport.hpp"


using namespace Game;
using namespace Game::Animation;
using namespace Game::Entity;
using namespace Game::Item;
using namespace Game::Misc::Algorithm;
using namespace GameUtils::Helper;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(sprite_resource_instancer)
ATTR_BASE_INTERFACE_HELPER_DEFINE(sprite_resource_instancer)
ATTR_INTERFACE_HELPER_DEFINE(sprite_resource_instancer)
ATTR_PARENT_CHILD_WATCHER_HELPER_DEFINE(sprite_resource_instancer)



void sprite_resource_instancer::_create_resource_nodes(){
  if(_pivot_point){
    INIT_ASSERT()
    ERR_ASSERT("Pivot point already exists, clear the resource nodes first.")
    return;
  }

  // instance all needed nodes first
  Node* _new_pivot_point;
  if(get_parent()->is_class(Node2D::get_class_static()))
    _new_pivot_point = normal_case_pivot_node_pckscene->instantiate();
  else
    _new_pivot_point = subviewport_node_pckscene->instantiate();

  _pivot_point = _new_pivot_point;
  get_subautoload_root()->add_child_node(get_parent(), _new_pivot_point);
  __child_watcher_change_parent(_pivot_point);

  animation_interfacer* _new_animation_interfacer = (animation_interfacer*)animation_interfacer_pckscene->instantiate();
  get_subautoload_root()->add_child_node(_pivot_point, _new_animation_interfacer);
  _anim_interfacer = _new_animation_interfacer;

  // instance_animation_node_pckscene is a structure_holder_node, so storing the instanced reference should be avoided
  get_subautoload_root()->add_child_node(_pivot_point,
    instance_animation_node_pckscene->instantiate());


  {// check and pass resource data (image)
    Ref<Resource> _resource = _resource_data->get_image();
    if(_resource.is_valid()){
      Ref<Image> _image = _resource;
      Ref<SpriteFrames> _sprite_frames = _resource;
      Sprite2D* _tex_node = find_child_node_type<Sprite2D>(_pivot_point);
      AnimatedSprite2D* _anim_node = find_child_node_type<AnimatedSprite2D>(_pivot_point);
      bool _delete_as2d = true, _delete_s2d = true;

      if(_sprite_frames.is_valid()){
        _delete_as2d = false;
        if(_anim_node)
          _anim_node->set_sprite_frames(_sprite_frames);
        else{
          INIT_ASSERT()
          Array _paramarr;{
            _paramarr.append(AnimatedSprite2D::get_class_static());
          }

          ERR_ASSERT(String("Cannot find resource node: '{0}'").format(_paramarr))
        }
      }
      else if(_image.is_valid()){
        _delete_s2d = false;
        if(_tex_node)
          _tex_node->set_texture(_image);
        else{
          INIT_ASSERT()
          Array _paramarr;{
            _paramarr.append(Sprite2D::get_class_static());
          }

          ERR_ASSERT(String("Cannot find resource node: '{0}'").format(_paramarr))
        }
      }
      else{
        INIT_ASSERT()
        Array _paramarr;{
          _paramarr.append(_resource_data->get_full_id());
          _paramarr.append(Image::get_class_static());
          _paramarr.append(SpriteFrames::get_class_static());
        }

        ERR_ASSERT(String("On: '{0}'; resource exists, but not an object of '{1}' or '{2}'.").format(_paramarr))
      }

      
      // delete unused nodes
      if(_delete_as2d && _anim_node)
        _anim_node->queue_free();

      if(_delete_s2d && _tex_node)
        _tex_node->queue_free();
    }
  }


  {// check and pass resource data (AnimationLibrary)
    Ref<Resource> _resource = _resource_data->get_animation_library();
    if(_resource.is_valid()){
      Ref<AnimationLibrary> _anim_lib = _resource;
      AnimationPlayer* _anim_player = find_child_node_type<AnimationPlayer>(_pivot_point);
      bool _delete_ap = true;

      if(_anim_lib.is_valid()){
        _delete_ap = false;
        if(_anim_player){
          _anim_player->remove_animation_library("");
          _anim_player->add_animation_library("", _anim_lib);
        }
        else{
          INIT_ASSERT()
          Array _paramarr;{
            _paramarr.append(AnimationPlayer::get_class_static());
          }

          ERR_ASSERT(String("Cannot find resource node: '{0}'").format(_paramarr))
        }
      }
      else{
        INIT_ASSERT()
        Array _paramarr;{
          _paramarr.append(_resource_data->get_full_id());
          _paramarr.append(AnimationLibrary::get_class_static());
        }

        ERR_ASSERT(String("On: '{0}'; resource exists, but not an object of '{1}'.").format(_paramarr))
      }


      // delete unused nodes
      if(_delete_ap && _anim_player)
        _anim_player->queue_free();
    }
  }


  {// check and pass resource data (Custom animation object)
    Ref<PackedScene> _pckscene = _resource_data->get_custom_object_sprite();
    if(_pckscene.is_valid()){
      Node* _custom_node = _pckscene->instantiate();
      get_subautoload_root()->add_child_node(_pivot_point, _custom_node);
    }
  }
}

void sprite_resource_instancer::_clear_resource_nodes(){
  if(_pivot_point)
    _pivot_point->queue_free();

  _pivot_point = NULL;
  _anim_interfacer = NULL;
}


void sprite_resource_instancer::_on_entity_changed(){
  for(auto _pair: _interface_entity_list){
    _pair.second->set_entity_data(_current_entity_data);
    _pair.second->set_entity_resource_data(_current_entity_res_data);
  }
}

void sprite_resource_instancer::_on_item_changed(){
  for(auto _pair: _interface_item_list)
    _pair.second->set_item_object(_current_item_object);
}


void sprite_resource_instancer::_notify_entity_handler_node(uint64_t node_id){
  auto _iter = _interface_entity_list.find(node_id);
  if(_iter == _interface_entity_list.end())
    return;

  _iter->second->set_entity_data(_current_entity_data);
  _iter->second->set_entity_resource_data(_current_entity_res_data);
}

void sprite_resource_instancer::_notify_item_handler_node(uint64_t node_id){
  auto _iter = _interface_item_list.find(node_id);
  if(_iter == _interface_item_list.end())
    return;

  _iter->second->set_item_object(_current_item_object);
}


void sprite_resource_instancer::_on_child_entered(Node* _child){
  I_item_handler* _item_handler = NULL;
  INTERFACE_GET(_child, I_item_handler, _item_handler)
  if(_item_handler){
    _interface_item_list[_child->get_instance_id()] = _item_handler; 
    _notify_item_handler_node(_child->get_instance_id());
  }

  I_entity_handler* _entity_handler = NULL;
  INTERFACE_GET(_child, I_entity_handler, _entity_handler)
  if(_entity_handler){
    _interface_entity_list[_child->get_instance_id()] = _entity_handler;
    _notify_entity_handler_node(_child->get_instance_id());
  }
}

void sprite_resource_instancer::_on_child_exited(Node* _child){
  auto _iter_item = _interface_item_list.find(_child->get_instance_id());
  if(_iter_item != _interface_item_list.end())
    _interface_item_list.erase(_iter_item);

  auto _iter_entity = _interface_entity_list.find(_child->get_instance_id());
  if(_iter_entity != _interface_entity_list.end())
    _interface_entity_list.erase(_iter_entity);
}


void sprite_resource_instancer::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(sprite_resource_instancer)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(sprite_resource_instancer)
  ATTR_INTERFACE_HELPER_BIND_GODOT(sprite_resource_instancer)
  ATTR_PARENT_CHILD_WATCHER_HELPER_BIND_GODOT(sprite_resource_instancer)

  GD_PROPERTY_BIND_HINT(sprite_resource_instancer, Variant::STRING, animation_interfacer_pckscene_path, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(sprite_resource_instancer, Variant::STRING, instance_animation_node_pckscene_path, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(sprite_resource_instancer, Variant::STRING, normal_case_pivot_node_pckscene_path, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(sprite_resource_instancer, Variant::STRING, subviewport_node_pckscene_path, PropertyHint::PROPERTY_HINT_FILE)
}


void sprite_resource_instancer::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    CREATE_LOAD_PACKED_SCENE_CHECKER(animation_interfacer_pckscene, animation_interfacer_pckscene_path, animation_interfacer)
    CREATE_LOAD_PACKED_SCENE_CHECKER(instance_animation_node_pckscene, instance_animation_node_pckscene_path, structure_holder_node)
    CREATE_LOAD_PACKED_SCENE_CHECKER(normal_case_pivot_node_pckscene, normal_case_pivot_node_pckscene_path, Node2D)
    CREATE_LOAD_PACKED_SCENE_CHECKER(subviewport_node_pckscene, subviewport_node_pckscene_path, SubViewport)

    // don't process, since nothing to check
    set_process(false);
  }
}


void sprite_resource_instancer::_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(_entity_updated){
      _entity_updated = false;
      _on_entity_changed();
    }

    set_process(false);
  }
}


void sprite_resource_instancer::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_PARENT_CHILD_WATCHER_HELPER_ON_NOTIFICATION(code)
  }
}


void sprite_resource_instancer::set_item_object(const Game::Item::item_object* item){
  _current_item_object = item;
  _clear_resource_nodes();

  _resource_data = NULL;
  _resource_data_cm.reset();
  _on_item_changed();

  if(!item)
    return;

  const item_resource_data* _res_data = item->get_resource_data();
  if(!_res_data)
    return;

  _resource_data = _res_data->get_subdata<sprite_resource_data>();
  _resource_data_cm = _res_data->get_subdata_shared<sprite_resource_data>();

  if(!_resource_data)
    return;

  _create_resource_nodes();
}


void sprite_resource_instancer::set_entity_data(const entity_data* data){
  _current_entity_data = data;
  _entity_updated = true;
  set_process(true);
}

void sprite_resource_instancer::set_entity_resource_data(const entity_resource_data* res_data){
  _current_entity_res_data = res_data;
  _entity_updated = true;
  set_process(true);

  _clear_resource_nodes();

  _resource_data = NULL;
  _resource_data_cm.reset();

  if(!res_data)
    return;

  _resource_data = res_data->get_subdata<sprite_resource_data>();
  _resource_data_cm = res_data-> get_subdata_shared<sprite_resource_data>();

  if(!_resource_data)
    return;

  _create_resource_nodes();
}


Node* sprite_resource_instancer::get_pivot_node(){
  return _pivot_point;
}


Ref<ViewportTexture> sprite_resource_instancer::as_viewport_texture(){
  if(!_pivot_point || !_pivot_point->is_class(SubViewport::get_class_static()))
    return Ref<ViewportTexture>();

  SubViewport* _viewport = (SubViewport*)_pivot_point;
  return _viewport->get_texture();
}


animation_interfacer* sprite_resource_instancer::get_animation_interface(){
  return _anim_interfacer;
}