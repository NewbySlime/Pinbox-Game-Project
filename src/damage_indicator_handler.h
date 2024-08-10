#ifndef DAMAGE_INDICATOR_HANDLER_HEADER
#define DAMAGE_INDICATOR_HANDLER_HEADER

#include "bus_event.h"
#include "godot_macro.h"
#include "interface_helper.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/vector2.hpp"


namespace Game::Effect{
  class damage_indicator_handler: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(damage_indicator_handler, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(damage_indicator_handler)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(damage_indicator_handler)
  ATTR_INTERFACE_HELPER_INHERIT(damage_indicator_handler)

    GD_PROPERTY_VAR(godot::String, damage_indicator_object)

    private:
      Game::bus_event* _bus_event;

      godot::Ref<godot::PackedScene> _damage_indicator_object;
      godot::Node* _root_node;

      void _on_damageable_damaged(godot::Vector2 origin, double damage);

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;

    public:
      void _notification(int code);

      void set_parent_target(godot::Node* _node);
  };
}

#endif