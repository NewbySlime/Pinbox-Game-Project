#include "algorithm_container.h"
#include "algorithm_directory.h"
#include "defines.h"
#include "godot_error.h"
#include "logger.h"
#include "modifiers.h"
#include "modifier_database.h"
#include "static_variable_storage.h"

#include "godot_cpp/classes/dir_access.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/classes/json.hpp"


using namespace Game;
using namespace Game::Error;
using namespace Game::Misc::Algorithm;
using namespace Game::Modifiers;
using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace godot;


#define ADD_TO_CREATE_CALLBACK_MACRO(class_name) \
  {MODDB_HELPER_GET_ASSOCIATED_ID(class_name), modifier_init_data( \
    MODDB_HELPER_GET_STATIC_JSON_PARSER(class_name), \
    [](){ \
      return std::shared_ptr<I_modifier>((I_modifier*)new class_name()); \
    } \
  )}


struct modifier_init_data{
  parse_modifier_data_callback parse_data_callback;
  create_modifier_callback create_callback;

  modifier_init_data(
    parse_modifier_data_callback p1,
    create_modifier_callback p2
  ){
    parse_data_callback = p1;
    create_callback = p2;
  }
};


struct static_data: public custom_data{
  public:
    static String get_id(){ return "modifier_database.static_data"; }

    const std::map<std::string, modifier_init_data> modifier_init_data_map = {
      ADD_TO_CREATE_CALLBACK_MACRO(damage_normal_additive_modifier),
      ADD_TO_CREATE_CALLBACK_MACRO(damage_percentage_additive_modifier)
    };

    const std::set<std::string> modifier_list_set = create_set_of_key(modifier_init_data_map);
};



void modifier_database::_load_modifier_data(const godot::String& folder_dir){
  static_data* _static = _static_data->get_data<static_data>();

  INIT_ASSERT()
  Array _paramarr;

  std::set<std::string> _modifier_list_left = _static->modifier_list_set;
  Ref<DirAccess> _data_folder = DirAccess::open(folder_dir);
  if(_data_folder.is_null()){
    _paramarr.clear();{
      _paramarr.append(folder_dir);
    }

    throw gdstr_exception(String("Cannot open modifier folder '{0}'.").format(_paramarr), ERR_DOES_NOT_EXIST);
  }
  
  struct _tmp_param{
    static_data* _static;

    String _folder_dir;
    std::set<std::string>* _modifier_list_left;
    modifier_database* _database;

    bool _errors_exist = false;
    String _errors_msg;
  };

  _tmp_param _tmp_func_param;{
    _tmp_func_param._static = _static;

    _tmp_func_param._folder_dir = folder_dir;
    _tmp_func_param._modifier_list_left = &_modifier_list_left;
    _tmp_func_param._database = this;
  }

  PackedStringArray _allowed_extension;{
    _allowed_extension.append(".json");
  }

  try{
    recurse_folders<_tmp_param>(
      folder_dir,
      [](_tmp_param* _param, Ref<FileAccess> _file){
        JSON _json_data;
        godot::Error _json_err = _json_data.parse(_file->get_as_text());
        if(_json_err != godot::Error::OK){
          String _file_dir = _file->get_path();
          Array _paramarr;{
            _paramarr.append(_file_dir);
            _paramarr.append(_json_err);
          }

          throw gdstr_exception(String("File ({0}) cannot be parse to JSON. Error when parsing: {1}").format(_paramarr));
        }

        Dictionary _dict_data = _json_data.get_data();
        PackedStringArray _dict_keys = _dict_data.keys();
        for(int i = 0; i < _dict_keys.size(); i++){
          String _modifier_name = _dict_keys[i];
          std::string _modifier_name_stdstr = GDSTR_TO_STDSTR(_modifier_name);
          auto _iter_init_data = _param->_static->modifier_init_data_map.find(_modifier_name_stdstr);
          if(_iter_init_data == _param->_static->modifier_init_data_map.end()){
            Array _paramarr;{
              _paramarr.append(_modifier_name);
            }

            throw gdstr_exception(String("'{0}' is not a valid modifier.").format(_paramarr));
          }

          auto _iter_left = _param->_modifier_list_left->find(_modifier_name_stdstr);
          if(_iter_left == _param->_modifier_list_left->end()){
            Array _paramarr;{
              _paramarr.append(_modifier_name);
              _paramarr.append(_param->_folder_dir);
            }

            throw gdstr_exception(String("Found duplicate of '{0}' in '{1}'").format(_paramarr));
          }

          // TODO parse the data
          {Array _paramarr;
            modifier_metadata _metadata;
            Dictionary& _data_part = _dict_data;
            Variant _v;

            _CREATE_CHECKER_DICT2(_modifier_name, VARNAME_MODIFIER_DATA_CHANCE_SCORE, Variant::FLOAT)
            _metadata.chance_score = _v;
            add_randomized_chance_data(_param->_database->_chance_modifier_id, _metadata.chance_score, _modifier_name_stdstr);

            _CREATE_CHECKER_DICT2(_modifier_name, VARNAME_MODIFIER_DATA_MODIFIER_DATA, Variant::DICTIONARY)
            Dictionary _modifier_data_dict = _v;
            _metadata.metadata_randomizer_data = _iter_init_data->second.parse_data_callback(&_modifier_data_dict);

            _param->_database->_modifier_metadata_map[_modifier_name] = _metadata;
            _param->_modifier_list_left->erase(_iter_left);
          }
        }
      },
      _allowed_extension,
      &_tmp_func_param
    );
  }
  catch(gdstr_exception e){
    ERR_ASSERT("Error when parsing modifier data, error msg;")
    ERR_ASSERT(e.what())

    throw e.get_gderror_equivalent();
  }


  if(_modifier_list_left.size() > 0){
    for(auto _left: _modifier_list_left){
      Array _paramarr;{
        _paramarr.append(_left.c_str());
      }

      WARN_ASSERT(String("Modifer data missing: '{0}'").format(_paramarr))
    }

    ERR_ASSERT("Cannot continue if one or more modifier data is missing.")
    throw ERR_FILE_MISSING_DEPENDENCIES;
  }
}


