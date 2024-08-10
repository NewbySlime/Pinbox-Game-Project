#ifndef ITEM_FRAME_CONTROL_HEADER
#define ITEM_FRAME_CONTROL_HEADER

#include "godot_macro.h"
#include "interface_helper.h"
#include "item_object.h"
#include "sprite_resource_instancer.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/canvas_item.hpp"
#include "godot_cpp/classes/margin_container.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/texture_rect.hpp"
#include "godot_cpp/variant/node_path.hpp"

#include "map"


#define SIGNAL_ITEM_FRAME_CONTROL_ON_MOUSE_ENTERED "sifcome"
#define SIGNAL_ITEM_FRAME_CONTROL_ON_MOUSE_EXITED "sifcomex"


namespace Game::Item{
  class item_frame_control: public godot::MarginContainer,
    public GameUtils::Helper::parent_child_watcher_helper,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(item_frame_control, godot::MarginContainer)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(item_frame_control)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(item_frame_control)
  ATTR_INTERFACE_HELPER_INHERIT(item_frame_control)

    GD_PROPERTY_VAR_WITHOUT_DEF_FUNC(double, margin_relative_size)
    GD_PROPERTY_VAR_WITHOUT_DEF_FUNC(int, min_size)

    GD_PROPERTY_VAR(godot::NodePath, texture_rect_nodepath)
    GD_PROPERTY_VAR(godot::NodePath, resource_instancer_nodepath)

    private:
      const Game::Item::item_object* _bound_item = NULL;
      Game::Item::item_inst_id _bound_item_id = ITEM_ID_NULL;

      Game::sprite_resource_instancer* _res_instancer;
      godot::TextureRect* _image_renderer_node;

      godot::CanvasItem* _item_renderer = NULL; 

      int _bound_index;

      void _update_size();

      void _on_mouse_entered();
      void _on_mouse_exited();

    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;


    public:
      item_frame_control();

      void _notification(int code);

      void bind_frame_position(int index);

      // item parameter can be NULL
      void bind_item_object(const Game::Item::item_object* item);

      void hide_item();
      void show_item();
  };
}

#endif