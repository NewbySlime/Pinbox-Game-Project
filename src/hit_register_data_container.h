#ifndef HIT_REGISTER_DATA_CONTAINER
#define HIT_REGISTER_DATA_CONTAINER

#include "hit_register_interface.h"

#include "godot_cpp/variant/dictionary.hpp"

namespace Game{
  class hit_register_data_container{
    private:
      Game::hit_register_data* _data_head;

      static void _clear_data(Game::hit_register_data* _data_start);

    public:
      hit_register_data_container(hit_register_data& start_data);
      ~hit_register_data_container();

      Game::hit_register_data* get_head();
      static Game::hit_register_data* add_node(Game::hit_register_data* from, Game::hit_register_data* data);

      static std::shared_ptr<Game::hit_register_data_container> create_by_json(godot::Dictionary* json_data);
      static std::shared_ptr<Game::hit_register_data_container> copy_hitreg_container(const Game::hit_register_data_container& from);
  };
}

#endif