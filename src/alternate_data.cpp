#include "defines.h"

#include "alternate_data.h"
#include "godot_error.h"
#include "weapon_stats_data.h"

#include "set"


using namespace Game;
using namespace Game::Error;
using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace godot;



String alternate_data::get_subdata_name(){
  return VARNAME_WEAPON_STATS_ALTERNATE;
}

void alternate_data::parse_dictionary_to(custom_memory* mem, Dictionary* data){
  typedef void(alternate_data::*subdata_parser_function)(Dictionary*);
  std::map<std::string, subdata_parser_function> _subdata_parser_map = {
    {VARNAME_WEAPON_STATS_ALTERNATE_WEAPON_STATS, &alternate_data::__add_new_subdata<weapon_stats_data>}

    // don't use since it use different subdata name
    //_SUBDATA_PARSER_MAP_HELPER(weapon_stats_data)
  };


  mem->change_data<alternate_data>();
  alternate_data* _new_data = mem->get_data<alternate_data>();

  Array _paramarr;
  Variant _v;

  Dictionary& _data_part = *data;
  std::set<String> _key_left;{
    Array _list = _data_part.keys();
    for(int i = 0; i < _list.size(); i++)
      _key_left.insert((String)_list[i]);
  }

  for(String _key: _key_left){
    _paramarr.clear();{
      _paramarr.append(VARNAME_WEAPON_STATS_ALTERNATE);
      _paramarr.append(_key);
    }

    auto _iter = _subdata_parser_map.find(GDSTR_TO_STDSTR(_key));
    if(_iter == _subdata_parser_map.end()){
      throw gdstr_exception(String("In '{0}'; '{1}' is not a valid subdata.").format(_paramarr));
    }

    _CREATE_CHECKER_DICT2(VARNAME_WEAPON_STATS_ALTERNATE, _key, Variant::DICTIONARY)
    Dictionary _subdata_dict = _v;

    (_new_data->*_iter->second)(&_subdata_dict);
  }
}


void alternate_data::copy_subdata_to(custom_memory* mem){
  mem->change_data<alternate_data>();
  alternate_data* _new_data = mem->get_data<alternate_data>();

  __copy_subdata_to_subdata(_new_data);
}