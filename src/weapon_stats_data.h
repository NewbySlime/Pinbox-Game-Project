#ifndef WEAPON_STATS_DATA_HEADER
#define WEAPON_STATS_DATA_HEADER

#include "custom_memory.h"
#include "defines.h"
#include "hitreg_data.h"
#include "subdata_container.h"

#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"


namespace Game::Item{
  namespace Factory{
    class weapon_factory;
  };

  class weapon_stats_data: public GameUtils::ParsingHelper::subdata_container, public GameUtils::Memory::custom_data{
    public:
      enum fire_mode{
        basic_config_filter = 0x00FF,
        additional_config_filter = 0xFF00,

        // basic configuration
        single_fire = 0x0000,
        burst_fire = 0x0001,
        automatic = 0x0002,

        // additional configuration
        // this will shoot uncontrollably until player's energy are drained
        full_exhaust = 0x0100,

        // priming doesn't instantly shoot it, not like normal configuration (no charge flag)
        // but continuing will shoot it, if the primed beyond charged delay 
        charge = 0x0200
      };

    private:
      double _damage;
      double _charge_delay;

      fire_mode _fire_mode;
      double _fire_rate;

      double _tap_rate;

      uint64_t _burst_count;

      void _parse_weapon_stats_dict(godot::Dictionary* data);
      
      fire_mode _parse_fire_mode(godot::String mode_flags);

    public:
      weapon_stats_data();
      ~weapon_stats_data(){}

      static godot::String get_subdata_name();
      static void parse_dictionary_to(GameUtils::Memory::custom_memory* mem, godot::Dictionary* data);

      void copy_subdata_to(GameUtils::Memory::custom_memory* mem) const;

      double get_damage() const;
      void set_damage(double damage);

      double get_charge_delay() const;
      void set_charge_delay(double delay);

      fire_mode get_fire_mode() const;
      void set_fire_mode(fire_mode mode);

      double get_fire_rate() const;
      void set_fire_rate(double rate);

      double get_tap_rate() const;
      void set_tap_rate(double rate);

      uint64_t get_burst_count() const;
      void set_burst_count(uint64_t count);

      template<typename subdata_class> const subdata_class* get_subdata() const{
        return __get_subdata<subdata_class>();
      }

      template<typename subdata_class> std::shared_ptr<const GameUtils::Memory::custom_memory> get_subdata_shared() const{
        return __get_subdata_shared<subdata_class>();
      }


    protected:
      friend Game::Item::Factory::weapon_factory;
      template<typename subdata_class> subdata_class* get_subdata_mutable() const{
        return __get_subdata<subdata_class>();
      }
  };


  class weapon_stats_data_primary: public weapon_stats_data{
    public:
      static godot::String get_subdata_name(){
        return VARNAME_WEAPON_STATS_TYPE_PRIMARY;
      }
  };

  class weapon_stats_data_secondary: public weapon_stats_data{
    public:
      static godot::String get_subdata_name(){
        return VARNAME_WEAPON_STATS_TYPE_SECONDARY;
      }
  };
}

#endif