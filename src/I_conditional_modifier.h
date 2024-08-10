#ifndef I_CONDITIONAL_MODIFIER_HEADER
#define I_CONDITIONAL_MODIFIER_HEADER

#include "hit_register_interface.h"
#include "interface_helper.h"
#include "weapon_item.h"


namespace Game{
  class I_conditional_modifier{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_conditional_modifier)

    public:
      virtual ~I_conditional_modifier(){}

      virtual bool check_state(Game::Item::weapon_stats_data* stats, const Game::hit_register_state* hitreg_state){ return false; }

      virtual godot::String get_modifier_title(){ return ""; }
      virtual godot::String get_modifier_description(){ return ""; }
  };
}

#endif