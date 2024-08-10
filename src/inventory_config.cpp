#include "defines.h"
#include "godot_error.h"
#include "inventory_config.h"

#include "godot_cpp/variant/variant.hpp"

#include "map"
#include "string"


using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace Game::Error;
using namespace godot;



inventory_config::inv_type inventory_config::_parse_inventory_type(const godot::String& value){
  const std::map<String, inventory_config::inv_type> _inv_type_parser_map = {
    {"normal", inventory_config::inv_type_normal},
    {"loadout", inventory_config::inv_type_loadout}
  };


  auto _iter = _inv_type_parser_map.find(value);
  if(_iter == _inv_type_parser_map.end())
    return _wrong_type;

  return _iter->second;
}

void inventory_config::_parse_data(const Dictionary& _data_part){
  Array _paramarr;
  Variant _v;

  _CREATE_CHECKER_DICT2(VARNAME_INVENTORY_CONFIG, VARNAME_INVENTORY_CONFIG_TYPE, Variant::STRING)
  _inventory_type = _parse_inventory_type((String)_v);
  if(_inventory_type == _wrong_type){
    _paramarr.clear();{
      _paramarr.append(VARNAME_INVENTORY_CONFIG);
      _paramarr.append(VARNAME_INVENTORY_CONFIG_TYPE);
      _paramarr.append((String)_v);
    }

    throw gdstr_exception(String("({0}:{1}) \"{2}\" is not a valid inventory type.").format(_paramarr));
  }

  _CREATE_CHECKER_DICT2(VARNAME_INVENTORY_CONFIG, VARNAME_INVENTORY_CONFIG_SIZE, Variant::INT)
  _inventory_size = _v;
}


String inventory_config::get_subdata_name(){
  return VARNAME_INVENTORY_CONFIG;
}

void inventory_config::parse_dictionary_to(custom_memory* mem, Dictionary* data){
  mem->change_data<inventory_config>();
  inventory_config* _this = mem->get_data<inventory_config>();

  _this->_parse_data(*data);
}


void inventory_config::copy_subdata_to(custom_memory* mem){
  mem->change_data<inventory_config>();
  inventory_config* _target = mem->get_data<inventory_config>();

  _target->_inventory_type = _inventory_type;
  _target->_inventory_size = _inventory_size;
}



inventory_config::inv_type inventory_config::get_inventory_type() const{
  return _inventory_type;
}

uint32_t inventory_config::get_inventory_size() const{
  return _inventory_size;
}