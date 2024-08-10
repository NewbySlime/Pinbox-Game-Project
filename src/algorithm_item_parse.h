#ifndef ALGORITHM_PARSE_HEADER
#define ALGORITHM_PARSE_HEADER

#include "godot_cpp/variant/packed_string_array.hpp"
#include "godot_cpp/variant/string.hpp"


namespace Game::Misc::Algorithm{
  /// @brief The result of parsing from full Item ID, from function parse_full_item_id.
  class parse_item_id_result{
    private:
      godot::String _item_type = "";
      godot::String _item_id = "";

      godot::String _warning_message = "";
      
      bool _warning_exist = false;

    public:
      /// @brief Function to add the warning when parsing the Item ID.
      /// @param warning_message The warning message to append
      void add_warning(const godot::String& warning_message);
      /// @brief Function to check if a warning has appended or not. 
      /// @return Has warning or not
      bool is_warning_exist();
      /// @brief Function to get the appended warnings.
      /// @return Appended warning string
      godot::String get_warning();

      /// @brief Function to set the main ID of the Item.
      /// @param item_id The Item ID
      void set_item_id(const godot::String& item_id);
      /// @brief Function to set the type ID of the Item.
      /// @param item_type The type ID
      void set_item_type(const godot::String& item_type);

      /// @brief Function to get main Item ID.
      /// @return The Item ID
      godot::String get_item_id();
      /// @brief Function to get type Item ID.
      /// @return The type ID
      godot::String get_item_type();

  };

  /// @brief Parsing function to process full ID to Item ID and Type ID.
  /// @param full_id The full Item ID
  /// @return Result of the parsing
  parse_item_id_result parse_full_item_id(godot::String full_id);
}

#endif