#ifndef DAMAGEABLE_OBJECT_HEADER
#define DAMAGEABLE_OBJECT_HEADER

#include "custom_memory.h"
#include "defines.h"
#include "entity_object.h"
#include "interface_helper.h"
#include "subautoload_container.h"

#include "I_vital_object.h"

#include "godot_cpp/classes/collision_object2d.hpp"
#include "godot_cpp/classes/rigid_body2d.hpp"
#include "godot_cpp/classes/timer.hpp"
#include "godot_cpp/variant/string.hpp"

#include "map"


#define SIGNAL_DAMAGEABLE_OBJECT_ON_HP_CHANGED "sdoohpc"

#define SIGNAL_DAMAGEABLE_OBJECT_ON_HEALED "sdooheal"

#define SIGNAL_DAMAGEABLE_OBJECT_ON_DAMAGED "sdoodmg"
#define SIGNAL_DAMAGEABLE_OBJECT_ON_DEAD "sdooded"
#define SIGNAL_DAMAGEABLE_OBJECT_ON_REVIVED "sdoor"

#define SIGNAL_DAMAGEABLE_OBJECT_ON_EP_CHANGED "sdooepc"


namespace Game::Entity{
  class vital_data;

  class damageable_object: public godot::RigidBody2D,
    public GameUtils::Interface::I_interface_class,
    public Game::Entity::I_vital_object,
    public Game::I_subautoload_compatible{
  GDCLASS(damageable_object, godot::RigidBody2D)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(damageable_object)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(damageable_object)
  ATTR_INTERFACE_HELPER_INHERIT(damageable_object)

    GD_PROPERTY_VAR(bool, is_static)

    private:
      std::shared_ptr<GameUtils::Memory::custom_memory> _vital_data_cm;
      vital_data* _vital_data;

      bool _do_restore_energy = false;
      godot::Timer* _cooldown_ep_timer;

      void _check_health();
      
      void _on_cooldown_ep_timeout();

    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;

      virtual void _on_vital_data_changed(){}
      
      // these are callbacks for derived class for certain event without listening to signals
      // parameters that passed within this function, can be edited
      virtual void _on_hp_changed(){}
      virtual void _on_damaged(Game::Entity::damage_info& d_info){}
      virtual void _on_healed(Game::Entity::heal_info& h_info){}
      virtual void _on_dead(){}
      virtual void _on_revived(Game::Entity::revive_info& r_info){}

      virtual void _on_ep_changed(double new_ep){}
      virtual void _on_energy_exhausted(){}

      void _set_max_hp(double max_hp, bool restore_hp = false);
      void _set_max_ep(double max_ep, bool restore_ep = false);

      void _set_health_point(double new_hp, bool capped = true);
      void _set_energy_point(double new_ep, bool capped = true);

      void _set_restore_rate_ep(double restore_rate);
      void _set_restore_delay_ep(double restore_delay);


    public:
      damageable_object();

      void _physics_process(double delta) override;

      void _notification(int code);

      double do_damage(const Game::Entity::damage_info& d_info) override;
      double do_heal(const Game::Entity::heal_info& h_info) override;
      double do_revive(const Game::Entity::revive_info& r_info) override;

      bool use_energy(const double e_point) override;
      double get_energy_point() override;

      const Game::Entity::vital_data* get_vital_data();
  };


  class vital_data: public GameUtils::Memory::custom_data{
    private:
      friend Game::Entity::damageable_object;

      double max_hp;
      double current_hp;

      double max_ep;
      double current_ep;

      double restore_rate_ep;
      double restore_delay_ep;


    public:
      // TODO define this
      static godot::String get_subdata_name();
      void copy_subdata_to(GameUtils::Memory::custom_memory* mem);

      double get_max_hp() const;
      double get_current_hp() const;

      double get_max_ep() const;
      double get_current_ep() const;
  };
}

#endif