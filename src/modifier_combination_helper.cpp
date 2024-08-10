#include "modifier_combination_helper.h"


using namespace Game;
using namespace Game::Modifiers;
using namespace Game::Item;
using namespace GameUtils::Memory;


modifier_combination_helper::modifier_combination_helper(I_item_container* container){
  if(!container)
    return;
    
  for(int i = 0; i < container->get_container_size(); i++){
    const item_object* _item = container->get_item(i);
    if(!_item)
      continue;

    const modifier_container_item* _modifier_item = _item->get_subdata<modifier_container_item>();
    if(!_modifier_item)
      continue;

    std::pair<const modifier_container_item*, std::shared_ptr<custom_memory>> _new_data = std::make_pair(
      _modifier_item,
      _item->get_subdata_shared<modifier_container_item>()
    );

    _combination_data.insert(_combination_data.end(), _new_data); 
  }
}


void modifier_combination_helper::modify_weapon_stats(weapon_stats_data* stats, const hit_register_state* hitreg_state) const{
  for(auto _iter: _combination_data)
    _iter.first->modify_weapon_stats(stats, hitreg_state);
}