#ifndef CLASS_INTERFACE_DATABASE_HEADER
#define CLASS_INTERFACE_DATABASE_HEADER

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/string.hpp"

#include "map"
#include "memory"
#include "set"


namespace Game{
  class class_interface_database: public godot::Node{
  GDCLASS(class_interface_database, godot::Node)
    
    private:
      std::map<godot::String, std::shared_ptr<std::set<godot::String>>> _class_interface_map;

      std::shared_ptr<std::set<godot::String>> _dummy_interface_map;

    protected:
      static void _bind_methods();


    public:
      class_interface_database();

      void _ready() override;

      bool has_interface_type(const godot::String& class_name, const godot::String& interface_name);

      std::shared_ptr<const std::set<godot::String>> get_interface_list(const godot::String& origin_class_name);

    private:
      template<typename origin_class, typename interface_class, typename... interface_Ts> void _add_interface_type(){
        {// scoping for reducing stack memory usage
          godot::String origin_class_name = origin_class::get_class_static();

          // WARN: don't use this, this is just to test the type in compile time
          origin_class* _test_tmp = NULL;

          // in compile time, this will result in error when the origin_class doens't publicly inherit the interface_class
          interface_class* _test_interface = _test_tmp;

    
          std::shared_ptr<std::set<godot::String>> _interface_data = NULL;
          auto _class_iter = _class_interface_map.find(origin_class_name);
          if(_class_iter == _class_interface_map.end()){
            _interface_data = std::make_shared<std::set<godot::String>>();
            _class_interface_map[origin_class_name] = _interface_data;
          }
          else
            _interface_data = _class_iter->second;

          godot::String _interface_class_name = interface_class::get_id();  
          _interface_data->insert(_interface_class_name);
        }

        if constexpr(sizeof...(interface_Ts) > 0)
          _add_interface_type<origin_class, interface_Ts...>();
      } 

    public:
      template<typename origin_class, typename... interface_Ts> void add_interface_type(){
        _add_interface_type<origin_class, interface_Ts...>();
      }

      template<typename origin_class, typename inherit_class> void inherit_interface(){
        // WARN: don't use this, this is just to test the type in compile time
        origin_class* _test_tmp = NULL;

        // in compile time, this will result int error when the origin_class doesn't publicly inherit the inherit_class
        inherit_class* _test_inherit = _test_tmp;

        auto _inherit_iter = _class_interface_map.find(inherit_class::get_class_static());
        if(_inherit_iter == _class_interface_map.end())
          return;

        std::shared_ptr<std::set<godot::String>> _interface_data;
        auto _interface_iter = _class_interface_map.find(origin_class::get_class_static());
        if(_interface_iter == _class_interface_map.end()){
          _interface_data = std::make_shared<std::set<godot::String>>();
          _class_interface_map[origin_class::get_class_static()] = _interface_data;
        }
        else
          _interface_data = _interface_iter->second;

        for(String _interface_classname: *_inherit_iter->second)
          _interface_data->insert(_interface_classname);
      }

      template<typename origin_class, typename interface_class> bool has_interface_type(){
        return has_interface_type(origin_class::get_class_static(), interface_type::get_id());
      }

      template<typename origin_class> std::shared_ptr<const std::set<godot::String>> get_interface_list(){
        return get_interface_list(origin_class::get_class_static());
      }
  };
}

#endif