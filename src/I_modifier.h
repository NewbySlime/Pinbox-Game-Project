#ifndef I_MODIFIER_HEADER
#define I_MODIFIER_HEADER

#include "hit_register_interface.h"
#include "interface_helper.h"
#include "weapon_item.h"
#include "weapon_stats_data.h"


namespace Game::Modifiers{
  class I_modifier{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_modifier)

    public:
      virtual ~I_modifier(){}

      // if hitreg_state is NULL then the modifier would expect to apply passive modifiers
      virtual void modify_weapon_stats(Game::Item::weapon_stats_data* stats, const Game::hit_register_state* hitreg_state = NULL) const{}

      virtual godot::String get_modifier_title() const{ return ""; }
      virtual godot::String get_modifier_description() const{ return ""; }
  };
}

#endif