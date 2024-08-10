#include "algorithm_container.h"
#include "algorithm_directory.h"
#include "algorithm_item_parse.h"
#include "defines.h"
#include "entity_database.h"
#include "entity_object.h"
#include "godot_error.h"
#include "id_types.h"
#include "logger.h"
#include "static_variable_storage.h"

#include "godot_cpp/classes/dir_access.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/classes/json.hpp"
#include "godot_cpp/classes/resource_loader.hpp"


using namespace Game;
using namespace Game::Error;
using namespace Game::Entity;
using namespace Game::Misc::Algorithm;
using namespace GameUtils::Memory;
using namespace godot;




struct static_data: public custom_data{
  public:
    static String get_id(){ return "entity_database.static_data"; }

    std::map<uint64_t, std::string> _entitytype_idlist;
    std::map<std::string, uint64_t> _entitytype_idlist_inv;

    static_data(){
      _entitytype_idlist = {
        {et_enemy, ENTITY_TYPENAME_ENEMY},
        {et_npc, ENTITY_TYPENAME_NPC},
        {et_player, ENTITY_TYPENAME_PLAYER}
      };

      _entitytype_idlist_inv = create_inverse_of_map(_entitytype_idlist);
    }
};


void entity_database::_parse_file_data(const Dictionary& _data_part){
  static_data* _static = _static_data->get_data<static_data>();

  Array _paramarr;

  bool _error_occurred = false;
  String _tmp_error_msg = "";

  Array _list_entity_id = _data_part.keys();
  for(int i = 0; i < _list_entity_id.size(); i++){
    String _entity_full_id = _list_entity_id[i];
    try{
      PackedStringArray _split_str = _entity_full_id.split(CHARSYMBOL_KEY_SPLIT);
      if(_split_str.size() <= 1)
        throw gdstr_exception("Entity type is missing.");

      auto _iter_typename = _static->_entitytype_idlist_inv.find(GDSTR_TO_STDSTR(_split_str[0]));
      if(_iter_typename == _static->_entitytype_idlist_inv.end())
        throw gdstr_exception("Entity type is invalid.");

      String _entity_typename = _split_str[0];
      String _entity_id = _split_str[1];

      // check if this is a dupe
      {auto _entity_typeiter = _entity_metadata_map.find(_iter_typename->second);
        if(_entity_typeiter != _entity_metadata_map.end()){
          auto _entity_iter = _entity_typeiter->second->find(_entity_id);
          if(_entity_iter != _entity_typeiter->second->end()){
            _paramarr.clear();{
              _paramarr.append(_entity_full_id);
            }

            throw gdstr_exception(String("Entity dupe detected with ID: '{0}'.").format(_paramarr));
          }
        }
      }

      std::shared_ptr<custom_memory> _memory_data = custom_memory::create_shared<custom_data>();

      // parse data
      Dictionary _entity_data = _data_part[_entity_id];
      entity_data::parse_dictionary_to(_memory_data.get(), &_entity_data);

      // create metadata then store
      _entity_metadata _new_metadata;{
        _new_metadata.memory_data = _memory_data;
        _new_metadata.data = _memory_data->get_data<entity_data>();
      }

      {String _type_class_check = _new_metadata.data->_entity_type_class;
        auto _iter = _entity_scene_list.find(_type_class_check);
        if(_iter == _entity_scene_list.end()){
          _paramarr.clear();{
            _paramarr.append(_entity_id);
            _paramarr.append(_type_class_check);
          }

          throw gdstr_exception(String("Entity ID ({0}); Scene object ('{1}') is invalid.").format(_paramarr));
        }

        _new_metadata.entity_pckscene = _iter->second;
      }

      _entity_metadata_map[_iter_typename->second]->operator[](_entity_id) = _new_metadata;
    }
    catch(gdstr_exception e){
      _error_occurred = true;
      
      _paramarr.clear();{
        _paramarr.append(_entity_full_id);
        _paramarr.append(e.what());
      }

      _tmp_error_msg += String("Something went wrong when parsing ID of '{0}'.\nError: {1}\n").format(_paramarr);
    }
  }

  if(_error_occurred)
    throw gdstr_exception(_tmp_error_msg);
}


