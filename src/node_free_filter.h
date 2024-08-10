#ifndef NODE_FREE_FILTER_HEADER
#define NODE_FREE_FILTER_HEADER

#include "algorithm_node.h"
#include "logger.h"

#include "godot_cpp/classes/node.hpp"

#include "set"
#include "stdint.h"
#include "vector"


#define ATTR_NODE_FREE_FILTER_INHERIT(origin_class) \
  private: \
    std::set<uint64_t> _node_free_filter_map; \
 \
  public: \
    void add_node_free_filter(godot::Node* target_node); \
    void remove_node_free_filter(godot::Node* target_node); \
    void queue_free_filter();


#define ATTR_NODE_FREE_FILTER_DEFINE(origin_class) \
  void origin_class::add_node_free_filter(godot::Node* target_node){ \
    if(!Game::Misc::Algorithm::is_node_in_child(this, target_node)){ \
      INIT_ASSERT() \
      WARN_ASSERT("Added filter Node is not in Parent's child first layer.") \
      return; \
    } \
\
    _node_free_filter_map.insert(target_node->get_instance_id()); \
  } \
\
  void origin_class::remove_node_free_filter(godot::Node* target_node){ \
    auto _iter = _node_free_filter_map.find(target_node->get_instance_id()); \
    if(_iter == _node_free_filter_map.end()) \
      return; \
\
    _node_free_filter_map.erase(_iter); \
  } \
\
  void origin_class::queue_free_filter(){ \
    Node* _parent_node = get_parent(); \
\
    std::vector<Node*> _list_node; \
    for(int i = 0; i < _parent_node->get_child_count(); i++){ \
      Node* _check_node = _parent_node->get_child(i); \
      if(_node_free_filter_map.find(_check_node->get_instance_id()) != _node_free_filter_map.end()) \
        continue; \
\
      _list_node.insert(_list_node.end(), _check_node); \
    } \
\
    for(int i = 0; i < _list_node.size(); i++) \
      _parent_node->remove_child(_list_node[i]); \
  }


#define ATTR_NODE_FREE_FILTER_BIND_GODOT(origin_class)
  

#define ATTR_NODE_FREE_FILTER_ON_NOTIFICATION(notification_code) \
  switch(notification_code){ \
    break; case godot::Node::NOTIFICATION_READY:{ \
      add_node_free_filter(this); \
    } \
 \
    break; case godot::Node::NOTIFICATION_PARENTED:{ \
      _node_free_filter_map.clear(); \
      add_node_free_filter(this); \
    } \
  }

#endif