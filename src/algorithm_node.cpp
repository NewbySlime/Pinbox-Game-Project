#include "algorithm_node.h"

#include "set"


using namespace Game::Misc::Algorithm;
using namespace godot;



bool Game::Misc::Algorithm::is_node_in_child(Node* parent, Node* target_obj, bool check_recursive){
  int _child_count = parent->get_child_count();
  for(int i = 0; i < _child_count; i++){
    Node* _child = parent->get_child(i);
    if(_child->get_instance_id() == target_obj->get_instance_id())
      return true;

    if(check_recursive && is_node_in_child(_child, target_obj, check_recursive))
      return true;
  }

  return false;
}


void Game::Misc::Algorithm::remove_all_child_filter(Node* parent, void(*removed_callback)(Node*), std::vector<Node*>* filter){
  std::set<uint64_t> _id_list;
  if(filter){
    for(int i = 0; i < filter->size(); i++){
      Node* _filter_node = filter->operator[](i);
      _id_list.insert(_filter_node->get_instance_id());
    }
  }

  int _child_count = parent->get_child_count();
  for(int i = 0; i < _child_count; i++){
    Node* _child = parent->get_child(i);
    if(_id_list.size() > 0){
      uint64_t _child_inst_id = _child->get_instance_id();
      auto _iter = _id_list.find(_child_inst_id);

      // when the node is in the filter
      if(_iter != _id_list.end())
        continue;
    }

    parent->remove_child(_child);
    removed_callback(_child);
  }
}

void Game::Misc::Algorithm::remove_all_child(Node* parent, void(*removed_callback)(Node*)){
  remove_all_child_filter(parent, removed_callback);
}