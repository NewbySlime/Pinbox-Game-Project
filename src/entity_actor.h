#ifndef ENTITY_ACTOR_HEADER
#define ENTITY_ACTOR_HEADER

#include "entity_server.h"
#include "I_actor.h"
#include "interface_helper.h"
#include "subautoload_container.h"


namespace Game::Entity{
  class entity_actor: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_actor,
    public Game::I_subautoload_compatible {
  GDCLASS(entity_actor, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(entity_actor)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(entity_actor)
  ATTR_INTERFACE_HELPER_INHERIT(entity_actor)

    private:
      struct _overrider_metadata{
        godot::Ref<godot::PackedScene> overrider_packed_scene;
        godot::Node* _current_instance = NULL;
      };

      Game::Entity::entity_server* _entity_server;
      Game::Entity::entity_inst_id _bound_entity_id;

      std::map<godot::String, _overrider_metadata*> _overrider_metadata_map;
      std::map<uint64_t, godot::String> _instanced_overrider_backreference_map;

      void __metadata_delete(uint64_t id);
      void __metadata_delete(godot::String associated_id);

      // this will create overriders, or reparent when exists
      void _add_overrider_to_entity(Game::Entity::entity_object* _entity_object);
      godot::Node* _create_overrider(godot::String associated_id);

      void _on_overrider_freed(gdvar_custom_object param);

      // listens from damageable_object (if possible)
      void _on_entity_killed();
      // listens from entity_object
      void _on_entity_despawned();

    
    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;

      virtual void _on_entity_killed_callback(){}
      virtual void _on_entity_despawned_callback(){}

      // supplied overrider should have overrider_base as base class and also predelete_notifier attribute signals
      bool _add_overrider(godot::String associated_id, godot::Ref<godot::PackedScene> overrider_pckscene);
      void _remove_overrider(godot::String associated_id);

    
    public:
      void _notification(int code);

      void bind_to_entity(Game::Entity::entity_inst_id entity_id) override;
  };
}

#endif