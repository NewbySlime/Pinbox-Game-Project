#include "register_types.h"

#include "animation_interfacer.h"
#include "breakable_object.h"
#include "bus_event.h"
#include "camera2d_entity.h"
#include "class_interface_database.h"
#include "cursor_follower_control.h"
#include "damage_indicator_handler.h"
#include "damage_indicator.h"
#include "damageable_object.h"
#include "entity_database.h"
#include "entity_object.h"
#include "entity_server.h"
#include "equippable_factory.h"
#include "equippable_object.h"
#include "flipper.h"
#include "flipper_handler.h"
#include "game_context_listener_helper.h"
#include "game_handler.h"
#include "game_persistance.h"
#include "game_scene_handler.h"
#include "hit_register_handler.h"
#include "hit_registers.h"
#include "inventory_control.h"
#include "item_container.h"
#include "item_container_action.h"
#include "item_container_control.h"
#include "item_container_delete.h"
#include "item_container_infinite.h"
#include "item_database.h"
#include "item_deleter_control.h"
#include "item_description_popout.h"
#include "item_dropper.h"
#include "item_frame_control.h"
#include "item_picker_area.h"
#include "item_pouch_factory.h"
#include "item_resource_database.h"
#include "item_server.h"
#include "item_world_entity_2d.h"
#include "item_world_entity_2d_handler.h"
#include "level_generator.h"
#include "logger.h"
#include "modifier_database.h"
#include "modifier_factory.h"
#include "node2d_object_follower.h"
#include "persistance_context.h"
#include "player_actor.h"
#include "player_spawner.h"
#include "rng_server.h"
#include "spawner.h"
#include "sprite_resource_instancer.h"
#include "static_level.h"
#include "static_variable_storage.h"
#include "structure_holder_node.h"
#include "subautoload_container.h"
#include "time_scaler.h"
#include "ui_container.h"
#include "weapon_factory.h"
#include "weapon_handler.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "set"


using namespace Game;
using namespace Game::Animation;
using namespace Game::Effect;
using namespace Game::Entity;
using namespace Game::Handlers;
using namespace Game::HitRegisters;
using namespace Game::Level;
using namespace Game::Persistance;
using namespace Game::Item;
using namespace Game::Item::Factory;
using namespace Game::UI;
using namespace GameUtils;
using namespace GameUtils::ParsingHelper;
using namespace GameUtils::Helper;
using namespace godot;


void register_types_interface(class_interface_database* database){
  database->add_interface_type<animation_interfacer,
    I_subautoload_compatible>();

  database->add_interface_type<camera2d_entity,
    I_subautoload_compatible>();

  database->add_interface_type<equippable_object,
    I_subautoload_compatible>();
  
  database->add_interface_type<cursor_follower_control,
    I_subautoload_compatible>();

  database->add_interface_type<damage_indicator,
    I_subautoload_compatible>();

  database->add_interface_type<damageable_object,
    I_vital_object>();

  database->add_interface_type<entity_actor,
    I_actor,
    I_subautoload_compatible>();

  database->add_interface_type<entity_object,
    I_entity_object,
    I_subautoload_compatible>();

  database->add_interface_type<entity_server,
    I_subautoload_compatible>();

  database->add_interface_type<equippable_factory,
    I_subautoload_compatible>();

  database->add_interface_type<equippable_object,
    I_subautoload_compatible>();

  database->add_interface_type<game_context_listener_helper,
    I_subautoload_compatible>();

  database->add_interface_type<game_handler,
    I_subautoload_compatible>();

  database->add_interface_type<game_scene_handler,
    I_subautoload_compatible>();

  database->add_interface_type<hit_register_handler,
    I_subautoload_compatible>();

  database->add_interface_type<inventory_control,
    I_subautoload_compatible>();

  database->add_interface_type<item_deleter_control,
    I_subautoload_compatible>();

  database->add_interface_type<item_frame_control,
    I_subautoload_compatible>();

  database->add_interface_type<item_pouch_factory,
    I_item_factory,
    I_subautoload_compatible>();

  database->add_interface_type<item_server,
    I_subautoload_compatible>();

  database->add_interface_type<item_world_entity_2d_handler,
    I_subautoload_compatible>();

  database->add_interface_type<item_world_entity_2d,
    I_subautoload_compatible>();

  database->add_interface_type<modifier_factory,
    I_item_factory,
    I_subautoload_compatible>();

  database->add_interface_type<node2d_object_follower,
    I_subautoload_compatible>();

  database->add_interface_type<persistance_context,
    I_subautoload_compatible>();

  database->add_interface_type<player_spawner,
    I_spawner,
    I_subautoload_compatible>();

  database->add_interface_type<rng_server,
    I_subautoload_compatible>();

  database->add_interface_type<sprite_resource_instancer,
    I_entity_handler,
    I_item_handler,
    I_subautoload_compatible>();

  database->add_interface_type<static_level,
    I_level,
    I_subautoload_compatible>();

  database->add_interface_type<ui_container,
    I_subautoload_compatible>();

  database->add_interface_type<weapon_factory,
    I_item_factory,
    I_subautoload_compatible>();

  database->inherit_interface<weapon_handler, equippable_object>();
}


