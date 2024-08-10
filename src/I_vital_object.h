#ifndef I_VITAL_OBJECT_HEADER
#define I_VITAL_OBJECT_HEADER

#include "interface_helper.h"


namespace Game::Entity{
struct revive_info{
    bool full_revive;
    double hp;
  };

  struct heal_info{
    double heal_amount;
  };

  struct damage_info{
    double damage;
  };

  class I_vital_object{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_vital_object)
    public:
      // returns the damage value
      virtual double do_damage(const Game::Entity::damage_info& d_info){ return 0; }

      // returns the healing value
      virtual double do_heal(const Game::Entity::heal_info& h_info){ return 0; }

      // returns the healing value
      virtual double do_revive(const Game::Entity::revive_info& r_info){ return 0; }
      virtual double get_health_point(){ return 0; }

      virtual bool use_energy(const double e_point){ return false; }
      virtual double get_energy_point(){ return 0; }
  };
}

#endif