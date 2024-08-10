#ifndef DEFINES_HEADER
#define DEFINES_HEADER


#ifdef _WIN64
#include "windows.h"

#define __LOCK_MUTEX(mutex) WaitForSingleObject(mutex, INFINITE)
#define __RELEASE_MUTEX(mutex) ReleaseMutex(mutex)
#endif



// collision layers
#define COLLISION_LAYER_STATIC_LAYER        0x00000001
#define COLLISION_LAYER_COLLISION_LAYER     0x00000002
#define COLLISION_LAYER_ITEM_LAYER          0x00000004
#define COLLISION_LAYER_PROJECTILE_LAYER    0x20000000
#define COLLISION_LAYER_ENEMY_LAYER         0x40000000
#define COLLISION_LAYER_PLAYER_LAYER        0x80000000


// format:
//  0 - The save identifier
#define SAVEFILE_PATH "user://players/{0}/save.dat"


#define CHARSYMBOL_KEY_SPLIT "."


#define VARNAME_COMBO_TRACKER_LOGO_SPRITE_DIR "logo_sprite_dir"
#define VARNAME_COMBO_TRACKER_BASE_LIMIT "base_limit"
#define VARNAME_COMBO_TRACKER_BASE_MULTIPLIER "base_multiplier"
#define VARNAME_COMBO_TRACKER_MAX_MULTIPLIER "max_multiplier"
#define VARNAME_COMBO_TRACKER_MULTIPLIER_DECREMENT "multiplier_decrement"

#define VARNAME_ITEM_DATA_NAME "name"
#define VARNAME_ITEM_DATA_DESCRIPTION "desc"

#define VARNAME_WEAPON_STATS "weapon_stats"
#define VARNAME_WEAPON_STATS_TYPE_PRIMARY VARNAME_WEAPON_STATS ":primary"
#define VARNAME_WEAPON_STATS_TYPE_SECONDARY  VARNAME_WEAPON_STATS ":secondary"
  #define VARNAME_WEAPON_STATS_DAMAGE "damage"
  #define VARNAME_WEAPON_STATS_ENERGY_COST "energy_cost"
  #define VARNAME_WEAPON_STATS_ALTERNATE "alternate_data"
    #define VARNAME_WEAPON_STATS_ALTERNATE_WEAPON_STATS VARNAME_WEAPON_STATS
  #define VARNAME_WEAPON_STATS_HITREG_DATA "hitreg_data"
    #define VARNAME_WEAPON_STATS_HITREG_DATA_TYPE "type"
    #define VARNAME_WEAPON_STATS_HITREG_DATA_MAX_LENGTH "max_length"
    #define VARNAME_WEAPON_STATS_HITREG_DATA_TIME "time"
    #define VARNAME_WEAPON_STATS_HITREG_DATA_NEXT_NODE "next_node"
  #define VARNAME_WEAPON_STATS_CHARGE_DELAY "charge_delay"
  #define VARNAME_WEAPON_STATS_FIRE_MODE "fire_mode"
    #define VARNAME_WEAPON_STATS_FIRE_MODE_SINGLE "single_fire"
    #define VARNAME_WEAPON_STATS_FIRE_MODE_BURST "burst_fire"
    #define VARNAME_WEAPON_STATS_FIRE_MODE_AUTOMATIC "automatic"
    #define VARNAME_WEAPON_STATS_FIRE_MODE_FULL_EXHAUST "full_exhaust"
    #define VARNAME_WEAPON_STATS_FIRE_MODE_CHARGE "charge"
  #define VARNAME_WEAPON_STATS_FIRE_RATE "fire_rate"
  #define VARNAME_WEAPON_STATS_TAP_RATE "tap_rate"
  #define VARNAME_WEAPON_STATS_BURST_COUNT "burst_count"
#define VARNAME_CONSUMABLE_STATS "consumable_stats"
  #define VARNAME_CONSUMABLE_STATS_HEAL "heal"
  #define VARNAME_CONSUMABLE_STATS_IS_THROWABLE "is_throwable"
