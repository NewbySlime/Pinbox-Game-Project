#ifndef SPAWNER_HEADER
#define SPAWNER_HEADER

#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/packed_scene.hpp"

#include "damageable_object.h"
#include "spawner_types.h"

#define SIGNAL_SPAWNER_ON_CHANGED_TYPE "ssoct"

namespace Game::Level{
  class spawner: public godot::Node2D{
  GDCLASS(spawner, godot::Node2D)

    protected:
      static void _bind_methods();

    public:
      void set_spawner_types(spawner_types type);

      void spawn(Game::Entity::damageable_object* obj);
      void spawn_instantiate(godot::PackedScene* scene);
  };
}

#endif