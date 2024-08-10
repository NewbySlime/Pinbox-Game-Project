#include "algorithm_directory.h"
#include "algorithm_item_parse.h"
#include "consumable_stats_data.h"
#include "godot_error.h"
#include "godot_var_extended.h"
#include "hitreg_data.h"
#include "item_database.h"
#include "item_pouch_data.h"
#include "logger.h"
#include "weapon_stats_data.h"

#include "godot_cpp/classes/dir_access.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/classes/json.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/os.hpp"


using namespace Game;
using namespace Game::Misc::Algorithm;
using namespace Game::Error;
using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace godot;


#define ITEM_DATABASE_KEY_ITEM_ID "item_id"
#define ITEM_DATABASE_KEY_ITEM_TYPE "item_type"
#define ITEM_DATABASE_KEY_ITEM_RARITY "item_rarity"
#define ITEM_DATABASE_KEY_ITEM_DATA_OBJECT "data_object"

#define _SUBDATA_PARSER_MAP_HELPER(subdata_name) {subdata_name::get_subdata_name(), &item_data::_add_new_subdata<subdata_name>}

void item_database::_process_item_data_dict(Dictionary* _data_dict){
  // list of subdata parser
  // NOTE: don't try to parse item_resource_data, it will be handled by item_resource_database
  typedef void(item_data::*subdata_parser_function)(Dictionary*);
  const std::map<String, subdata_parser_function> _subdata_parser_map = std::map<String, subdata_parser_function>{
    _SUBDATA_PARSER_MAP_HELPER(consumable_stats_data),
    _SUBDATA_PARSER_MAP_HELPER(item_pouch_data),

    // weapon_stats_data types
    _SUBDATA_PARSER_MAP_HELPER(weapon_stats_data_primary),
    _SUBDATA_PARSER_MAP_HELPER(weapon_stats_data_secondary)
  };


  INIT_ASSERT()
  Array _paramarr;

  Array _key_list = _data_dict->keys();
  for(int i = 0; i < _key_list.size(); i++){
    String _full_key_id = _key_list[i];
    parse_item_id_result _parse_result;
    try{
      _parse_result = parse_full_item_id(_full_key_id);
    }
    catch(gdstr_exception e){
      _paramarr.clear();{
        _paramarr.append(_full_key_id);
        _paramarr.append(e.what());
      }
      
      ERR_ASSERT(String("Error when parsing full item id of '{0}'.\nError message: {1}").format(_paramarr))
      continue;
    }

    // check if there's a warning
    if(_parse_result.is_warning_exist()){
      _paramarr.clear();{
        _paramarr.append(_full_key_id);
        _paramarr.append(_parse_result.get_warning());
      }

      WARN_ASSERT(String("Warning occurred when parsing full item id of '{0}'.\nWarning message: {1}").format(_paramarr))
    }


    String _key_type = _parse_result.get_item_type();
    String _key_id = _parse_result.get_item_id();

    Variant _current_data = _item_database_dict[_full_key_id];
    if(_current_data.get_type() != Variant::NIL){
      _paramarr.clear();{
        _paramarr.append(_full_key_id);
      }

      ERR_ASSERT(String("Key '{0}' is a dupe.").format(_paramarr))
      continue;
    }

    item_data* _new_data = new item_data();
    try{
      // get the dictionary about the item data
      Variant _v = _data_dict->operator[](_full_key_id);
      _CREATE_CHECKER_DICT1("", _full_key_id, Variant::DICTIONARY)
      Dictionary _data_part = _v;

      // get list of keys in _data_part
      std::set<String> _key_list;{
        Array _data_part_key_list = _data_part.keys();
        for(int i = 0; i < _data_part_key_list.size(); i++)
          _key_list.insert((String)_data_part_key_list[i]);
      }

      // get item name
      _CREATE_CHECKER_DICT2(_full_key_id, VARNAME_ITEM_DATA_NAME, Variant::STRING)
      _new_data->_item_name = _v;
      _key_list.erase(VARNAME_ITEM_DATA_NAME);

      // get item description
      _CREATE_CHECKER_DICT2(_full_key_id, VARNAME_ITEM_DATA_DESCRIPTION, Variant::STRING)
      _new_data->_item_desc = _v;
      _key_list.erase(VARNAME_ITEM_DATA_DESCRIPTION);


      // get subdata(s)
      for(String _subdata_name: _key_list){
        _paramarr.clear();{
          _paramarr.append(_full_key_id);
          _paramarr.append(_subdata_name);
        }
        
        auto _iter = _subdata_parser_map.find(_subdata_name);
        if(_iter == _subdata_parser_map.end()){
          WARN_ASSERT(String("In '{0}', '{1}' is not a valid subdata.").format(_paramarr))
          continue;
        }

        try{
          _CREATE_CHECKER_DICT2(_full_key_id, _subdata_name, Variant::DICTIONARY)
          Dictionary _subdata_data = _v;

          (_new_data->*_iter->second)(&_subdata_data);
        }
        catch(gdstr_exception e){
          ERR_ASSERT(String("In '{0}'; an error occurred when parsing '{1}', error message:").format(_paramarr))
          ERR_ASSERT(e.what())
          continue;
        }
      }
    }
    catch(gdstr_exception e){
      delete _new_data;

      ERR_ASSERT(String(e.what()))
      continue;
    }


    // then insert to database
    gdvar_custom_object _item_data_gd_obj = GDVAR_CUSTOM_FROM_PTR(_new_data);
    Dictionary _data_dict;{
      _data_dict[ITEM_DATABASE_KEY_ITEM_ID] = _new_data->_item_id;
      _data_dict[ITEM_DATABASE_KEY_ITEM_TYPE] = _new_data->_item_type;
      _data_dict[ITEM_DATABASE_KEY_ITEM_DATA_OBJECT] = _item_data_gd_obj;
    }

    _item_database_dict[_full_key_id] = _data_dict;
  }
}