#define VARNAME_ITEM_POUCH_DATA "item_pouch_data"
  #define VARNAME_ITEM_POUCH_DATA_SIZE "pouch_size"
  #define VARNAME_ITEM_POUCH_DATA_TYPE "container_type"
  #define VARNAME_ITEM_POUCH_DATA_FILTER "item_filter"

#define VARNAME_ITEM_RESOURCE_DATA "item_resource_data"
  #define VARNAME_ITEM_RES_EQUIPPABLE_SPRITE_DATA "equippable_sprite",
  #define VARNAME_ITEM_RES_ITEM_SPRITE_DATA "item_sprite"
#define VARNAME_SPRITE_RESOURCE_DATA_STATIC "static_sprite"
#define VARNAME_SPRITE_RESOURCE_DATA_ANIMATED "animated_sprite"
#define VARNAME_SPRITE_RESOURCE_DATA_ANIMATION_LIB "animation_library"
#define VARNAME_SPRITE_RESOURCE_DATA_CUSTOM "custom_object_sprite"

#define VARNAME_VITAL_INFO "vital_info"
  #define VARNAME_VITAL_INFO_MAX_HEALTH "max_health_point"
  #define VARNAME_VITAL_INFO_MAX_ENERGY "max_energy_point"
  #define VARNAME_VITAL_INFO_EP_RESTORE_DELAY "ep_restore_delay"
  #define VARNAME_VITAL_INFO_EP_RESTORE_RATE "ep_restore_rate"

#define VARNAME_INVENTORY_CONFIG "inventory_config"
  #define VARNAME_INVENTORY_CONFIG_TYPE "inventory_type"
  #define VARNAME_INVENTORY_CONFIG_SIZE "size"

#define VARNAME_ENTITY_DATA "entity_data"
  #define VARNAME_ENTITY_DATA_NAME "name"
  #define VARNAME_ENTITY_DATA_DESCRIPTION "description"
  #define VARNAME_ENTITY_DATA_TYPE_CLASS "type_class"
  #define VARNAME_ENTITY_DATA_VITAL_INFO VARNAME_VITAL_INFO
  #define VARNAME_ENTITY_DATA_INVENTORY_CONFIG "inventory"
  #define VARNAME_ENTITY_DATA_LOADOUT_INVENTORY_CONFIG "loadout_inventory"


#define VARNAME_MODIFIER_DATA_CHANCE_SCORE "chance_score"
#define VARNAME_MODIFIER_DATA_MODIFIER_DATA "modifier_data"

#define VARNAME_ITEM_DATA_IMAGE_PATH "image_path"

#define VARNAME_ITEM_CONTAINER_PREMADE_DATA_SIZE "size"

#define ITEM_DATA_HITREG_DATA_START_ID "0"

#define ITEM_TYPENAME_CONSUMABLE "consumable"
#define ITEM_TYPENAME_MATERIAL "material"
#define ITEM_TYPENAME_MODIFIER "modifier"
#define ITEM_TYPENAME_WEAPON "weapon"

#define ENTITY_TYPENAME_ENEMY "enemy"
#define ENTITY_TYPENAME_NPC "npc"
#define ENTITY_TYPENAME_PLAYER "player"


#define VARNAME_ENTITY_SCENE_CLASS_PATH "scene_path"
  


#define _CREATE_CHECKER_DICT1(key, varname, type)\
  if(_v.get_type() != type){\
    _paramarr.clear();{\
      _paramarr.append(key);\
      _paramarr.append(varname);\
      _paramarr.append(Variant::get_type_name(type));\
    }\
    \
    throw Game::Error::gdstr_exception(String("({0}:{1}) is not {2}.").format(_paramarr));\
  }

#define _CREATE_CHECKER_DICT2(key, varname, type)\
  _v = _data_part[varname];\
  _CREATE_CHECKER_DICT1(key, varname, type)

