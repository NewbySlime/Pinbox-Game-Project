#ifndef MODIFIER_DATABASE_HEADER
#define MODIFIER_DATABASE_HEADER

#include "algorithm_random.h"
#include "custom_memory.h"
#include "godot_macro.h"
#include "I_modifier.h"
#include "I_modifier_database_helper.h"

#include "godot_cpp/classes/node.hpp"

#include "map"
#include "memory"
#include "string"


namespace Game::Item{
  namespace Factory{
    class modifier_factory;
  }

  class modifier_database: public godot::Node{
  GDCLASS(modifier_database, godot::Node)
    GD_PROPERTY_VAR(godot::String, modifier_folder)

    public:
      struct modifier_metadata{
        private:
          friend modifier_database;
          double chance_score;
          std::shared_ptr<GameUtils::Memory::custom_memory> metadata_randomizer_data;

        public:
          double get_chance_score() const{ return chance_score; }
      };

    private:
      std::shared_ptr<GameUtils::Memory::custom_memory> _static_data;

      Game::Misc::Algorithm::random_chance_data<std::string>* _chance_modifier_id;
      std::map<godot::String, modifier_metadata> _modifier_metadata_map;

      void _load_modifier_data(const godot::String& folder_dir);

    
    protected:
      static void _bind_methods();
    
    public:
      modifier_database();

      void _notification(int code);

      void _ready() override;


    private:
      friend Game::Item::Factory::modifier_factory;
      std::string get_random_modifier_id(godot::RandomNumberGenerator* rng);
      std::shared_ptr<Game::Modifiers::I_modifier> create_specific_modifier(std::string ID);

      // when there's an error, it throws a gdstr_exception
      void randomize_modifier(godot::RandomNumberGenerator* rng, std::shared_ptr<Game::Modifiers::I_modifier> modifier);

      const modifier_metadata* get_metadata(std::string ID);

  };

  typedef std::shared_ptr<Game::Modifiers::I_modifier>(*create_modifier_callback)();
  typedef std::shared_ptr<GameUtils::Memory::custom_memory>(*parse_modifier_data_callback)(godot::Dictionary* json_data);
}

#endif