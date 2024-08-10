#ifndef ITEM_CONTAINER_CONTROL_HEADER
#define ITEM_CONTAINER_CONTROL_HEADER

#include "godot_macro.h"
#include "godot_signal.h"
#include "subautoload_container.h"

#include "item_container.h"
#include "item_frame_control.h"
#include "item_server.h"
#include "item_location_data_struct.h"
#include "predelete_notifier.h"

#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/v_box_container.hpp"
#include "godot_cpp/classes/h_box_container.hpp"
#include "godot_cpp/classes/scroll_container.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/variant/node_path.hpp"

#include "vector"


#define SIGNAL_ITEM_CONTAINER_CONTROL_ON_MOUSE_HOVERED "siccomh"
#define SIGNAL_ITEM_CONTAINER_CONTROL_ON_MOUSE_EXITED "siccomex"

#define SIGNAL_ITEM_CONTAINER_CONTROL_ON_CONTAINER_CHANGED "siccocc"

namespace Game::Item{
  class item_container_control: public godot::Control,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(item_container_control, godot::Control)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(item_container_control)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(item_container_control)
  ATTR_INTERFACE_HELPER_INHERIT(item_container_control)
  ATTR_PREDELETE_NOTIFIER_INHERIT(item_container_control)
  
    GD_PROPERTY_VAR(int, max_size)
    GD_PROPERTY_VAR(uint32_t, frame_horizontal_count);
    GD_PROPERTY_VAR(godot::NodePath, scroll_container_path)

    GD_PROPERTY_VAR(godot::String, item_frame_control_scene)

    private:
      Game::Item::item_server* _item_server;

      godot::VBoxContainer* _v_container;
      std::vector<godot::HBoxContainer*> _h_containers;
      std::vector<Game::Item::item_frame_control*> _item_frames;

      godot::Ref<godot::PackedScene> _item_frame_control_scene;

      godot::ScrollContainer* _scroll_container;

      godot::Node* _bound_item_container_node = NULL;
      Game::Item::I_item_container* _bound_item_container = NULL;

      int _frame_size;


      void _item_container_on_index_changed(uint64_t _param);
      void _item_container_on_container_resized();

      void _item_frame_control_on_mouse_entered(int index);
      void _item_frame_control_on_mouse_exited();

      void _update_container_size();

      godot::VBoxContainer* _create_new_vbox_cont();
      godot::HBoxContainer* _create_new_hbox_cont();

      void _delete_vbox_cont(godot::VBoxContainer* _v_cont);
      void _delete_hbox_cont(godot::HBoxContainer* _h_cont);

      Game::Item::item_frame_control* _create_new_item_frame_control(godot::HBoxContainer* parent_hbox);
      void _delete_item_frame_control(Game::Item::item_frame_control* item_frame);

      void _disconnect_container_signal(godot::Node* _container_node);
      void _connect_container_signal(godot::Node* _container);

      void _resize_to_new_container(Game::Item::I_item_container* _container);
      void _update_item_data(int index);
      void _update_all_item_data();

      void _on_container_deleted(gdsignal_param_ref param);

      void _on_resized();

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;

    public:
      item_container_control();

      void _notification(int code);
      
      void set_reference(godot::Node* container_node, Game::Item::I_item_container* container);
      void set_reference(Game::Item::item_container_inst_id id);

      Game::Item::item_container_inst_id get_reference_container_id();

      void hide_item(int index);
      void show_item(int index);
  };

  
  class item_container_control_on_mouse_hovered_param{
  
    private:
      friend item_container_control;

      Game::Item::item_position_data _pos_data;

    public:
      const Game::Item::item_position_data* get_pos_data(){ return &_pos_data; }
  };

  class item_container_control_on_container_changed{

    private:
      friend item_container_control;

      Game::Item::item_container_control* _control;

      Game::Item::item_container_inst_id prev_cont;
      Game::Item::item_container_inst_id new_cont;

    public:
      Game::Item::item_container_control* get_control_node(){ return _control; }

      Game::Item::item_container_inst_id get_prev_cont_id(){ return prev_cont; }
      Game::Item::item_container_inst_id get_new_cont_id(){ return new_cont; }
  };
}

#endif