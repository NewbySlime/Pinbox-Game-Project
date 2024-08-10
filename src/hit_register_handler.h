#ifndef HIT_REGISTER_HANDLER_HEADER
#define HIT_REGISTER_HANDLER_HEADER


#include "custom_memory.h"
#include "hit_register_data_container.h"
#include "hit_register_interface.h"
#include "interface_helper.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/area2d.hpp"
#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/node_path.hpp"

#include "memory"
#include "map"
#include "set"
#include "vector"


namespace Game{
  class hit_register;
  class hit_register_handler: public godot::Node2D,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(hit_register_handler, godot::Node2D)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(hit_register_handler)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(hit_register_handler)
  ATTR_INTERFACE_HELPER_INHERIT(hit_register_handler)

    GD_PROPERTY_VAR(godot::Dictionary, hit_reg_node_packed_scene)

    private:
      Node2D* _root_node;

      void _load_hr_nodes();
      godot::PackedScene* _get_hr_node(Game::hit_register_type_enum hrte_type);

      std::map<uint32_t, godot::PackedScene*> _hr_packed_scenes;

      void _cast_hit_reg(
        std::shared_ptr<Game::hit_register_data_container>& reg_data, 
        Game::hit_register_data* hitreg_data,
        std::shared_ptr<Game::I_damager>& damager,
        hit_register_start_data* start_data
      );

    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;

    public:
      hit_register_handler();

      void _notification(int code);

      void _physics_process(double delta) override;
      
      void cast_hit_reg(std::shared_ptr<Game::hit_register_data_container>& reg_data, std::shared_ptr<Game::I_damager>& damager, hit_register_start_data* start_data);
      void next_hitreg_part(Game::hit_register* hitreg, hit_register_start_data* start_data);
      void stop_hitreg(Game::hit_register* hitreg);

      void set_root(Node2D* node);
  };


  class hit_register: I_hit_register{
    private:
      friend Game::hit_register_handler;
      godot::Node* _this_node;

    protected:
      Game::hitreg_metadata _metadata;
      hit_register_start_data _start_data;

      void on_hitreg_finished();
      void on_hitreg_interrupted();

    public:
      virtual ~hit_register(){}
  };
}

#endif