#define _CREATE_CHECKER_PACKED_SCENE(pck, classname)\
  if(!pck->is_class(#classname)){\
    _paramarr.clear();{\
      _paramarr.append(pck->get_path());\
      _paramarr.append(#classname);\
    }\
    throw Game::Error::gdstr_exception(String("File '{0}' is not {1}.").format(_paramarr));\
  }

#define CREATE_LOAD_RESOURCE_CHECKER(target, path, type) {\
  godot::ResourceLoader* __macro_res_loader = godot::ResourceLoader::get_singleton();\
  godot::Ref<godot::Resource> __macro_var_res = __macro_res_loader->load(path);\
  target = Ref<type>();\
  if(__macro_var_res.ptr()){\
    if(__macro_var_res->is_class(#type))\
      target = __macro_var_res;\
    else{\
      _paramarr.clear();{\
        _paramarr.append(path);\
        _paramarr.append(#type);\
      }\
      ERR_ASSERT(String("Resource ({0}) loaded is not a type of {1}.").format(_paramarr))\
    }\
  }\
  else{\
    _paramarr.clear();{\
      _paramarr.append(path);\
    }\
    ERR_ASSERT(String("Resource ({0}) cannot be loaded.").format(_paramarr))\
  }\
}

#define CREATE_LOAD_PACKED_SCENE_CHECKER(target, path, instantiated_type)\
  CREATE_LOAD_RESOURCE_CHECKER(target, path, PackedScene)\
  if(target.is_valid()){\
    Node* __macro_test_node = target->instantiate();\
    if(!__macro_test_node->is_class(instantiated_type::get_class_static())){\
      ERR_ASSERT(String("Instantiated godot::PackedScene is not a(n) " #instantiated_type))\
    }\
  }

#define CREATE_GET_NODE_TESTER(target, node_path, type){\
    target = NULL;\
    godot::Node* __macro_test_node = get_node<godot::Node>(node_path);\
    if(__macro_test_node){\
      if(__macro_test_node->is_class(#type))\
        target = (type*)__macro_test_node;\
      else{\
        _paramarr.clear();{\
          _paramarr.append(node_path);\
          _paramarr.append(#type);\
        }\
        \
        ERR_ASSERT(String("Fetched node (path: {0}), isn't a type of '{1}'.").format(_paramarr))\
      }\
    }\
    else{\
      _paramarr.clear();{\
        _paramarr.append(node_path);\
      }\
      \
      ERR_ASSERT(String("Cannot get specified node. Path: '{0}'").format(_paramarr))\
    }\
  }


#define PROCESS_JSON_PATH(json_path, process_func){\
    Ref<FileAccess> _data = FileAccess::open(json_path, FileAccess::READ);\
    if(_data.is_null()){\
      _paramarr.clear();{\
        _paramarr.append(#json_path);\
        _paramarr.append(json_path);\
      }\
      ERR_ASSERT(String("{0} (dir: '{1}') is not a file").format(_paramarr))\
    }\
    else{\
      JSON _json_data = JSON();\
      _json_data.parse(_data->get_as_text());\
      Dictionary _dict_data = _json_data.get_data();\
      process_func(&_dict_data);\
    }\
  }



#define GAME_CONTEXT_SCENE_FOLDER "res://scene/game_context_scenes"

#define PARSE_GAME_CONTEXT_SCENE_PATH(target_var, context_id, class_type) \
  { \
    Array __macro_paramarr;{ \
      __macro_paramarr.append(context_id); \
      __macro_paramarr.append(class_type); \
    } \
    \
    target_var = String(GAME_CONTEXT_SCENE_FOLDER "/{0}/{1}.tscn").format(__macro_paramarr); \
  }

#define GDSTR_TO_STDSTR(gdstr) std::string((char*)gdstr.to_ascii_buffer().ptr(), gdstr.length())

#endif