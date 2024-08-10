#ifndef PARENT_CHILD_WATCHER_HELPER_HEADER
#define PARENT_CHILD_WATCHER_HELPER_HEADER

#include "godot_cpp/classes/node.hpp"

#include "map"
#include "vector"


// NOTE: class that uses this have to use __child_watcher_change_parent function at least once to work
#define ATTR_PARENT_CHILD_WATCHER_HELPER_INHERIT(origin_class) \
  private: \
    godot::Node* ___child_watcher_parent_node = NULL; \
    std::map<uint64_t, godot::Node*> ___child_watcher_child_list; \
 \
    void __child_watcher_change_parent(godot::Node* parent_node); \
 \
    void __child_watcher_child_reordered(); \
 \
    void __child_watcher_parent_exiting_tree();

  
#define ATTR_PARENT_CHILD_WATCHER_HELPER_DEFINE(origin_class) \
void origin_class::__child_watcher_change_parent(godot::Node* parent_node){ \
  godot::Callable _child_reordered_callable = godot::Callable(this, "__child_watcher_change_reordered"); \
  godot::Callable _parent_removed_callable = godot::Callable(this, "__child_watcher_parent_exiting_tree"); \
 \
  ___child_watcher_child_list.clear(); \
  if(___child_watcher_parent_node){ \
    ___child_watcher_parent_node->disconnect("child_order_changed", _child_reordered_callable); \
    ___child_watcher_parent_node->disconnect("tree_exiting", _parent_removed_callable); \
  } \
 \
  if(parent_node){ \
    parent_node->connect("child_order_changed", _child_reordered_callable); \
    parent_node->connect("tree_exiting", _parent_removed_callable); \
  } \
 \
  ___child_watcher_parent_node = parent_node; \
  __child_watcher_child_reordered(); \
} \
 \
void origin_class::__child_watcher_child_reordered(){ \
  if(!___child_watcher_parent_node){ \
    ___child_watcher_child_list.clear(); \
 \
    return; \
  } \
 \
  int _child_count = ___child_watcher_parent_node->get_child_count(); \
 \
  /* also as exited list */ \
  std::map<uint64_t, godot::Node*> _tmp_list = ___child_watcher_child_list; \
 \
  std::vector<godot::Node*> _entered_list; \
  for(int i = 0; i < _child_count; i++){ \
    godot::Node* _curr_child = ___child_watcher_parent_node->get_child(i); \
    auto _iter = ___child_watcher_child_list.find(_curr_child->get_instance_id()); \
    if(_iter == ___child_watcher_child_list.end()) \
      _entered_list.insert(_entered_list.end(), _curr_child); \
    else \
      _tmp_list.erase(_curr_child->get_instance_id()); \
  } \
 \
  for(int i = 0; i < _entered_list.size(); i++){ \
    godot::Node* _curr_child = _entered_list[i]; \
    ___child_watcher_child_list[_curr_child->get_instance_id()] = _curr_child; \
 \
    this->_on_child_entered(_curr_child); \
  } \
 \
  for(auto _pair: _tmp_list){ \
    ___child_watcher_child_list.erase(_pair.first); \
 \
    this->_on_child_exited(_pair.second); \
  } \
} \
 \
void origin_class::__child_watcher_parent_exiting_tree(){ \
  godot::Node* _parent_node = ___child_watcher_parent_node; \
  __child_watcher_change_parent(NULL); \
 \
  this->_on_parent_removed(_parent_node); \
}


#define ATTR_PARENT_CHILD_WATCHER_HELPER_BIND_GODOT(origin_class) \
  ClassDB::bind_method(D_METHOD("__child_watcher_child_reordered"), &origin_class::__child_watcher_child_reordered); \
  ClassDB::bind_method(D_METHOD("__child_watcher_parent_exiting_tree"), &origin_class::__child_watcher_parent_exiting_tree);


#define ATTR_PARENT_CHILD_WATCHER_HELPER_ON_NOTIFICATION(code)



namespace GameUtils::Helper{
  class parent_child_watcher_helper{
    protected:
      virtual void _on_child_entered(godot::Node* child){}
      virtual void _on_child_exited(godot::Node* child){}

      virtual void _on_parent_removed(godot::Node* parent){}
  };
}

#endif