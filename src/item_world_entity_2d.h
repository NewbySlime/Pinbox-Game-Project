#ifndef ITEM_WORLD_ENTITY_2D_HEADER
#define ITEM_WORLD_ENTITY_2D_HEADER

#include "algorithm_bezier.h"
#include "godot_macro.h"
#include "I_item_handler.h"
#include "interface_helper.h"
#include "item_object.h"
#include "parent_child_watcher_helper.h"
#include "sprite_resource_instancer.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/character_body2d.hpp"
#include "godot_cpp/classes/timer.hpp"

#include "map"


namespace Game::Item{
  class item_picker_area;
  class item_world_entity_2d_handler;
  class item_server;

  class item_world_entity_2d: public godot::CharacterBody2D,
    public GameUtils::Helper::parent_child_watcher_helper,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(item_world_entity_2d, godot::CharacterBody2D)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(item_world_entity_2d)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(item_world_entity_2d)
  ATTR_INTERFACE_HELPER_INHERIT(item_world_entity_2d)
  ATTR_PARENT_CHILD_WATCHER_HELPER_INHERIT(item_world_entity_2d)

    GD_PROPERTY_VAR(double, default_throw_smooth_time)
    GD_PROPERTY_VAR(double, time_until_pickable)

    GD_PROPERTY_VAR(godot::PackedVector2Array, smooth_throw_bezier)

    GD_PROPERTY_VAR(godot::NodePath, animated_player_node)

    private:
      friend Game::Item::item_world_entity_2d_handler;
      Game::Item::iwe2d_inst_id _entity_id;

      // item_server are given by iwe2d handler
      Game::Item::item_server* _item_server;
      Game::Item::item_world_entity_2d_handler* _iwe2d_handler;

      godot::AnimationPlayer* _anim_player;

      Game::Item::item_inst_id _bound_item_id;
      const Game::Item::item_object* _bound_item;

      std::map<uint64_t, Game::Item::item_picker_area*> _picker_inrange_map;

      godot::Timer* _pickable_timer;
      bool _is_pickable;

      godot::Timer* _throw_timer;
      godot::Vector2 _throw_dir;
      double _throw_dist;
      double _throw_last_dist;
      bool _throw_done;

      std::map<uint64_t, Game::Item::I_item_handler*> _item_handler_interfacer_map;

      bool _queue_free = false;


      void _on_animation_finished(godot::String animation_name);

      void _on_picker_deleted(uint64_t id);

      void _on_pickable_timer_timeout();

      void _start_animation_picked();

      void _on_item_deleted(gdvar_custom_object param);
      void _on_item_changed();


    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;

      void _on_child_entered(godot::Node* child) override;
      void _on_child_exited(godot::Node* child) override;

    public:
      item_world_entity_2d();

      void _notification(int code);

      void _physics_process(double delta) override;

      void bind_item(const Game::Item::item_object* item);
      void throw_object(godot::Vector2 force, bool is_pickable = true, double smooth_time = -1);

      void add_picker_gravity(uint64_t picker_id);
      void remove_picker_gravity(uint64_t picker_id);

      void on_object_picked_up();

      void set_pickable(bool _value);
      bool is_pickable();

      uint64_t get_dropped_item_id();
  };
}

#endif