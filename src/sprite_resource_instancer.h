#ifndef SPRITE_RESOURCE_INSTANCER_HEADER
#define SPRITE_RESOURCE_INSTANCER_HEADER

#include "animation_interfacer.h"
#include "custom_memory.h"
#include "godot_macro.h"
#include "entity_data.h"
#include "entity_resource_data.h"
#include "I_entity_handler.h"
#include "I_item_handler.h"
#include "interface_helper.h"
#include "node_free_filter.h"
#include "parent_child_watcher_helper.h"
#include "subautoload_container.h"
#include "sprite_resource_data.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/viewport_texture.hpp"

#include "map"


namespace Game{
  class sprite_resource_instancer: public godot::Node,
    public GameUtils::Helper::parent_child_watcher_helper,
    public GameUtils::Interface::I_interface_class,
    public Game::Entity::I_entity_handler,
    public Game::Item::I_item_handler,
    public Game::I_subautoload_compatible{
  GDCLASS(sprite_resource_instancer, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(sprite_resource_instancer)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(sprite_resource_instancer)
  ATTR_INTERFACE_HELPER_INHERIT(sprite_resource_instancer)
  ATTR_PARENT_CHILD_WATCHER_HELPER_INHERIT(sprite_resource_instancer)

    GD_PROPERTY_VAR(godot::String, animation_interfacer_pckscene_path)
    GD_PROPERTY_VAR(godot::String, instance_animation_node_pckscene_path)
    GD_PROPERTY_VAR(godot::String, normal_case_pivot_node_pckscene_path)
    GD_PROPERTY_VAR(godot::String, subviewport_node_pckscene_path)

    private:
      godot::Ref<godot::PackedScene> 
        animation_interfacer_pckscene,
        instance_animation_node_pckscene,
        normal_case_pivot_node_pckscene,
        subviewport_node_pckscene
      ;

      godot::Node* _pivot_point = NULL;
      Game::Animation::animation_interfacer* _anim_interfacer = NULL;

      const Game::Item::item_object* _current_item_object;

      const Game::Entity::entity_data* _current_entity_data;
      const Game::Entity::entity_resource_data* _current_entity_res_data;

      // this custom_memory serve as fail save if xx_object already deleted
      std::shared_ptr<const GameUtils::Memory::custom_memory> _resource_data_cm;
      const Game::sprite_resource_data* _resource_data = NULL;

      // this is for setting two entity data and then trigger entity_changed once on _process
      bool _entity_updated = false;
      std::map<uint64_t, Game::Entity::I_entity_handler*> _interface_entity_list;
      std::map<uint64_t, Game::Item::I_item_handler*> _interface_item_list;

      void _create_resource_nodes();
      void _clear_resource_nodes();

      void _on_entity_changed();
      void _on_item_changed();

      void _notify_entity_handler_node(uint64_t node_id);
      void _notify_item_handler_node(uint64_t node_id);


      void _on_child_entered(godot::Node* child) override;
      void _on_child_exited(godot::Node* child) override;


      void _set_sprite_resource_data(const Game::sprite_resource_data* res_data);


    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;


    public:
      void _process(double delta) override;

      void _notification(int code);

      void set_item_object(const Game::Item::item_object* item) override;

      void set_entity_data(const Game::Entity::entity_data* data) override;
      void set_entity_resource_data(const Game::Entity::entity_resource_data* res_data) override;

      godot::Node* get_pivot_node();

      // useable when parent node is not a Node2D, else it will return a null reference
      godot::Ref<godot::ViewportTexture> as_viewport_texture();
      
      Game::Animation::animation_interfacer* get_animation_interface();
  };
}

#endif