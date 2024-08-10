#ifndef HIT_REGISTER_INTERFACE_HEADER
#define HIT_REGISTER_INTERFACE_HEADER

#include "godot_macro.h"
#include "custom_memory.h"

#include "godot_cpp/classes/area2d.hpp"
#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/timer.hpp"
#include "godot_cpp/classes/ray_cast2d.hpp"

#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/string.hpp"

#include "memory"


namespace Game{
  namespace Item{
    class weapon_stats_data;
  }

  enum hit_register_type_enum{
    hrte_raycast = 0,
    hrte_dome,
    //hrte_chain_reaction_dome,
    hrte_arc,
    hrte_projectiles,
    hrte_thrown_projectiles,
    hrte_idle,

    hrte__option_flag = 0xff000000,
    hrte__type_flag = 0x00ffffff,
    hrte_with_next = 0x80000000,
    hrte_ignore_simultaneous_deletion = 0x40000000
  };

  struct hit_register_start_data{
    godot::Vector2 position;
    godot::Vector2 dir;
    double length;

    bool is_from_player;
  };

  struct hit_register_state{
    hit_register_type_enum hitreg_type;
  };

  struct hit_register_pass_data{
    godot::CollisionObject2D* collider;
    bool do_continue;
    
    const hit_register_state* current_hitreg_state;
  };

  struct hit_register_data{
    void copy_to(hit_register_data* to){
      memcpy(to, this, sizeof(hit_register_data)
        - sizeof(std::vector<hit_register_data*>)
      );
    }
    
    godot::String key;

    hit_register_type_enum hitreg_type;

    // length of a ray/radius of a dome/radius of projectile's hitbox
    double length;

    // only applicable for thrown projectiles
    // bool end_on_collision;

    // decay time of a dome/time on air/time idling
    double time;
    
    // use vector in case if the hitreg is branching
    std::vector<hit_register_data*> _next;
  };


  class I_damager{
    public:
      virtual ~I_damager(){}

      virtual void on_hit(hit_register_pass_data* pass_data) = 0;
      virtual void on_hitreg_changed(const hit_register_state* new_state = NULL) = 0;
      
      virtual std::shared_ptr<I_damager> clone_damager() = 0;
  };

  class I_hit_register{
    protected:
      virtual ~I_hit_register(){}

      virtual void start_casting() = 0;
  };


  class hit_register_data_container;
  class hit_register_handler;
  struct hitreg_metadata{
    std::shared_ptr<Game::I_damager> damager = std::shared_ptr<Game::I_damager>(NULL);
    std::shared_ptr<Game::hit_register_data_container> data = std::shared_ptr<Game::hit_register_data_container>(NULL);

    hit_register_data* current_hitreg_data;
    hit_register_handler* handler;
  };
}

#endif