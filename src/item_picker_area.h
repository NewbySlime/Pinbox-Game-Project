#ifndef ITEM_PICKER_AREA_HEADER
#define ITEM_PICKER_AREA_HEADER

#include "id_types.h"

#include "item_server.h"
#include "item_world_entity_2d.h"

#include "godot_cpp/classes/area2d.hpp"

#include "map"


namespace Game::Item{
  class item_world_entity_2d_handler;
  class item_picker_area: public godot::Node2D{
  GDCLASS(item_picker_area, godot::Node2D)

    GD_PROPERTY_VAR(double, max_dragged_speed)
    GD_PROPERTY_VAR(godot::PackedVector2Array, drag_speed_bezier_graph)

    GD_PROPERTY_VAR(godot::NodePath, picker_area_area)
    GD_PROPERTY_VAR(godot::NodePath, picker_collector_area)

    private:
      friend Game::Item::item_world_entity_2d_handler;
      Game::Item::item_server* _item_server;
      Game::Item::item_world_entity_2d_handler* _iwe2d_handler;

      godot::Area2D* _picker_area;
      godot::Area2D* _picker_collector_area;

      Game::Item::I_item_container* _bound_container;
      Game::Item::item_container_inst_id _bound_container_id;

      Game::Item::item_picker_inst_id _item_picker_id;

      double _max_dragged_radius;

      bool _is_container_full;

      void _item_container_on_added(uint64_t data);
      void _item_container_on_removed(uint64_t data);
      void _on_item_container_deleting(uint64_t data);

      void _area_on_body_entered(godot::Node* node);
      void _area_on_body_exited(godot::Node* node);

      void _collector_on_body_entered(godot::Node* node);

      void _unbind_container(Game::Item::I_item_container* container, godot::Node* container_node);
      void _bind_container(Game::Item::I_item_container* container, godot::Node* container_node);

      void _check_container();

    protected:
      static void _bind_methods();

      void bind_item_container(uint64_t container);
      void unbind_item_container();

    public:
      item_picker_area();

      void _notification(int code);

      void _ready() override;

      double get_picker_gravity_speed(godot::Vector2 from_position, const Game::Item::item_object* item);

      Game::Item::item_picker_inst_id get_id();
  };
}

#endif