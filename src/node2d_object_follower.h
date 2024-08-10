#ifndef NODE2D_OBJECT_FOLLOWER_HEADER
#define NODE2D_OBJECT_FOLLOWER_HEADER

#include "godot_cpp/classes/node2d.hpp"

#include "interface_helper.h"
#include "subautoload_container.h"


#define SIGNAL_NODE2D_OBJECT_FOLLOWER_POSITION_UPDATE "sn2dofpu"
#define SIGNAL_NODE2D_OBJECT_FOLLOWER_DELETED "sn2dofd"


namespace Game{
  class node2d_object_follower: public godot::Node2D,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(node2d_object_follower, godot::Node2D)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(node2d_object_follower)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(node2d_object_follower)
  ATTR_INTERFACE_HELPER_INHERIT(node2d_object_follower)

    GD_PROPERTY_VAR_WITHOUT_DEF_FUNC(bool, disabled)
    GD_PROPERTY_VAR_WITHOUT_DEF_FUNC(bool, use_physics_process)
    
    public:
      struct signal_position_update{
        private:
          friend node2d_object_follower;
          godot::Vector2 _position;

        public:
          godot::Vector2 get_position(){
            return _position;
          }
      };


    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;

    public:
      void _notification(int code);
    
      void _process(double delta) override;
      void _physics_process(double delta) override;

  };
}

#endif