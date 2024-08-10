#ifndef ITEM_DELETER_CONTROL_HEADER
#define ITEM_DELETER_CONTROL_HEADER

#include "godot_macro.h"
#include "godot_signal.h"
#include "interface_helper.h"
#include "item_server.h"
#include "item_container_control.h"
#include "item_container_delete.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/button.hpp"
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/variant/node_path.hpp"


namespace Game::Item{
  class item_deleter_control: public godot::Control,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(item_deleter_control, godot::Control)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(item_deleter_control)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(item_deleter_control)
  ATTR_INTERFACE_HELPER_INHERIT(item_deleter_control)

    GD_PROPERTY_VAR(uint64_t, deleter_container_size)

    GD_PROPERTY_VAR(godot::NodePath, button_delete_node)
    GD_PROPERTY_VAR(godot::NodePath, item_container_control_node)

    private:
      Game::Item::item_server* _item_server;

      Game::Item::item_container_control* _item_container_control;
      godot::Button* _delete_button;

      Game::Item::item_container_delete* _deleter_container;
      Game::Item::item_container_inst_id _deleter_container_id;

      void _on_container_deleted(gdsignal_param_ref param);

      void _on_button_released();

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;

    public:
      item_deleter_control();

      void _notification(int code);
  };
}

#endif