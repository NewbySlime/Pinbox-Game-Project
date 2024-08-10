#ifndef ITEM_DROPPER_HEADER
#define ITEM_DROPPER_HEADER

#include "id_types.h"

#include "godot_cpp/classes/node2d.hpp"


namespace Game::Item{
  class item_world_entity_2d_handler;
  class item_dropper: public godot::Node2D{
  GDCLASS(item_dropper, godot::Node2D)

    private:
      friend Game::Item::item_world_entity_2d_handler;
      Game::Item::item_world_entity_2d_handler* _iwe2d_handler;

      Game::Item::item_dropper_inst_id _dropper_id;

      godot::Vector2 _passive_speed;
      godot::Vector2 _last_pos;
 
    protected:
      static void _bind_methods();

    public:
      item_dropper();

      void _notification(int code);

      void _physics_process(double delta) override;

      Game::Item::item_dropper_inst_id get_id();

      godot::Vector2 get_speed();
  };
}

#endif