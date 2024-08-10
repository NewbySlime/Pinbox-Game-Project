#include "defines.h"
#include "algorithm_node_structure.h"

#include "godot_cpp/variant/typed_array.hpp"

#include "set"


using namespace Game::Misc::Algorithm::Structure;
using namespace godot;


bool tree_branch_node::_check_branch(Node* parent, tree_branch_node* branch){
  int _child_count = parent->get_child_count();

  {// (DFS algo)
    // checked so the algorithm doesn't have to use the branch again
    std::map<std::string, std::set<int>> _checked_index;

    for(int i = 0; i < _child_count; i++){
      Node* _child = parent->get_child(i);
      std::string _child_class_name = GDSTR_TO_STDSTR(_child->get_class());
      
      auto _iter = branch->_branches.find(_child_class_name);

      if(_iter == branch->_branches.end())
        continue;
        // not returning due to the algorithm will treat not same class as unnecessary class
        // return false;
      
      _checked_index[_child_class_name] = std::set<int>();
      std::set<int>* _index_set = &_checked_index[_child_class_name];

      std::vector<tree_branch_node*>* _branch_node_data = &_iter->second;
      for(int i = 0; i < _branch_node_data->size(); i++){
        // the algorithm tolerates if the child with same type is more than expected
        if(_branch_node_data->size() == _index_set->size())
          break;

        auto _iter_index_set = _index_set->find(i);
        if(_iter_index_set == _index_set->end()){
          tree_branch_node* _child_branch_data = _branch_node_data->operator[](i);
          if(_check_branch(_child, _child_branch_data)){
            _index_set->insert(i);
            break;
          }
        }

        if(i == (_branch_node_data->size()-1))
          return false;
      }
    }

    // if the size of the child of the node is not the same as the branch's
    if(_checked_index.size() != branch->_branches.size())
      return false;

    // check the same child count
    for(auto _pair: _checked_index){
      auto _iter = branch->_branches.find(_pair.first);
      if(_iter == branch->_branches.end())
        return false;

      if(_pair.second.size() != _iter->second.size())
        return false;
    }
  }

  return true;
}


tree_branch_node::tree_branch_node(const tree_branch_node& branch){
  _branches = branch._branches;
}

tree_branch_node::tree_branch_node(std::map<std::string, std::vector<tree_branch_node>> branch){
  for(auto _pair: branch){
    auto _iter = _branches.find(_pair.first);
    if(_iter == _branches.end()){
      _branches[_pair.first] = std::vector<tree_branch_node*>();
      _iter = _branches.find(_pair.first);
    }

    _iter->second.resize(_pair.second.size());
    for(int i = 0; i < _pair.second.size(); i++)
      _iter->second[i] = new tree_branch_node(_pair.second[i]);
  }
}

tree_branch_node::~tree_branch_node(){
  for(auto _pair: _branches)
    for(int i = 0; i < _pair.second.size(); i++)
      delete _pair.second[i];
}


bool tree_branch_node::check_branch(Node* parent){
  return _check_branch(parent, this);
}