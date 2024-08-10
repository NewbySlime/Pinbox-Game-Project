#ifndef VITAL_INFO_HEADER
#define VITAL_INFO_HEADER

#include "custom_memory.h"

#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"

#include "string"


namespace Game::Entity{
  class vital_info: public GameUtils::Memory::custom_data{
    private:
      float _max_health_point;

      float _max_energy_point;
      float _ep_restore_delay;
      float _ep_restore_rate;


      void _parse_data(const godot::Dictionary& _data_part);


    public:
      ~vital_info(){}

      static std::string get_subdata_name_stdstr();
      static godot::String get_subdata_name();
      static void parse_dictionary_to(GameUtils::Memory::custom_memory* mem, godot::Dictionary* data);

      void copy_subdata_to(GameUtils::Memory::custom_memory* mem);


      float get_max_hp();
      
      float get_max_ep();
      float get_ep_restore_delay();
      float get_ep_restore_rate();
  };
}

#endif