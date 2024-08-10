#ifndef ALGORITHM_NODE_HEADER
#define ALGORITHM_NODE_HEADER

#include "logger.h"

#include "godot_cpp/classes/collision_shape2d.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/shape2d.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "vector"


namespace Game::Misc::Algorithm{
  /// @brief Find child Node in parent Node based on the type supplied.
  /// @tparam T The child type to find
  /// @param parent Target parent to find the child
  /// @param default_value The default value when the child Node of the type cannot be found
  /// @return Returns the child Node when found, on the contrary, default value will be returned
  template<typename T> T* find_child_node_type(godot::Node* parent, T* default_value = NULL){
    int _child_count = parent->get_child_count();
    for(int i = 0; i < _child_count; i++){
      godot::Node* _child_node = parent->get_child(i);
      if(_child_node->is_class(T::get_class_static()))
        return (T*)_child_node;
    }

    return default_value;
  }

  template<typename T> T* find_autoload(godot::Node* node_gateway, T* default_value = NULL){
    godot::Node* _root_node = node_gateway->get_node<godot::Node>("/root");
    if(!_root_node){
      INIT_ASSERT()
      ERR_ASSERT("Cannot get root node?")

      return default_value;
    }

    return find_child_node_type<T>(_root_node, default_value);
  }

  /// @brief Get the collision shape based on the Type
  /// @tparam T The collision shape type to get (based on Shape2D)
  /// @param shape The Node that contains the Shape2D
  /// @param default_value The default value when the shape is not the same as the type checking
  /// @return Returns the shape Object 
  template<typename T> T* get_collision_shape(godot::CollisionShape2D* shape, T* default_value = NULL){
    godot::Ref<godot::Shape2D> _shape_type = shape->get_shape();
    if(!_shape_type->is_class(T::get_class_static()))
      return default_value;

    return (T*)_shape_type.ptr();
  }
  
  /// @brief Function to check if the target Node is in the child area of parent Node
  /// @param parent The parent Node to check
  /// @param target_obj The child Node to check
  /// @param check_recursive Is recursive (not only in the one layer)
  /// @return Child Node is in parent or not
  bool is_node_in_child(godot::Node* parent, godot::Node* target_obj, bool check_recursive = false);

  /// @brief Function to remove all child in the parent Node. \n NOTE: this function will only detach child from parent. To free the child, use the removed_callback parameter.
  /// @param parent The parent Node 
  /// @param removed_callback Callback to get the removed child Node
  void remove_all_child(godot::Node* parent, void(*removed_callback)(godot::Node*));
  /// @brief Function to remove some child based on the filters provided, in the parent Node. \n NOTE: this function will only detach child from parent. To free the child, use the removed_callback parameter.
  /// @param parent The Parent Node
  /// @param removed_callback Callback to get the removed child Node
  /// @param filter Filter to exclude child from being detached from parent
  void remove_all_child_filter(godot::Node* parent, void(*removed_callback)(godot::Node*), std::vector<godot::Node*>* filter = NULL);
}

#endif