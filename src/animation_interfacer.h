#ifndef ANIMATION_INTERFACER_HEADER
#define ANIMATION_INTERFACER_HEADER

#include "I_animation.h"
#include "interface_helper.h"
#include "parent_child_watcher_helper.h"
#include "static_variable_storage.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/node.hpp"

#include "map"


namespace Game::Animation{
  class animation_interfacer: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible,
    public GameUtils::Helper::parent_child_watcher_helper
  {
  GDCLASS(animation_interfacer, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(animation_interfacer)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(animation_interfacer)
  ATTR_INTERFACE_HELPER_INHERIT(animation_interfacer)
  ATTR_PARENT_CHILD_WATCHER_HELPER_INHERIT(animation_interfacer)

    private:
      std::shared_ptr<GameUtils::Memory::custom_memory> _static_data;

      struct _interface_metadata{
        godot::Node* _this_node;
        Game::Animation::I_animation* _animation_interface;

        bool _using_own_interfacer = false;
      };

      std::map<uint64_t, _interface_metadata> _node_metadata;


      void _on_child_entered(godot::Node* child) override;
      void _on_child_exited(godot::Node* child) override;

      void _remove_all_metadata();


    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;

    public:
      void _notification(int code);

      void play(godot::StringName name = "", float custom_blend = -1, float custom_speed = 1, bool from_end = false);
      void play_backwards(godot::StringName name = "", float custom_blend = -1);

      void pause();
      void stop();

      std::map<uint64_t, bool> is_playing() const;

      std::map<uint64_t, float> get_speed_scale() const;
      void set_speed_scale(float speed);

      void seek(float seconds, bool update = false, bool update_only = false);
      std::map<uint64_t, float> get_animation_length() const;
      std::map<uint64_t, float> get_animation_position() const;

      std::map<uint64_t, bool> get_looping() const;
      void set_looping(bool looping);


      std::map<uint64_t, bool> get_centered() const;
      void set_centered(bool centered);

      std::map<uint64_t, bool> get_flip_h() const;
      void set_flip_h(bool flip);

      std::map<uint64_t, bool> get_flip_v() const;
      void set_flip_v(bool flip);

      std::map<uint64_t, godot::Vector2> get_offset() const;
      void set_offset(godot::Vector2 offset);


      void on_update(godot::StringName anim_name);

    
    private:
      template<typename return_type, typename... argument_types>
      std::map<uint64_t, return_type> _start_func_interface(return_type(Game::Animation::I_animation::*_interface_function)(argument_types...), argument_types... args){
        std::map<uint64_t, return_type> _res;
        for(auto _pair: _node_metadata){
          Game::Animation::I_animation* _interface = _pair.second._animation_interface;
          _res[_pair.first] = (_interface->*_interface_function)(args...);
        }

        return _res;
      }

      template<typename return_type, typename... argument_types>
      std::map<uint64_t, return_type> _start_funcconst_interface(return_type(Game::Animation::I_animation::*_interface_function)(argument_types...) const, argument_types... args) const{
        std::map<uint64_t, return_type> _res;
        for(auto _pair: _node_metadata){
          Game::Animation::I_animation* _interface = _pair.second._animation_interface;
          _res[_pair.first] = (_interface->*_interface_function)(args...);
        }

        return _res;
      }

      template<typename... argument_types>
      void _start_voidfunc_interface(void(Game::Animation::I_animation::*_interface_function)(argument_types...), argument_types... args){
        for(auto _pair: _node_metadata){
          Game::Animation::I_animation* _interface = _pair.second._animation_interface;
          (_interface->*_interface_function)(args...);
        }
      }
  };
}

#endif