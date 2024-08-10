#ifndef BREAKABLE_OBJECT_HEADER
#define BREAKABLE_OBJECT_HEADER

#include "godot_macro.h"

#include "damageable_object.h"

#include "godot_cpp/variant/string.hpp"


namespace Game::Entity{
  class breakable_object: public Game::Entity::damageable_object{
  GDCLASS(breakable_object, Game::Entity::damageable_object)
    GD_PROPERTY_VAR(godot::String, object_id)

    protected:
      static void _bind_methods();
      
      void _on_damaged(Game::Entity::damage_info& d_info) override;
      void _on_dead() override;

    public:
      breakable_object();

      void _ready() override;

  };
}

#endif