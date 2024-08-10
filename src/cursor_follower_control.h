#ifndef CURSOR_FOLLOWER_CONTROL_HEADER
#define CURSOR_FOLLOWER_CONTROL_HEADER

#include "interface_helper.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/input_event.hpp"


namespace Game{
  class cursor_follower_control: public godot::Control,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(cursor_follower_control, godot::Control)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(cursor_follower_control)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(cursor_follower_control)
  ATTR_INTERFACE_HELPER_INHERIT(cursor_follower_control)

    private:
      godot::Node2D* _follower;

    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;
    
    public:
      cursor_follower_control();

      void _notification(int code);

      void _unhandled_input(const godot::Ref<godot::InputEvent>& _event) override;

      void add_child_node(godot::Node* _node);
      void remove_child_node(godot::Node* _node);
  };
}

#endif