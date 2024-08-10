#include "defines.h"
#include "logger.h"
#include "godot_error.h"

#include "game_global_variable.h"

#include "hit_register_data_container.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/classes/json.hpp"

#include "exception"
#include "string"


using namespace Game;
using namespace Game::Error;
using namespace godot;


game_global_variable* _this_autoload = NULL;



void game_global_variable::_process_combo_tracker_data_dict(Dictionary* _data_dict){
  INIT_ASSERT()
  Variant _v;
  Array _paramarr;

  _combo_tracker_data_map.clear();
  
  Array _keys = _data_dict->keys();
  for(int i = 0; i < _keys.size(); i++){
    String _key = _keys[i];
    ui_combo_tracker_data_struct _data_struct;

    try{
      if(!_key.is_valid_int()){
        _paramarr.clear();{
          _paramarr.append(_key);
        }

        throw gdstr_exception(String("Key '{0}' is not a number.").format(_paramarr));
      }

      _v = _data_dict->operator[](_key);
      _CREATE_CHECKER_DICT1(_key, "", Variant::DICTIONARY)

      Dictionary _data_part = _v;
      int _enum = _key.to_int();

      _CREATE_CHECKER_DICT2(_key, VARNAME_COMBO_TRACKER_BASE_LIMIT, Variant::FLOAT)
      _data_struct.base_limit = _v;

      _CREATE_CHECKER_DICT2(_key, VARNAME_COMBO_TRACKER_BASE_MULTIPLIER, Variant::FLOAT)
      _data_struct.base_multiplier = _v;

      _CREATE_CHECKER_DICT2(_key, VARNAME_COMBO_TRACKER_MAX_MULTIPLIER, Variant::FLOAT)
      _data_struct.max_multiplier = _v;

      _CREATE_CHECKER_DICT2(_key, VARNAME_COMBO_TRACKER_MULTIPLIER_DECREMENT, Variant::FLOAT)
      _data_struct.multiplier_decrement = _v;

      _CREATE_CHECKER_DICT2(_key, VARNAME_COMBO_TRACKER_LOGO_SPRITE_DIR, Variant::STRING)
      String _logo_dir = _v;

      _data_struct._logo_sprite = _res_loader->load(_logo_dir);
      _CREATE_CHECKER_PACKED_SCENE(_data_struct._logo_sprite, Texture)
      _data_struct.logo_sprite = _data_struct._logo_sprite;

      _combo_tracker_data_map[_enum] = _data_struct;
    }
    catch(gdstr_exception e){
      ERR_ASSERT(String(e.what()))
    }
  }
}


void game_global_variable::_process_player_init_data_dict(Dictionary* _data_dict){

}



void game_global_variable::_bind_methods(){
  GD_PROPERTY_BIND_HINT(game_global_variable, Variant::STRING, combo_tracker_data_json, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(game_global_variable, Variant::STRING, player_init_data_json, PropertyHint::PROPERTY_HINT_FILE)
}


game_global_variable::game_global_variable(){
  main_scene_camera = NULL;
  main_scene_viewport = NULL;
  main_view_pointer = NULL;
  main_view_camera = NULL;
}



void game_global_variable::_ready(){
  INIT_ASSERT()
  Array _paramarr;

  _res_loader = ResourceLoader::get_singleton();

  // combo tracker data
  PROCESS_JSON_PATH(
    combo_tracker_data_json,
    _process_combo_tracker_data_dict
  )

  // player init data
  PROCESS_JSON_PATH(
    player_init_data_json,
    _process_player_init_data_dict
  )

  _this_autoload = this;
}


const ui_combo_tracker_data_struct* game_global_variable::get_ui_combo_tracker_data(uint32_t type){
  auto _iter = _combo_tracker_data_map.find(type);
  if(_iter == _combo_tracker_data_map.end())
    return NULL;

  return &_iter->second;
}


bool game_global_variable::is_in_game_scene(){
  // TOOD this should need to change
  return
    main_scene_camera &&
    main_view_camera &&
    main_view_pointer &&
    main_scene_viewport
  ;
}