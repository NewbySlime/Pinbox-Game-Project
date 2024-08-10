#include "algorithm_container.h"
#include "algorithm_directory.h"
#include "algorithm_node.h"
#include "defines.h"
#include "entity_resource_database.h"
#include "godot_error.h"
#include "logger.h"
#include "sprite_resource_data.h"
#include "static_variable_storage.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/json.hpp"

#include "map"


using namespace Game;
using namespace Game::Entity;
using namespace Game::Error;
using namespace Game::Misc::Algorithm;
using namespace GameUtils::Memory;
using namespace godot;




struct static_data: public custom_data{
  public:
    static String get_id(){ return "entity_resouce_database.static_data"; }

    std::map<uint64_t, String> _entitytype_idlist;
    std::map<String, uint64_t> _entitytype_idlist_inv;

    // list of subdata parser
    std::map<String, void(entity_resource_data::*)(Dictionary*)> _subdata_parser_map;

    static_data(){
      _entitytype_idlist = {
        {et_enemy, ENTITY_TYPENAME_ENEMY},
        {et_npc, ENTITY_TYPENAME_NPC},
        {et_player, ENTITY_TYPENAME_PLAYER}
      };

      _entitytype_idlist_inv = create_inverse_of_map(_entitytype_idlist);

      _subdata_parser_map = {
        {sprite_resource_data::get_subdata_name(), &entity_resource_data::add_new_subdata<sprite_resource_data>}
      };
    }
};



void entity_resource_database::_process_entity_res_data(Dictionary* _data_dict){
  INIT_ASSERT()
  Array _paramarr;

  if(!_static_data.get())
    throw gdstr_exception("Cannot get static data.");

  static_data* _static = _static_data->get_data<static_data>();

  bool _error_exists = false;
  String _error_message;

  Array _key_list = _data_dict->keys();
  for(int i = 0; i < _key_list.size(); i++){
    String _full_id = _key_list[i];
    try{
      // split full id into type and id
      PackedStringArray _split_str = _full_id.split(CHARSYMBOL_KEY_SPLIT);
      if(_split_str.size() <= 1)
        throw gdstr_exception("Missing Entity type.");

      auto _iter_type = _static->_entitytype_idlist_inv.find(_split_str[1]);
      if(_iter_type == _static->_entitytype_idlist_inv.end())
        throw gdstr_exception("Entity type is invalid.");

      // check if dupe
      auto _iter_res_data = _res_data_map.find(_full_id);
      if(_iter_res_data != _res_data_map.end()){
        _paramarr.clear();{
          _paramarr.append(_full_id);
        }

        throw gdstr_exception(String("Entity resource data dupe; id: '{0}'.").format(_paramarr));
      }


      entity_resource_data* _new_data = new entity_resource_data();
      try{ // try catch statement just for safe return
        Variant _v = _data_dict->operator[](_full_id);
        _CREATE_CHECKER_DICT1("", _full_id, Variant::DICTIONARY)
        Dictionary _data_part = _v;
        Array _dp_keys = _data_part.keys();

        bool _sd_error_occured = false;
        String _sd_error_message;

        for(int i = 0; i < _dp_keys.size(); i++){
          String _subdata_name = _dp_keys[i];

          _paramarr.clear();{
            _paramarr.append(_full_id);
            _paramarr.append(_dp_keys[i]);
          }

          auto _subdata_iter = _static->_subdata_parser_map.find(_dp_keys[i]);
          if(_subdata_iter == _static->_subdata_parser_map.end()){
            _sd_error_occured = true;
            _sd_error_message += String("WARN: In '{0}', '{1}' is not a valid subdata.").format(_paramarr) + String("\n");
            continue;
          }

          try{
            _v = _data_part[_subdata_name];
            _CREATE_CHECKER_DICT1(_full_id, _subdata_name, Variant::DICTIONARY)
            Dictionary _subdata_data = _v;

            auto _subdata_parser = _subdata_iter->second;
            (_new_data->*_subdata_parser)(&_subdata_data);
          }
          catch(gdstr_exception e){
            _sd_error_occured = true;
            _sd_error_message += String("In '{0}'; an error occurred when parsing subdata '{1}', error message:\n").format(_paramarr) + e.what() + String("\n");
          }
        }

        if(_sd_error_occured)
          throw gdstr_exception(_sd_error_message);
      }
      catch(gdstr_exception e){
        delete _new_data;

        throw e;
      }
    }
    catch(gdstr_exception e){
      _error_exists = true;

      _paramarr.clear();{
        _paramarr.append(_full_id);
      }

      _error_message += String("Cannot load Entity resource data in '{0}'. Error message;") + String("\n");
      _error_message += e.what() + String("\n");
    }
  }

  if(_error_exists)
    throw gdstr_exception(_error_message);
}


void entity_resource_database::_load_resource_data(){
  INIT_ASSERT()
  Array _paramarr;

  LOG_ASSERT("Loading Entity resource data.")

  PackedStringArray _ext_filter;{
    _ext_filter.append(".json");
  }

  recurse_folders<entity_resource_database>(
    entity_resource_data_folder, 
    [](entity_resource_database* _this, Ref<FileAccess> _file){
      JSON _json_data;
      godot::Error _json_err = _json_data.parse(_file->get_as_text());
      if(_json_err != godot::Error::OK){
        String _file_dir = _file->get_path();
        Array _paramarr;{
          _paramarr.append(_file_dir);
          _paramarr.append(_json_err);
        }

        throw gdstr_exception(String("File ({0}) cannot be parsed to JSON. Error when parsing: {1}").format(_paramarr));
      }

      Dictionary _data = _json_data.get_data();
      _this->_process_entity_res_data(&_data);
    },
    _ext_filter,
    this
  );
}


void entity_resource_database::_bind_methods(){
  GD_PROPERTY_BIND_HINT(entity_resource_database, Variant::STRING, entity_resource_data_folder, PropertyHint::PROPERTY_HINT_FILE)
}


void entity_resource_database::_notification(int code){
  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      for(auto _pair: _res_data_map)
        delete _pair.second;
    }
  }
}


void entity_resource_database::_ready(){
  INIT_ASSERT()
  Array _paramarr;

  GET_STATIC_DATA(static_data, _static_data)
  static_data* _static = _static_data->get_data<static_data>();
  if(!_static){
    ERR_ASSERT("Cannot get static data.")
    throw ERR_UNCONFIGURED;
  }

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    LOG_ASSERT("Initiating Entity resource database.")

    _load_resource_data();
  }
}


entity_resource_data* entity_resource_database::get_resource_data(uint64_t entity_type, String res_id){
  static_data* _static = _static_data->get_data<static_data>();

  auto _type_iter = _static->_entitytype_idlist.find(entity_type);
  if(_type_iter == _static->_entitytype_idlist.end())
    return NULL;

  String _full_id = _type_iter->second + String(CHARSYMBOL_KEY_SPLIT) + res_id;
  auto _iter = _res_data_map.find(_full_id);
  if(_iter == _res_data_map.end())
    return NULL;

  return _iter->second;
}