void initialize_gdextension_module(ModuleInitializationLevel p_level) {
  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }


  // first layer of inheritance
  ClassDB::register_class<animation_interfacer>();
  ClassDB::register_class<bus_event>();
  ClassDB::register_class<camera2d_entity>();
  ClassDB::register_class<class_interface_database>();
  ClassDB::register_class<cursor_follower_control>();
  ClassDB::register_class<damage_indicator>();
  ClassDB::register_class<damage_indicator_handler>();
  ClassDB::register_class<damageable_object>();
  ClassDB::register_class<entity_actor>();
  ClassDB::register_class<entity_database>();
  ClassDB::register_class<entity_object>();
  ClassDB::register_class<entity_server>();
  ClassDB::register_class<equippable_factory>();
  ClassDB::register_class<equippable_object>();
  ClassDB::register_class<flipper>();
  ClassDB::register_class<flipper_handler>();
  ClassDB::register_class<game_context_listener_helper>();
  ClassDB::register_class<game_handler>();
  ClassDB::register_class<game_persistance>();
  ClassDB::register_class<game_scene_handler>();
  ClassDB::register_class<hit_register_handler>();
  ClassDB::register_class<inventory_control>();
  ClassDB::register_class<item_container>();
  ClassDB::register_class<item_container_control>();
  ClassDB::register_class<item_database>();
  ClassDB::register_class<item_deleter_control>();
  ClassDB::register_class<item_description_popout>();
  ClassDB::register_class<item_dropper>();
  ClassDB::register_class<item_frame_control>();
  ClassDB::register_class<item_picker_area>();
  ClassDB::register_class<item_resource_database>();
  ClassDB::register_class<item_server>();
  ClassDB::register_class<item_world_entity_2d>();
  ClassDB::register_class<item_world_entity_2d_handler>();
  ClassDB::register_class<logger>();
  ClassDB::register_class<modifier_database>();
  ClassDB::register_class<modifier_factory>();
  ClassDB::register_class<node2d_object_follower>();
  ClassDB::register_class<persistance_context>();
  ClassDB::register_class<player_spawner>();
  ClassDB::register_class<rng_server>();
  ClassDB::register_class<sprite_resource_instancer>();
  ClassDB::register_class<static_level>();
  ClassDB::register_class<static_variable_storage>();
  ClassDB::register_class<structure_holder_node>();
  ClassDB::register_class<subautoload_container>();
  ClassDB::register_class<time_scaler>();
  ClassDB::register_class<ui_container>();
  ClassDB::register_class<weapon_factory>();

  // second layer of inheritance
  ClassDB::register_class<breakable_object>();
  ClassDB::register_class<item_container_action>();
  ClassDB::register_class<item_container_delete>();
  ClassDB::register_class<item_container_infinite>();
  ClassDB::register_class<player_actor>();
  ClassDB::register_class<weapon_handler>();


  //ClassDB::register_class<level_generator>();

  // hit registers
  register_class_hit_registers();
}

void uninitialize_gdextension_module(ModuleInitializationLevel p_level) {
  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }
}

extern "C" {
  // Initialization.
  GDExtensionBool GDE_EXPORT gdextension_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_gdextension_module);
    init_obj.register_terminator(uninitialize_gdextension_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
  }
}