#include "godot_macro.h"
#include "id_types.h"
#include "weapon_item.h"

#include "godot_cpp/core/class_db.hpp"


using namespace Game;
using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace godot;


void weapon_item::copy_stats_to(const weapon_stats_data* _original_stats, std::shared_ptr<custom_memory>* _target_cd_stats, weapon_stats_data** _target_stats){
  (*_target_cd_stats) = custom_memory::create_shared<custom_data>();
  _original_stats->copy_subdata_to(&**_target_cd_stats);
  *_target_stats = (*_target_cd_stats)->get_data<weapon_stats_data>();
}


String weapon_item::get_subdata_name(){
  return "weapon_item";
}

void weapon_item::copy_subdata_to(custom_memory* mem) const{
  mem->change_data<weapon_item>();
  
  // NOTE: create copy of a weapon_stats_data since it is editable
  weapon_item* _data = mem->get_data<weapon_item>();{
    // primary weapon stats
    copy_stats_to(_primary_stats,
      &_data->_cd_primary_stats,
      &_data->_primary_stats
    );

    // secondary weapon stats
    copy_stats_to(_secondary_stats,
      &_data->_cd_secondary_stats,
      &_data->_secondary_stats
    );
  }
}


#define GET_STATS_MACRO(type)\
  switch(type){\
    break; case stats_type_primary:\
      return _primary_stats;\
    break; case stats_type_secondary:\
      return _secondary_stats;\
  }\
\
  return NULL;

weapon_stats_data* weapon_item::get_stats_mutable(stats_type type){
  GET_STATS_MACRO(type)
}

const weapon_stats_data* weapon_item::get_stats(stats_type type) const{
  GET_STATS_MACRO(type)
}



#define GET_STATS_SHARED_MACRO(type)\
  switch(type){\
    break; case stats_type_primary:\
      return _cd_primary_stats;\
    break; case stats_type_secondary:\
      return _cd_secondary_stats;\
  }\
\
  return std::shared_ptr<custom_memory>();

std::shared_ptr<custom_memory> weapon_item::get_stats_shared_mutable(stats_type type){
  GET_STATS_SHARED_MACRO(type)
}

std::shared_ptr<const custom_memory> weapon_item::get_stats_shared(stats_type type) const{
  GET_STATS_SHARED_MACRO(type)
}