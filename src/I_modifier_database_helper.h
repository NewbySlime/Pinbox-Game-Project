#ifndef I_MODIFIER_DATABASE_HELPER_HEADER
#define I_MODIFIER_DATABASE_HELPER_HEADER

#include "subautoload_container.h"

#include "custom_memory.h"

#include "godot_cpp/classes/random_number_generator.hpp"
#include "godot_cpp/variant/dictionary.hpp"

#include "memory"
#include "string"


#define MODDB_HELPER_GET_STATIC_JSON_PARSER(class_name)\
  class_name::parse_json_data_static

#define MODDB_HELPER_ADD_STATIC_JSON_PARSER_DECLARE()\
  static std::shared_ptr<GameUtils::Memory::custom_memory> parse_json_data_static(godot::Dictionary* json_data);

#define MODDB_HELPER_ADD_STATIC_JSON_PARSER_DEFINE(class_name)\
  std::shared_ptr<GameUtils::Memory::custom_memory> class_name::parse_json_data_static(godot::Dictionary* json_data)


#define MODDB_HELPER_ADD_STATIC_ID_GETTER(associated_id) \
  static std::string get_associated_id(){ return associated_id; } \
  std::string get_modifier_class() override{ return associated_id; }

#define MODDB_HELPER_GET_ASSOCIATED_ID(class_name)\
  class_name::get_associated_id()



namespace Game::Item{
  class I_modifier_database_helper{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_modifier_database_helper)

    public:
      virtual void randomize_modifier(godot::RandomNumberGenerator* rng, std::shared_ptr<GameUtils::Memory::custom_memory> modifier_data){}

      virtual std::string get_modifier_class(){ return ""; }
  };
}

#endif