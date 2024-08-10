#ifndef EQUIPPABLE_OBJECT_HEADER
#define EQUIPPABLE_OBJECT_HEADER


#include "godot_macro.h"
#include "I_item_container.h"
#include "id_types.h"
#include "interface_helper.h"
#include "item_container.h"
#include "item_object.h"
#include "item_server.h"
#include "item_world_entity_2d_handler.h"
#include "modifiers.h"
#include "player_actor.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/variant/string.hpp"


#define SIGNAL_EQUIPPABLE_OBJECT_ON_DELETED "seood"


namespace Game{
  namespace Item::Factory{
    class equippable_factory;
  }

  class equippable_object: public godot::Node2D,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(equippable_object, godot::Node2D)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(equippable_object)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(equippable_object)
  ATTR_INTERFACE_HELPER_INHERIT(equippable_object)

    GD_PROPERTY_VAR(godot::String, default_action_id)

    private:
      const Game::Item::item_object* _current_item = NULL;

      double _set_angle = 0;


    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;
  
      // function for inheriting class
      virtual void _on_item_set(const Game::Item::item_object* item){}

    
    public:
      equippable_object();

      void _notifaction(int code);

      void _physics_process(double delta) override;

      void set_object_rotation_deg(double angle);
      double get_object_rotation();

      const Game::Item::item_object* get_item_object();

      virtual Game::I_action_class* get_primary_class(){ return NULL; }
      virtual Game::I_action_class* get_secondary_class(){ return NULL; }


    private:
      friend Game::Item::Factory::equippable_factory;
      // subautoloads always be added by the factory
      Game::Item::Factory::equippable_factory* _current_factory;
      Game::Item::item_world_entity_2d_handler* _iwe2d_handler;
      Game::Item::item_server* _item_server;

      void _set_item(const Game::Item::item_object* item);
      bool _set_item_id(Game::Item::item_inst_id item_id);

  };
}

#endif