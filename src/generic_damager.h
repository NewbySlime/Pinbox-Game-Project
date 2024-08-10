#ifndef GENERIC_DAMAGER_HEADER
#define GENERIC_DAMAGER_HEADER

#include "bus_event.h"
#include "custom_memory.h"
#include "modifier_combination_helper.h"
#include "hit_register_interface.h"
#include "weapon_item.h"
#include "weapon_stats_data.h"

#include "memory"


namespace Game{
  class generic_damager: public Game::I_damager{
    private:
      Game::bus_event* _bus_event;

      std::shared_ptr<Game::Modifiers::modifier_combination_helper> _modifier;

      // this should be a copy
      Game::Item::weapon_stats_data _wstats;

    public:
      ~generic_damager(){}
      generic_damager(Game::bus_event* _bus_event);

      void on_hit(Game::hit_register_pass_data* pass_data) override;
      void on_hitreg_changed(const Game::hit_register_state* new_state = NULL) override;

      std::shared_ptr<Game::I_damager> clone_damager() override;

      static std::shared_ptr<Game::I_damager> create_from_data(
        const Game::Item::weapon_stats_data* w_item, 
        std::shared_ptr<Game::Modifiers::modifier_combination_helper> mod_combination, 
        Game::bus_event* bus
      );
  };
}

#endif