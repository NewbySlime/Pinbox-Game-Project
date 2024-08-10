#ifndef HITREG_DATA_HEADER
#define HITREG_DATA_HEADER

#include "custom_memory.h"
#include "hit_register_data_container.h"
#include "subdata_container.h"

#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"


namespace Game::Item{
  class weapon_stats_data;
  class hitreg_data: public GameUtils::ParsingHelper::subdata_container, public GameUtils::Memory::custom_data{
    private:
      friend Game::Item::weapon_stats_data;
      std::shared_ptr<Game::hit_register_data_container> _hitreg_graph = std::shared_ptr<Game::hit_register_data_container>(NULL);

    public:
      ~hitreg_data(){}

      static godot::String get_subdata_name();
      static void parse_dictionary_to(GameUtils::Memory::custom_memory* mem, godot::Dictionary* data);

      void copy_subdata_to(GameUtils::Memory::custom_memory* mem);

      std::shared_ptr<Game::hit_register_data_container> get_hitreg_graph() const;
  };
}

#endif