std::shared_ptr<std::vector<item_data*>> item_database::_get_data_filter(Dictionary* data_filter, bool return_first){
  std::shared_ptr<std::vector<item_data*>> _result(new std::vector<item_data*>());
  Array _values_list = _item_database_dict.values();
  for(int i = 0; i < _values_list.size(); i++){
    Dictionary _value_dict = _values_list[i];
    bool _is_valid = true;
    
    Array _filter_keys_list = data_filter->keys();
    for(int i_filter = 0; i_filter < _filter_keys_list.size(); i_filter++){
      String _filter_key = _filter_keys_list[i];
      Variant _filter_value = data_filter->operator[](_filter_key);
      Variant _value_value = _value_dict[_filter_key];

      if(_filter_value != _value_value){
        _is_valid = false;
        break;
      }
    }

    if(_is_valid){
      gdvar_custom_object _item_data_gd_obj = _value_dict[ITEM_DATABASE_KEY_ITEM_DATA_OBJECT];
      _result->insert(_result->end(), GDVAR_CUSTOM_TO(_item_data_gd_obj, item_data));

      if(return_first)
        break;
    }
  }

  return _result;
}


void item_database::_bind_methods(){
  GD_PROPERTY_BIND_HINT(item_database, Variant::STRING, item_data_folder, PropertyHint::PROPERTY_HINT_DIR)
}


void item_database::_notification(int code){
  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      Array _database_dict_values = _item_database_dict.values();
      for(int i = 0; i < _database_dict_values.size(); i++){
        Dictionary _data = _database_dict_values[i];
        gdvar_custom_object _item_data_gd_obj = _data[ITEM_DATABASE_KEY_ITEM_DATA_OBJECT];

        item_data* _item_data = GDVAR_CUSTOM_TO(_item_data_gd_obj, item_data);
        delete _item_data;
      }

      _item_database_dict.clear();
    }
  }
}


void item_database::_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    PackedStringArray _allowed_ext;{
      _allowed_ext.append(".json");
    }

    recurse_folders<item_database>(
      item_data_folder,
      [](item_database* database, Ref<FileAccess> _file){
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
        database->_process_item_data_dict(&_data);
      },
      _allowed_ext,
      this
    );
  }
}


item_data* item_database::get_item_data(const String& item_type, const String& item_id){
 Array _paramarr;{
    _paramarr.append(item_type);
    _paramarr.append(item_id);
  }

  Variant _v_check;

  String _full_item_id = String("{0}" CHARSYMBOL_KEY_SPLIT "{1}").format(_paramarr);
  _v_check = _item_database_dict[_full_item_id];
  if(_v_check.get_type() != Variant::DICTIONARY)
    return NULL;

  Dictionary _data_dict = _v_check;
  _v_check = _data_dict[ITEM_DATABASE_KEY_ITEM_DATA_OBJECT];
  if(_v_check.get_type() != Variant::INT)
    return NULL;

  gdvar_custom_object _item_res_data_gd_obj = _v_check;
  return GDVAR_CUSTOM_TO(_item_res_data_gd_obj, item_data);
}


std::shared_ptr<std::vector<item_data*>> item_database::find_type(const String& item_type){
  Dictionary _filter;{
    _filter[ITEM_DATABASE_KEY_ITEM_TYPE] = item_type;
  }

  return _get_data_filter(&_filter);
}