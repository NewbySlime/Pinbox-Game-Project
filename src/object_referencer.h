#ifndef OBJECT_REFERENCER_HEADER
#define OBJECT_REFERENCER_HEADER

#include "predelete_notifier.h"

#include "godot_cpp/core/class_db.hpp"


#define ATTR_OBJECT_REFERENCER_INHERIT(origin_class) \
  private: \
    struct ___object_referencer_metadata{ \
      godot::Node* _this_node; \
      godot::Node** _reference_var; \
    }; \
 \
    std::map<uint64_t, ___object_referencer_metadata> ___object_referencer_metadata_map; \
    std::map<uint64_t, ___object_referencer_metadata> ___object_referencer_slow_metadata_map; \
 \
    void __object_referencer_on_node_exiting(); \
    void __object_referencer_on_node_predelete(gdvar_custom_object param); \
 \
  protected: \
    /* This will also change the reference if the target node is already added. */ \
    void _object_referencer_add_reference(godot::Node* node, godot::Node** reference_var); \
    void _object_referencer_remove_reference(godot::Node* node);


#define ATTR_OBJECT_REFERENCER_DEFINE(origin_class) \
  void origin_class::__object_referencer_on_node_exiting(){ \
    std::map<uint64_t, ___object_referencer_metadata> _list_delete; \
    for(auto _pair: ___object_referencer_slow_metadata_map){ \
      if(_pair.second._this_node->is_queued_for_deletion()) \
        _list_delete[_pair.first] = _pair.second; \
    } \
 \
    for(auto _pair: _list_delete){ \
      *_pair.second._reference_var = NULL; \
      ___object_referencer_slow_metadata_map.erase(_pair.first); \
    } \
  } \
 \
  void origin_class::__object_referencer_on_node_predelete(gdvar_custom_object param){ \
    predelete_notifier_signal_param *_param = GDVAR_CUSTOM_TO(param, predelete_notifier_signal_param); \
    uint64_t _node_id = _param->get_node()->get_instance_id(); \
 \
    auto _iter = ___object_referencer_metadata_map.find(_node_id); \
    if(_iter == ___object_referencer_metadata_map.end()) \
      return; \
 \
    *_iter->second._reference_var = NULL; \
    ___object_referencer_metadata_map.erase(_iter); \
  } \
 \
 \
  void origin_class::_object_referencer_add_reference(godot::Node* node, godot::Node** reference_var){ \
    uint64_t _node_id = node->get_instance_id(); \
 \
    ___object_referencer_metadata _metadata;{ \
      _metadata._this_node = node; \
      _metadata._reference_var = reference_var; \
    } \
 \
    /* put it in predelete method */ \
    if(node->has_signal(SIGNAL_PREDELETE_NOTIFIER_ON_PREDELETE)){ \
      node->connect(SIGNAL_PREDELETE_NOTIFIER_ON_PREDELETE, godot::Callable(this, "__object_referencer_on_node_predelete")); \
 \
      ___object_referencer_metadata_map[_node_id] = _metadata; \
    } \
    /* put it in slow method */ \
    else{ \
      node->connect("tree_exiting", godot::Callable(this, "__object_referencer_on_node_exiting")); \
 \
      ___object_referencer_slow_metadata_map[_node_id] = _metadata; \
    } \
  } \
 \
  void origin_class::_object_referencer_remove_reference(godot::Node* node){ \
    uint64_t _node_id = node->get_instance_id(); \
    {auto _iter = ___object_referencer_metadata_map.find(_node_id); \
      if(_iter != ___object_referencer_metadata_map.end()) \
        ___object_referencer_metadata_map.erase(_iter); \
    } \
 \
    {auto _iter = ___object_referencer_slow_metadata_map.find(_node_id); \
      if(_iter != ___object_referencer_slow_metadata_map.end()) \
        ___object_referencer_slow_metadata_map.erase(_iter); \
    } \
  }


#define ATTR_OBJECT_REFERENCER_BIND_GODOT(origin_class) \
  godot::ClassDB::bind_method(godot::D_METHOD("__object_referencer_on_node_predelete", "param"), &origin_class::__object_referencer_on_node_predelete); \
  godot::ClassDB::bind_method(godot::D_METHOD("__object_referencer_on_node_exiting"), &origin_class::__object_reference_on_node_exiting);
  

#define ATTR_OBJECT_REFERENCER_ON_NOTIFICATION(code)

#endif