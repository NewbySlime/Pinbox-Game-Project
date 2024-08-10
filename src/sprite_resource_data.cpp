#include "defines.h"
#include "logger.h"

#include "godot_error.h"
#include "sprite_resource_data.h"

#include "godot_cpp/classes/animation_library.hpp"
#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/sprite_frames.hpp"

#include "map"


using namespace Game;
using namespace Game::Error;
using namespace GameUtils::Memory;
using namespace godot;



void sprite_resource_data::_parse_sprite_resource_data_dict(Dictionary* data){
  enum _tmp_restype{
    static_res,
    animated_res,
    animation_lib,
    custom_obj_res
  };

  const std::map<String, _tmp_restype> _res_typestr_map = {
    {VARNAME_SPRITE_RESOURCE_DATA_STATIC, _tmp_restype::static_res},
    {VARNAME_SPRITE_RESOURCE_DATA_ANIMATED, _tmp_restype::animated_res},
    {VARNAME_SPRITE_RESOURCE_DATA_ANIMATION_LIB, _tmp_restype::animation_lib},
    {VARNAME_SPRITE_RESOURCE_DATA_CUSTOM, _tmp_restype::custom_obj_res}
  };


  INIT_ASSERT()
  Array _paramarr;
  Variant _v;

  bool _an_error_occurred = false;
  String _error_message = "";
  
  Array _keys_list = data->keys();
  for(int i = 0; i < _keys_list.size(); i++){
    String _key = _keys_list[i];
    _paramarr.clear();{
      _paramarr.append(get_subdata_name());
      _paramarr.append(_key);
    }

    auto _iter = _res_typestr_map.find(_key);
    if(_iter == _res_typestr_map.end()){
      _an_error_occurred = true;
      _error_message = String("Key named '{1}' is not a valid key.\n").format(_paramarr);

      // give another error for another type
      continue;
    } 

    switch(_iter->second){
      // item_image
      break; case _tmp_restype::static_res:{
        _v = data->operator[](VARNAME_SPRITE_RESOURCE_DATA_STATIC);
        if(_v.get_type() == Variant::STRING){
          try{
            String _path = _v;
            Ref<Resource> _res_data;
            CREATE_LOAD_RESOURCE_CHECKER(_res_data, _path, Image)

            // if the animated_sprite exist at the same time
            if(_image_res.is_valid()){
              Array _tmp_paramarr;{
                _tmp_paramarr.append(VARNAME_SPRITE_RESOURCE_DATA_STATIC);
                _tmp_paramarr.append(VARNAME_SPRITE_RESOURCE_DATA_ANIMATED);
              }

              _an_error_occurred = true;
              _error_message += String("WARN: '{0}' will not be used, '{1}' already cover it up.").format(_tmp_paramarr) + "\n";
            }
            else
              _image_res = _res_data;
          }
          catch(gdstr_exception e){
            _an_error_occurred = true;
            _error_message += e.what() + "\n";
          }
        }
      }

      // animated_sprite
      break; case _tmp_restype::animated_res:{
        _v = data->operator[](VARNAME_SPRITE_RESOURCE_DATA_ANIMATED);
        if(_v.get_type() == Variant::STRING){
          try{
            String _path = _v;
            Ref<Resource> _res_data;
            CREATE_LOAD_RESOURCE_CHECKER(_res_data, _path, SpriteFrames)

            // if the static_image exist at the same time
            if(_image_res.is_valid()){
              Array _tmp_paramarr;{
                _tmp_paramarr.append(VARNAME_SPRITE_RESOURCE_DATA_STATIC);
                _tmp_paramarr.append(VARNAME_SPRITE_RESOURCE_DATA_ANIMATED);
              }

              _an_error_occurred = true;
              _error_message += String("WARN: '{0}' will be replaced with '{1}'.").format(_tmp_paramarr) + "\n";
            }

            _image_res = _res_data;
          }
          catch(gdstr_exception e){
            _an_error_occurred = true;
            _error_message += e.what() + "\n";
          }
        }
      }

      // animation_library
      break; case _tmp_restype::animation_lib:{
        _v = data->operator[](VARNAME_SPRITE_RESOURCE_DATA_ANIMATION_LIB);
        if(_v.get_type() == Variant::STRING){
          try{
            String _path = _v;
            CREATE_LOAD_RESOURCE_CHECKER(_animation_library, _path, godot::AnimationLibrary)
          }
          catch(gdstr_exception e){
            _an_error_occurred = true;
            _error_message += e.what() + "\n";
          }
        }
      }

      // custom_object_sprite
      break; case _tmp_restype::custom_obj_res:{
        _v = data->operator[](VARNAME_SPRITE_RESOURCE_DATA_CUSTOM);
        if(_v.get_type() == Variant::STRING){
          try{
            String _path = _v;
            //CREATE_LOAD_PACKED_SCENE_CHECKER(_custom_object_sprite, _path, animated_resource_handler)
          }
          catch(gdstr_exception e){
            _an_error_occurred = true;
            _error_message += e.what() + "\n";
          }
        }
      }
    }
  }

  // Note: This is for try to parsing all data without actually throwing an error. Errors are appended in _error_message
  if(_an_error_occurred)
    throw gdstr_exception(_error_message);
}


String sprite_resource_data::get_subdata_name(){
  return "item_resource";
}

void sprite_resource_data::parse_dictionary_to(custom_memory* mem, Dictionary* dict){
  mem->change_data<sprite_resource_data>();

  sprite_resource_data* _data = mem->get_data<sprite_resource_data>();
  _data->_parse_sprite_resource_data_dict(dict);
}


void sprite_resource_data::copy_subdata_to(custom_memory* mem) const{
  mem->change_data<sprite_resource_data>();

  sprite_resource_data* _data = mem->get_data<sprite_resource_data>();
  _data->_image_res = _image_res;
  _data->_animation_library = _animation_library;
  _data->_custom_object_sprite = _custom_object_sprite;
}


String sprite_resource_data::get_full_id() const{
  return _full_id;
}


Ref<Resource> sprite_resource_data::get_image() const{
  return _image_res;
}

Ref<Resource> sprite_resource_data::get_animation_library() const{
  return _animation_library;
}

Ref<PackedScene> sprite_resource_data::get_custom_object_sprite() const{
  return _custom_object_sprite;
}