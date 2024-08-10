#ifndef INTERFACE_HELPER_HEADER
#define INTERFACE_HELPER_HEADER

#include "algorithm_node.h"
#include "class_interface_database.h"
#include "defines.h"
#include "godot_var_extended.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/string.hpp"

#include "map"
#include "set"
#include "string"


/// @brief (any time) \n Macro for getting interface of a node object.
/// @param p_node pointer that represents Godot's node object.
/// @param interface_type the class type of the interface.
/// @param target variable to store the interface pointer.
#define INTERFACE_GET(p_node, interface_type, target){ \
  gdvar_custom_object _obj = p_node->call("interface_helper_get_interface", interface_type::get_id()); \
  target = (interface_type*)_obj; \
}

/// @brief (any time) \n Macro to check if the object class has the interface.
#define INTERFACE_CHECK(object_type, interface_type) \
  object_type::__##interface_type##_check()


/// @brief (important) (when declaring/at header) \n This is to define a class name of the interface.
/// @param interface_type the class type of the interface.
#define INTERFACE_DEFINE_INTERFACE_CLASS(interface_type) \
  public: \
    static godot::String get_id(){return #interface_type;} \
    static void __##interface_type##_check(){}



/// @brief (important) (when binding) \n A Macro for helping interface_helper bind with Godot's functionalities.
/// @param origin_class The class type.
#define ATTR_INTERFACE_HELPER_BIND_GODOT(origin_class)
  
  
/// @brief (important) (when declaring/at header) \n Macro to use when a class is inheriting from interface_helper.
#define ATTR_INTERFACE_HELPER_INHERIT(origin_class) \
  private: \
    std::shared_ptr<const std::set<godot::String>> __interface_list_map; \
    \
  protected: \
    void* __interface_helper_get_interface(const godot::String& interface_id) override; \
  private:


#define ATTR_INTERFACE_HELPER_DEFINE(origin_class) \
  void* origin_class::__interface_helper_get_interface(const godot::String& interface_id){ \
    if(!__interface_list_map.get()) \
      return NULL; \
    \
    auto _interface_iter = __interface_list_map->find(interface_id); \
    if(_interface_iter == __interface_list_map->end()) \
      return NULL; \
    \
    return this;\
  }


#define ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code) \
  switch(code){ \
    break; case NOTIFICATION_READY:{ \
      godot::Engine* _engine = godot::Engine::get_singleton(); \
      if(_engine && !_engine->is_editor_hint()){ \
        Game::class_interface_database* _database = Game::Misc::Algorithm::find_autoload<class_interface_database>(this, NULL); \
        if(!_database){ \
          INIT_ASSERT() \
          ERR_ASSERT("Cannot get class_interface_database") \
        } \
        else \
          __interface_list_map = _database->get_interface_list(get_class_static()); \
      } \
    } \
  }



#define ATTR_BASE_INTERFACE_HELPER_INHERIT(origin_class) \
  public: \
    gdvar_custom_object interface_helper_get_interface(godot::String interface_id); \
    \
  private:

#define ATTR_BASE_INTERFACE_HELPER_DEFINE(origin_class) \
  gdvar_custom_object origin_class::interface_helper_get_interface(godot::String interface_id){ \
    return GDVAR_CUSTOM_FROM_PTR(this->__interface_helper_get_interface(interface_id)); \
  }

#define ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(class_type) \
  godot::ClassDB::bind_method( \
    godot::D_METHOD( \
      "interface_helper_get_interface", \
      "interface_name" \
    ), \
    \
    &class_type::interface_helper_get_interface \
  );

#define ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)



namespace GameUtils::Interface{
  class I_interface_class{
    protected:
      virtual void* __interface_helper_get_interface(const godot::String& interface_id){ return NULL; }
  };
}

#endif