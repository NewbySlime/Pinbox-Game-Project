#ifndef UI_CONTAINER_HEADER
#define UI_CONTAINER_HEADER

#include "algorithm_node.h"
#include "godot_macro.h"
#include "interface_helper.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/sub_viewport.hpp"
#include "godot_cpp/classes/viewport_texture.hpp"
#include "godot_cpp/variant/node_path.hpp"


namespace Game::UI{
  class ui_container: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(ui_container, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(ui_container)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(ui_container)
  ATTR_INTERFACE_HELPER_INHERIT(ui_container)

    GD_PROPERTY_VAR(godot::NodePath, left_controller_path)
    GD_PROPERTY_VAR(godot::NodePath, right_controller_path)

    GD_PROPERTY_VAR(godot::NodePath, left_subviewport_path)
    GD_PROPERTY_VAR(godot::NodePath, right_subviewport_path)

    private:
      godot::Control* _left_controller;
      godot::Control* _right_controller;

      godot::SubViewport* _left_subviewport;
      godot::SubViewport* _right_subviewport;

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container) override;

    public:
      void _notification(int code);

      godot::Control* get_left_control();
      godot::Control* get_right_control();

      godot::Ref<godot::ViewportTexture> get_left_viewport_tex();
      godot::Ref<godot::ViewportTexture> get_right_viewport_tex();


      template<typename T> T* find_child_node_type(T* default_value = NULL){
        T* _res = Game::Misc::Algorithm::find_child_node_type<T>(this, default_value);
        if(_res != default_value)
          return _res;

        _res = Game::Misc::Algorithm::find_child_node_type<T>(_left_controller, default_value);
        if(_res != default_value)
          return _res;

        _res = Game::Misc::Algorithm::find_child_node_type<T>(_right_controller, default_value);
        if(_res != default_value)
          return _res;

        return default_value;
      }
  };
}

#endif