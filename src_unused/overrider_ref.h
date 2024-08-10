#ifndef OVERRIDER_REF_HEADER
#define OVERRIDER_REF_HEADER

#include "defines.h"
#include "godot_macro.h"
#include "node_holder_helper.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/variant/string.hpp"


namespace Game::Overrider{
  class overrider_ref: public godot::Node, GameUtils::node_holder_helper{
  GDCLASS(overrider_ref, godot::Node)
    
    private:
      godot::Node* _override_node;
      godot::Ref<godot::PackedScene> _overrider_pckscene;
    
    protected:
      static void _bind_methods();

    public:
      bool instantiate_node();
      bool reparent_ref_to(godot::Node* parent, bool keep_global_transform = true);
    
      template<typename overrider_class> bool set_overrider_pckscene(godot::Ref<godot::PackedScene>& packed_scene){
        godot::Node* _check_node = NULL;

        _check_node = packed_scene->instantiate();
        if(_check_node->get_class() != overrider_class::get_class_static())
          goto on_error;

        _check_node->queue_free();
        _overrider_pckscene = packed_scene;
        return true;

        on_error:{
          if(_check_node)
            _check_node->queue_free();

          return false;
        }
      }
      
      template<typename node> node* get_overrider(){
        if(!_override_node)
          return NULL;

        if(node::get_class_static() != _override_node->get_class())
          return NULL;

        return (node*)_override_node;
      }
    
  };
}

#endif