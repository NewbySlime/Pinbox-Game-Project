#ifndef PERSISTANCE_CONTEXT_HEADER
#define PERSISTANCE_CONTEXT_HEADER

#include "custom_memory.h"
#include "I_persistance_data.h"
#include "subautoload_container.h"
#include "subautoload_container.h"
#include "subdata_container.h"

#include "godot_cpp/classes/node.hpp"

#include "time.h"


#define SIGNAL_PERSISTANCE_CONTEXT_ON_SAVE "spcos"
#define SIGNAL_PERSISTANCE_CONTEXT_ON_LOAD "spcol"

#define SIGNAL_PERSISTANCE_CONTEXT_ON_SAVE_INITIATED "spcosi"
#define SIGNAL_PERSISTANCE_CONTEXT_ON_LOAD_INITIATED "spcoli"

#define SIGNAL_PERSISTANCE_CONTEXT_ON_SAVE_DONE "spcosd"
#define SIGNAL_PERSISTANCE_CONTEXT_ON_LOAD_DONE "spcold"

#define FALSE_PERSISTANCE_CTX_ID 0


namespace Game{
  class game_handler;
}

namespace Game::Persistance{
  class persistance_context: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(persistance_context, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(persistance_context)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(persistance_context)
  ATTR_INTERFACE_HELPER_INHERIT(persistance_context)
  
    GD_PROPERTY_VAR(godot::String, DEBUG_game_save)

    public:
      typedef uint64_t persistance_id;

      class persistance_metadata: public GameUtils::ParsingHelper::subdata_container{
        private:
          godot::String _save_name;
          time_t _unix_time_last_save;

        public:
          godot::String get_save_name();
          godot::String get_formatted_last_save_time();
      };


    private:
      std::shared_ptr<GameUtils::Memory::custom_memory> _static_data;

      std::map<persistance_id, persistance_metadata> _metadata;
      std::map<persistance_id, std::weak_ptr<Game::Persistance::I_persistance_data>> _opened_data;

      persistance_id _current_id;
      std::shared_ptr<Game::Persistance::I_persistance_data> _current_persistance;


    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;


    public:
      persistance_context();

      void _notification(int code);

      std::set<persistance_id> get_available_persistance();
      const persistance_metadata* get_metadata(persistance_id id);
      
      // if NULL then it's not loaded
      Game::Persistance::I_persistance_data* get_current_persistance_data();
      std::shared_ptr<Game::Persistance::I_persistance_data> get_persistance_data(persistance_id id);

    protected:
      friend Game::game_handler;

      persistance_id create_new_persistance();

      // NOTE: this will also load the persistance
      bool change_persistance(persistance_id id);
      bool change_persistance_name(persistance_id id, godot::String new_name);
      
      void trigger_save();
  };


  class persistance_context_on_save_param{
    
  };

  class persistance_context_on_load_param{

  };
}

#endif