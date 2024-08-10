#ifndef SUBAUTOLOAD_CONTAINER_HEADER
#define SUBAUTOLOAD_CONTAINER_HEADER

#include "logger.h"
#include "defines.h"
#include "godot_macro.h"
#include "godot_var_extended.h"
#include "interface_helper.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/string.hpp"

#include "map"


#define SIGNAL_SUBAUTOLOAD_ON_READY "ssaor"


#define GET_SUBAUTOLOAD(type_class, p_container, target) \
  target = (p_container)->get_singleton_node<type_class>(); \
  \
  if(target == NULL){ \
    INIT_ASSERT() \
    godot::Array _paramarr;{ \
      _paramarr.append(type_class::get_class_static()); \
    } \
    \
    ERR_ASSERT(godot::String("Cannot get '{0}' subautoload.").format(_paramarr)); \
    throw godot::Error::ERR_UNCONFIGURED; \
  }


namespace Game{
  class subautoload_container: public godot::Node{
  GDCLASS(subautoload_container, godot::Node)

    private:
      struct _autoload_metadata{
        godot::Node* _this_node;
        uint64_t _node_inst_id;
      };

      std::map<godot::String, _autoload_metadata> _nodes_map;

      void _check_autoloads();
      void _bind_autoload(godot::Node* node);

      void _child_call_bind(godot::Node* node);
      void _child_call_ready(godot::Node* node);

      void _check_child_recursively(godot::Node* from, bool is_autoload_layer = false);

      void _on_autoload_exiting_tree(godot::Node* node);

      void _on_child_added(godot::Node* child);
    
    protected:
      static void _bind_methods();
      void _bind_node(godot::Node* node);

    public:
      void _ready();

      void add_child_node(godot::Node* parent_node, godot::Node* target_child_node, bool force_readable_name = false, godot::Node::InternalMode internal = (godot::Node::InternalMode)0);

      // since the container is initiated last, the order of the child autoloads doens't matter
      template<typename T> T* get_singleton_node(){
        INIT_ASSERT()
        godot::Array _paramarr;{
          _paramarr.append(T::get_class_static());
        }

        LOG_ASSERT(String("get singleton {0}").format(_paramarr))

        auto _iter = _nodes_map.find(T::get_class_static());
        if(_iter == _nodes_map.end()){
          INIT_ASSERT()
          godot::Array _paramarr;{
            _paramarr.append(T::get_class_static());
          }
          
          ERR_ASSERT(String("Subautoload doesn't have {0} autoload node.").format(_paramarr))
          return NULL;
        }

        _paramarr.clear();{
          _paramarr.append((uint64_t)_iter->second._this_node);
        }

        LOG_ASSERT(String("address {0}").format(_paramarr))
        return (T*)_iter->second._this_node;
      }
  };

  
  class I_subautoload_compatible{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_subautoload_compatible)

    protected:
      virtual void _bind_subautoload(Game::subautoload_container* container){}
      virtual void _on_gamenode_ready(){}

    public:
      virtual Game::subautoload_container* get_subautoload_root(){ return NULL; }
      virtual void add_child_sa(godot::Node *node, bool force_readable_name = false, godot::Node::InternalMode internal = (godot::Node::InternalMode)0){}
  };
}


#define SUBAUTOLOAD_USER_BINDFUNC ___set_bound_subautoload
#define SUBAUTOLOAD_USER_BINDFUNCSTR ___MACRO_STRINGIFY(SUBAUTOLOAD_USER_BINDFUNC)

#define SUBAUTOLOAD_USER_READYFUNC ___on_ready_subautoload
#define SUBAUTOLOAD_USER_READYFUNCSTR ___MACRO_STRINGIFY(SUBAUTOLOAD_USER_READYFUNC)


#define ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(class_name) \
  private: \
    Game::subautoload_container* ___bound_subautoload = NULL; \
    bool ___is_ready = false; \
    void SUBAUTOLOAD_USER_BINDFUNC(gdvar_custom_object subautoload_ref); \
    void SUBAUTOLOAD_USER_READYFUNC(); \
    \
  public: \
    Game::subautoload_container* get_subautoload_root() override; \
    gdvar_custom_object get_subautoload_root_gdfunc(); \
    \
    void add_child_sa(godot::Node *node, bool force_readable_name = false, godot::Node::InternalMode internal = (godot::Node::InternalMode)0) override; 


#define ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(class_name) \
  void class_name::SUBAUTOLOAD_USER_BINDFUNC(gdvar_custom_object ___ref){ \
    if(___bound_subautoload) \
      return; \
    Game::subautoload_container* _container = GDVAR_CUSTOM_TO(___ref, Game::subautoload_container); \
    ___bound_subautoload = _container; \
    _bind_subautoload(_container); \
  } \
  \
  void class_name::SUBAUTOLOAD_USER_READYFUNC(){ \
    if(___is_ready) \
      return; \
    \
    ___is_ready = true; \
    _on_gamenode_ready(); \
  } \
  \
  Game::subautoload_container* class_name::get_subautoload_root(){ \
    return ___bound_subautoload; \
  } \
  \
  gdvar_custom_object class_name::get_subautoload_root_gdfunc(){ \
    return GDVAR_CUSTOM_FROM_PTR(___bound_subautoload); \
  } \
  \
  void class_name::add_child_sa(godot::Node *node, bool force_readable_name, godot::Node::InternalMode internal){ \
    get_subautoload_root()->add_child_node(this, node, force_readable_name, internal); \
  } 


#define ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(class_name) \
  godot::ClassDB::bind_method( \
    D_METHOD(SUBAUTOLOAD_USER_BINDFUNCSTR, \
      "container" \
    ), \
    &class_name::SUBAUTOLOAD_USER_BINDFUNC \
  ); \
  \
  godot::ClassDB::bind_method( \
    D_METHOD(SUBAUTOLOAD_USER_READYFUNCSTR), \
    &class_name::SUBAUTOLOAD_USER_READYFUNC \
  ); \
  \
  godot::ClassDB::bind_method( \
    D_METHOD("get_subautoload_root"), \
    &class_name::get_subautoload_root_gdfunc \
  ); 

#define ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)

#endif