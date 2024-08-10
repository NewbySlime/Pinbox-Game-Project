#ifndef RNG_SERVER_HEADER
#define RNG_SERVER_HEADER

#include "custom_memory.h"
#include "interface_helper.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/random_number_generator.hpp"

#include "map"


namespace Game{
  enum rng_types{
    rng_type_global = 0,
    rng_type_item_generation = 1,
    rng_type_enemy_generation = 2,
    rng_type_dungeon_generation = 3,
  };

  class rng_server: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(rng_server, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(rng_server)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(rng_server)
  ATTR_INTERFACE_HELPER_INHERIT(rng_server)

    private:
      std::shared_ptr<GameUtils::Memory::custom_memory> _static_data;

      std::map<rng_types, godot::RandomNumberGenerator*> _rng_type_map;
      int _current_seed;

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container) override;
      void _on_gamenode_ready() override;

    public:
      void _notification(int code);

      int get_seed();
      void set_seed(int seed);
      void set_seed_random();

      godot::RandomNumberGenerator* get_randomizer(rng_types rtype);
  };
}

#endif