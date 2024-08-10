#ifndef CAMERA2D_ENTITY_HEADER
#define CAMERA2D_ENTITY_HEADER

#include "entity_server.h"
#include "godot_var_extended.h"
#include "id_types.h"
#include "interface_helper.h"
#include "node2d_object_follower.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/camera2d.hpp"
#include "godot_cpp/classes/packed_scene.hpp"

#include "map"


namespace Game::Entity{
  class camera2d_entity: public godot::Camera2D,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(camera2d_entity, godot::Camera2D)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(camera2d_entity)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(camera2d_entity)  
  ATTR_INTERFACE_HELPER_INHERIT(camera2d_entity)  

    GD_PROPERTY_VAR(godot::String, entity_follower_pckscene_path) 

    private:
      Game::Entity::entity_server* _entity_server;
      godot::Ref<godot::PackedScene> _entity_follower_pckscene;

      // node instance id, entity instance id
      std::map<uint64_t, Game::Entity::entity_inst_id> _node_inst_map;

      // entity instance id, follower node
      std::map<Game::Entity::entity_inst_id, Game::node2d_object_follower*> _entity_follower_inst_map;
      
      godot::Vector2 _target_follow;
      Game::Entity::entity_inst_id _last_follow_entity;

      Game::Entity::entity_inst_id _player_entity_id;

      void _on_node_follower_deleted(Game::Entity::entity_inst_id inst_id);
      void _on_node_follower_position_update(gdvar_custom_object data);


    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* _container) override;
      void _on_gamenode_ready() override;


    public:
      camera2d_entity();

      void _notification(int code);

      void _process(double delta) override;

      bool follow_entity(Game::Entity::entity_inst_id entity_id, bool force_smooth = false);

      bool set_player_entity_id(Game::Entity::entity_inst_id entity_id);
      void switch_to_player_entity(bool force_smooth = false);

  };
}

#endif