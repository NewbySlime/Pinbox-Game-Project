#include "entity_data.h"
#include "godot_error.h"
#include "inventory_config.h"
#include "logger.h"
#include "vital_info.h"


#include "map"
#include "set"


using namespace Game;
using namespace Game::Entity;
using namespace Game::Error;
using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace godot;



#define _SUBDATA_PARSER_MAP_HELPER(subdata_name) {subdata_name::get_subdata_name_stdstr(), &entity_data::__add_new_subdata<subdata_name>}


void entity_data::_parse_data_from(const Dictionary& _data_part){
  typedef void(entity_data::*subdata_parser_function)(Dictionary*);
  const std::map<std::string, subdata_parser_function> _subdata_parser_map = {
    _SUBDATA_PARSER_MAP_HELPER(vital_info),
    _SUBDATA_PARSER_MAP_HELPER(entity_data::inv_config_data),
    _SUBDATA_PARSER_MAP_HELPER(entity_data::loadout_inv_config_data)
  };


  Array _paramarr;
  Variant _v;

  std::set<String> _key_left;{
    Array _key_list = _data_part.keys();
    for(int i = 0; i < _key_list.size(); i++)
      _key_left.insert((String)_key_list[i]);
  }

  _CREATE_CHECKER_DICT2(VARNAME_ENTITY_DATA, VARNAME_ENTITY_DATA_NAME, Variant::STRING)
  _entity_name = _v;
  _key_left.erase(VARNAME_ENTITY_DATA_NAME);

  _CREATE_CHECKER_DICT2(VARNAME_ENTITY_DATA, VARNAME_ENTITY_DATA_DESCRIPTION, Variant::STRING)
  _entity_desc = _v;
  _key_left.erase(VARNAME_ENTITY_DATA_DESCRIPTION);

  _CREATE_CHECKER_DICT2(VARNAME_ENTITY_DATA, VARNAME_ENTITY_DATA_TYPE_CLASS, Variant::STRING)
  _entity_type_class = _v;
  _key_left.erase(VARNAME_ENTITY_DATA_TYPE_CLASS);

  for(auto _str_subdata: _key_left){
    _paramarr.clear();{
      _paramarr.append(VARNAME_ENTITY_DATA);
      _paramarr.append(_str_subdata);
    }

    auto _iter = _subdata_parser_map.find(GDSTR_TO_STDSTR(_str_subdata));
    if(_iter == _subdata_parser_map.end()){
      throw gdstr_exception(String("In '{0}'; '{1}' is not a valid subdata in this context.").format(_paramarr));
    }

    _CREATE_CHECKER_DICT2(VARNAME_ENTITY_DATA, _str_subdata, Variant::DICTIONARY)
    Dictionary _subdata_data = _v;

    (this->*_iter->second)(&_subdata_data);
  }
}


String entity_data::get_entity_id() const{
  return _entity_id;
}

String entity_data::get_entity_name() const{
  return _entity_name;
}

String entity_data::get_entity_description() const{
  return _entity_desc;
}

String entity_data::get_entity_type_class() const{
  return _entity_type_class;
}


String entity_data::get_subdata_name(){
  return VARNAME_ENTITY_DATA;
}

void entity_data::parse_dictionary_to(custom_memory* mem, Dictionary* data){
  mem->change_data<entity_data>();
  entity_data* _this = mem->get_data<entity_data>();
  _this->_parse_data_from(*data);
}


void entity_data::copy_subdata_to(custom_memory* mem){
  mem->change_data<entity_data>();
  entity_data* _target = mem->get_data<entity_data>();
  
  _target->_entity_id = _entity_id;
  _target->_entity_name = _entity_name;
  _target->_entity_desc = _entity_desc;

  // then copy the rest of the subdata
  __copy_subdata_to_subdata(_target);
}