void entity_database::_load_entity_scene_type(){
  static_data* _static = _static_data->get_data<static_data>();

  INIT_ASSERT()
  Array _paramarr;

  LOG_ASSERT("Loading Entity Object Scene...")

  Ref<FileAccess> _packed_scene_config = FileAccess::open(entity_class_type_json, FileAccess::ModeFlags::READ);
  if(_packed_scene_config.is_null()){
    godot::Error _open_err = FileAccess::get_open_error();
    ERR_ASSERT("Cannot open packed scene config.")

    throw _open_err;
  }

  godot::Error _err_parse;
  JSON _json; _err_parse = _json.parse(_packed_scene_config->get_as_text());
  if(_err_parse != godot::Error::OK){
    ERR_ASSERT("Cannot parse file data to JSON data.")

    throw _err_parse;
  }

  Dictionary _dict = _json.get_data();
  std::set<std::string> _key_remaining = create_set_of_value(_static->_entitytype_idlist);

  Array _array_key = _dict.keys();
  for(int i = 0; i < _array_key.size(); i++){
    try{
      String _key = _array_key[i];
      // check if there is a duplicate of the key
      auto _dupe_iter = _entity_scene_list.find(_key);
      if(_dupe_iter != _entity_scene_list.end()){
        _paramarr.clear();{
          _paramarr.append(_key);
        }

        WARN_ASSERT(String("Found duplicate key '{0}' of scene type.").format(_paramarr))
        continue;
      }

      // check if the key is valid
      auto _check_iter = _key_remaining.find(GDSTR_TO_STDSTR(_key));
      if(_check_iter == _key_remaining.end()){
        _paramarr.clear();{
          _paramarr.append(_key);
        }

        throw gdstr_exception(String("Key '{0}' is not a valid scene type name.").format(_paramarr));
      }

      Dictionary _data_part = _dict[_key];
      Variant _v;

      String _scene_path;
      _CREATE_CHECKER_DICT2(VARNAME_ENTITY_SCENE_CLASS_PATH, _scene_path, Variant::STRING)

      Ref<PackedScene> _scene_pckscene;
      CREATE_LOAD_PACKED_SCENE_CHECKER(_scene_pckscene, _scene_path, entity_object)

      _entity_scene_list[_key] = _scene_pckscene;
      _key_remaining.erase(_check_iter);
    }
    catch(gdstr_exception e){
      ERR_ASSERT(e.what());
    }
  }

  if(_key_remaining.size() > 0){
    for(auto _remaining_key: _key_remaining){
      _paramarr.clear();{
        _paramarr.append(_remaining_key.c_str());
      }

      WARN_ASSERT(String("Config of Entity scene type '{0}' not found.").format(_paramarr))
    }

    ERR_ASSERT("There are missing Entity objects.")
    throw godot::Error::ERR_UNCONFIGURED;
  }
}

void entity_database::_load_entity_data_config(){
  INIT_ASSERT()
  Array _paramarr;

  LOG_ASSERT("Load Entity data...")
  try{
    PackedStringArray _allowed_ext;{
      _allowed_ext.append(".json");
    }

    recurse_folders<entity_database>(
      entity_folder_data,
      [](entity_database* data, Ref<FileAccess> _file){
        JSON _json;
        godot::Error _err_parse = _json.parse(_file->get_as_text());
        if(_err_parse != NULL){
          Array _paramarr;{
            _paramarr.append(_err_parse);
            _paramarr.append(_json.get_error_message());
          }

          throw gdstr_exception(String("Error when parsing to JSON.\n\tCode: {0}\n\tError Message: {1}").format(_paramarr));
        }

        Dictionary _file_data = _json.get_data();
        data->_parse_file_data(_file_data);
      },
      _allowed_ext,
      this
    );
  }
  catch(gdstr_exception e){
    INIT_ASSERT()
    Array _paramarr;

    ERR_ASSERT(e.what())
    switch(e.get_gderror_equivalent()){
      break; case ERR_SKIP:{
        WARN_ASSERT("Cannot parse some files. But the game will not stop.")
      }

      break; default:{
        throw e.get_gderror_equivalent();
      }
    }
  }
}


void entity_database::_bind_methods(){
  GD_PROPERTY_BIND_HINT(entity_database, Variant::STRING, entity_class_type_json, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(entity_database, Variant::STRING, entity_folder_data, PropertyHint::PROPERTY_HINT_DIR)
}


void entity_database::_notification(int code){
  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      for(auto _pair: _entity_metadata_map)
        delete _pair.second;
    }
  }
}


void entity_database::_ready(){
  INIT_ASSERT()
  Array _paramarr;

  GET_STATIC_DATA(static_data, _static_data);
  static_data* _static = _static_data->get_data<static_data>();
  if(!_static){
    ERR_ASSERT("Cannot get static data.")
    throw ERR_UNCONFIGURED;
  }

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    LOG_ASSERT("Loading Entity database...")

    static_data* _static = _static_data->get_data<static_data>();

    for(auto _pair: _static->_entitytype_idlist)
      _entity_metadata_map[_pair.first] = new std::map<String, _entity_metadata>();

    _load_entity_data_config();
    _load_entity_scene_type();
  }
}


entity_data* entity_database::get_entity_data(uint64_t type, const String& entity_id){
  auto _iter_typecont = _entity_metadata_map.find(type);
  if(_iter_typecont == _entity_metadata_map.end())
    return NULL;

  auto _iter_data = _iter_typecont->second->find(entity_id);
  if(_iter_data == _iter_typecont->second->end())
    return NULL;

  return _iter_data->second.data;
}

Ref<PackedScene> entity_database::get_entity_pckscene(uint64_t type, const String& entity_id){
  auto _iter_typecont = _entity_metadata_map.find(type);
  if(_iter_typecont == _entity_metadata_map.end())
    return NULL;

  auto _iter_data = _iter_typecont->second->find(entity_id);
  if(_iter_data == _iter_typecont->second->end())
    return NULL;

  return _iter_data->second.entity_pckscene;
}


std::shared_ptr<std::vector<entity_data*>> entity_database::find_entity_data(uint64_t type){
  std::shared_ptr<std::vector<entity_data*>> _res(new std::vector<entity_data*>());
  auto _metadata_iter = _entity_metadata_map.find(type);
  if(_metadata_iter == _entity_metadata_map.end())
    return _res;

  for(auto _pair: *_metadata_iter->second)
    _res->push_back(_pair.second.data);

  return _res;
}


Ref<PackedScene> entity_database::get_entity_type_scene(const String& scene_id){
  auto _iter = _entity_scene_list.find(scene_id);
  if(_iter == _entity_scene_list.end())
    return Ref<PackedScene>();

  return _iter->second;
}