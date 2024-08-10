#ifndef CONSUMABLE_STATS_DATA_HEADER
#define CONSUMABLE_STATS_DATA_HEADER

#include "custom_memory.h"

#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"


namespace Game::Item{
  class consumable_stats_data: public GameUtils::Memory::custom_data{
    private:
      double _heal;

      bool _is_throwable;

      void _parse_consumable_stats_dict(godot::Dictionary* data);

      public:
        ~consumable_stats_data(){}

        static godot::String get_subdata_name();
        static void parse_dictionary_to(GameUtils::Memory::custom_memory* mem, godot::Dictionary* data);

        void copy_subdata_to(GameUtils::Memory::custom_memory* mem) const;

        double get_heal();
  };
}

#endif