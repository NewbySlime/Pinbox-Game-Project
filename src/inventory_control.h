#ifndef INVENTORY_CONTROL_HEADER
#define INVENTORY_CONTROL_HEADER


#include "cursor_follower_control.h"
#include "godot_macro.h"
#include "interface_helper.h"
#include "item_container_control.h"
#include "item_description_popout.h"
#include "item_server.h"
#include "modifier_container_item.h"
#include "subautoload_container.h"
#include "time_scaler.h"

#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/nine_patch_rect.hpp"


#define SIGNAL_INVENTORY_CONTROL_DONE_TRANSITIONING "sicdt"


namespace Game::Item{
  class inventory_control: public godot::Control,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(inventory_control, godot::Control)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(inventory_control)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(inventory_control)
  ATTR_INTERFACE_HELPER_INHERIT(inventory_control)

    GD_PROPERTY_VAR(godot::String, UI_item_sprite_template_scene)
    GD_PROPERTY_VAR(godot::String, item_description_popout_scene)

    GD_PROPERTY_VAR(double, description_popout_idle_time)
    GD_PROPERTY_VAR(int, cursor_follower_item_sprite_size)

    GD_PROPERTY_VAR(godot::NodePath, animation_player)

    GD_PROPERTY_VAR(godot::NodePath, item_deleter_control)
    GD_PROPERTY_VAR(godot::NodePath, item_drop_area)
    GD_PROPERTY_VAR(godot::NodePath, main_icontainer_control)
    GD_PROPERTY_VAR(godot::NodePath, loadout_icontainer_control)

    private:
      struct _item_container_metadata{
        struct _object_data{
          Game::Item::item_container_control* control_node;
          uint64_t node_inst_id;
        };

        std::map<uint64_t, _object_data> _container_control_map;
        Game::Item::item_container_inst_id container_id;
      };

      Game::cursor_follower_control* _cursor_follower;
      Game::Item::item_server* _item_server;
      Game::time_scaler* _time_scaler;

      godot::Ref<godot::PackedScene> _item_sprite_template_scene;
      godot::Ref<godot::PackedScene> _item_description_popout_scene;

      godot::AnimationPlayer* _animation_player;

      Game::Item::item_container_control* _icontainer_main;
      Game::Item::item_container_control* _icontainer_loadout;

      Game::Item::item_position_data _current_grabbed;
      Game::Item::item_position_data _current_hover;

      godot::TextureRect* _cf_sprite_renderer;
      Game::sprite_resource_instancer* _cf_sprite_instancer;
      Game::Item::item_description_popout* _cf_item_desc_popout;

      godot::Timer* _popout_wait_timer;

      std::map<Game::Item::item_container_inst_id, _item_container_metadata*> _container_id_metadata_map;
      std::vector<std::shared_ptr<Game::Item::modifier_container_item>> _mod_cont_list;

      bool _is_drop_area_hovered;

      void _on_animation_finished(godot::String anim_name);

      void _on_popout_wait_timer_timeout();
  
      void _item_container_add(Game::Item::item_container_control* _control, Game::Item::item_container_inst_id id);
      void _item_container_remove(Game::Item::item_container_control* _control, Game::Item::item_container_inst_id id);
      void _item_container_control_on_mouse_hovered(gdsignal_param_ref param);
      void _item_container_control_on_mouse_exited();
      void _item_container_control_on_changed(gdsignal_param_ref param);
      void _item_container_control_on_exiting(gdsignal_param_ref param);

      void _drop_area_on_mouse_hovered();
      void _drop_area_on_mouse_exited();
      void _drop_grabbed_item();

      void _on_item_moved();

      void _bind_item_container_control(Game::Item::item_container_control* _control);
    
    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;

    public:
      inventory_control();

      void _notification(int code);
      
      void _process(double delta) override;

      void _gui_input(const godot::Ref<godot::InputEvent>& _event) override;

      void bind_icontainer_main(Game::Item::item_container_inst_id inst_id);
      void bind_icontainer_loadout(Game::Item::item_container_inst_id inst_id);

      void show();
      void hide();
  };
}

#endif