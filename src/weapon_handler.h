#ifndef WEAPON_HANDLER_HEADER
#define WEAPON_HANDLER_HEADER


#include "alternate_data.h"
#include "bus_event.h"
#include "custom_memory.h"
#include "equippable_object.h"
#include "hit_register_handler.h"
#include "hit_register_interface.h"
#include "item_pouch.h"
#include "item_object.h"
#include "item_server.h"
#include "interface_helper.h"
#include "modifier_container_item.h"
#include "modifier_combination_helper.h"
#include "weapon_item.h"

#include "godot_cpp/classes/node2d.hpp"

#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/vector2.hpp"

#include "memory"


namespace Game{
  namespace Item::Factory{
    class weapon_factory;
  }
  
  class weapon_handler: public Game::equippable_object{
  GDCLASS(weapon_handler, Game::equippable_object)
  ATTR_INTERFACE_HELPER_INHERIT(weapon_handler)
  
    private:
      struct _stats_metadata{
        public:
          std::shared_ptr<const GameUtils::Memory::custom_memory> _cd_weapon_stats;
          const Game::Item::weapon_stats_data* _weapon_stats = NULL;

          std::shared_ptr<Game::I_damager> _damager;
          std::shared_ptr<Game::hit_register_data_container> _hitreg_data;

          uint64_t fr_ms_delay = 0;
          double fr_ms_left = 0;

          uint64_t tr_ms_delay = 0;
          double tr_ms_left = 0;

          // timer for charging type of fire mode
          double charge_left = 0;

          // criteria for firing a weapon (for burst or automatic)
          uint64_t burst_iteration_left = 0;
          bool do_fire = false;
          bool fire_until_exhausted = false;


          void reset_metadata(){
            fr_ms_delay = 0;
            fr_ms_left = 0;

            tr_ms_delay = 0;
            tr_ms_left = 0;

            charge_left = 0;

            burst_iteration_left = 0;
            do_fire = false;
            fire_until_exhausted = false;
          }

          _stats_metadata(){
            reset_metadata();
          }
      };



      struct _stats_alternate_metadata: public _stats_metadata{
        public:
          std::shared_ptr<const GameUtils::Memory::custom_memory> _cd_alternate_data;
          const Game::Item::alternate_data* _alternate_data;
      };




      Game::hit_register_handler* _hitreg_handler;
      Game::bus_event* _bus_event;

      std::map<Game::Item::weapon_item::stats_type, _stats_metadata*> _damager_metadata;
      // alternate version
      std::map<Game::Item::weapon_item::stats_type, _stats_alternate_metadata*> _damager_alternate_metadata;

      std::shared_ptr<Game::Modifiers::modifier_combination_helper> _modifier_combination;

      GameUtils::Memory::custom_memory _action_helper_cm;
      GameUtils::Memory::custom_memory _alt_action_helper_cm;

      // no need to make it not const, since the damager will create a copy of it
      std::shared_ptr<const GameUtils::Memory::custom_memory> _cd_curr_wdata;
      const Game::Item::weapon_item* _curr_wdata;

      std::shared_ptr<const GameUtils::Memory::custom_memory> _cd_curr_pouch;
      const Game::Item::item_pouch* _curr_pouch;

      godot::Vector2 _global_pos;
      godot::Vector2 _target_pos;

      Game::hit_register_start_data _get_hitreg_start_data();

      Game::I_action_class* _primary_action;
      Game::I_action_class* _secondary_action;
      

      void _on_bind_container_item_changed(uint64_t _param);
      void _on_bind_container_item_resized();

      void _bind_container_connect_signals(godot::Node* _container);
      void _bind_container_disconnect_signals(godot::Node* _container);

      void _bind_item_pouch(const Game::Item::item_pouch* _pouch);

      void _set_stats_metadata(Game::Item::weapon_item::stats_type type, bool is_alternate, const Game::Item::weapon_item* item);

      void _reconstruct_modifier_combination();
      void _recalculate_damager();

      // fancy word for "shooting weapon"
      void _create_hitreg(Game::hit_register_start_data* _start, _stats_metadata* _metadata);

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;

      void _on_item_set(const Game::Item::item_object* item) override;

    public:
      weapon_handler();

      void _draw() override;
      void _physics_process(double delta) override;

      void _notification(int code);

      void on_action_priming(bool is_secondary, bool is_alternate);
      void on_action_continuing(bool is_secondary, bool is_alternate);
      void on_action_cancelled(bool is_secondary, bool is_alternate);

      bool is_alternate_viable(bool is_secondary);

      void on_update(const Game::action_class_update_data &update_data);


      Game::I_action_class* get_primary_class() override;
      Game::I_action_class* get_secondary_class() override;

  };
}

#endif