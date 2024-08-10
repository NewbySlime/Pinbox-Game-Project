#ifndef ALGORITHM_NODE_STRUCTURE_HEADER
#define ALGORITHM_NODE_STRUCTURE_HEADER

#include "godot_cpp/classes/node.hpp"

#include "map"
#include "string"
#include "vector"


namespace Game::Misc::Algorithm::Structure{
  /// @brief A class to create a reference tree of a node. This class also provide the function to check if the target Node has the same structure of the reference tree (target Node as the root).
  class tree_branch_node{
    private:
      std::map<std::string, std::vector<tree_branch_node*>> _branches;

      bool _check_branch(godot::Node* parent, tree_branch_node* branch);

    public:
      /// @brief Copy class function, not prefered to use generic copy function.
      /// @param branch The branch to copy from
      tree_branch_node(const tree_branch_node& branch);
      /// @brief Creating branch based on the structure, can be created as the node.
      /// @param branch The branch represented in std::map
      tree_branch_node(std::map<std::string, std::vector<tree_branch_node>> branch);
      /// @brief Empty tree
      tree_branch_node(){}
      /// @brief Destructor for removing the dynamically created memory.
      ~tree_branch_node();

      /// @brief Function to check if the parent has the same structure to the reference tree.
      /// @param parent The target Node as a root to check from
      /// @return If the target Node has the same structure to the treed
      bool check_branch(godot::Node* parent);
  };
}

#endif