void modifier_database::_bind_methods(){
  GD_PROPERTY_BIND_HINT(modifier_database, Variant::STRING, modifier_folder, PropertyHint::PROPERTY_HINT_DIR)
}


modifier_database::modifier_database(){
  _chance_modifier_id = NULL;
}


void modifier_database::_notification(int code){
  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      if(_chance_modifier_id)
        free_randomized_chance_data(_chance_modifier_id);
    }
  }
}


void modifier_database::_ready(){
  INIT_ASSERT()
  Array _paramarr;

  GET_STATIC_DATA(static_data, _static_data)
  static_data* _static = _static_data->get_data<static_data>();
  if(!_static){
    ERR_ASSERT("Cannot get static data")
    throw ERR_UNCONFIGURED;
  }

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _chance_modifier_id = init_randomized_chance_data<std::string>();

    _load_modifier_data(modifier_folder);
  }
}


std::string modifier_database::get_random_modifier_id(RandomNumberGenerator* rng){
  return get_randomized_chance(_chance_modifier_id, rng);
}

std::shared_ptr<I_modifier> modifier_database::create_specific_modifier(std::string ID){
  static_data* _static = _static_data->get_data<static_data>();

  auto _iter = _static->modifier_init_data_map.find(ID);
  if(_iter == _static->modifier_init_data_map.end())
    return std::shared_ptr<I_modifier>();

  return _iter->second.create_callback();
}


void modifier_database::randomize_modifier(RandomNumberGenerator* rng, std::shared_ptr<I_modifier> modifier){
  I_modifier_database_helper* _helper = (I_modifier_database_helper*)&*modifier;
  std::string _modifier_class_name = _helper->get_modifier_class();
  auto _iter = _modifier_metadata_map.find(_modifier_class_name.c_str());
  // there's no way that this will happen i guess
  if(_iter == _modifier_metadata_map.end())
    return;

  _helper->randomize_modifier(rng, _iter->second.metadata_randomizer_data);
}


const modifier_database::modifier_metadata* modifier_database::get_metadata(std::string ID){
  auto _iter = _modifier_metadata_map.find(ID.c_str());
  if(_iter == _modifier_metadata_map.end())
    return NULL;

  return &_iter->second;
}