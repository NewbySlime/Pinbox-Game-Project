#ifndef GAME_COUNTERS_HEADER
#define GAME_COUNTERS_HEADER

#include "godot_macro.h"
#include "subautoload_compatibles.h"

#include "game_global_variable.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/object.hpp"

#define SIGNAL_GAME_COUNTER_SCORE_ON_INCREMENT "sgcsoi"
#define SIGNAL_GAME_COUNTER_SCORE_MULTIPLIER_CHANGED "sgcsmc"


namespace Game{
  class game_counter;
  class score_data{
    private:
      friend game_counter;
      int score;

    public:
      int get_score(){return score;}
  };

  struct score_increment_data{
    int base_score;
    uint32_t score_type;
  };

  enum score_additional_type{
    sat_bumper,
    sat_enemy_hit
  };


  class game_counter: public godot_sac::Node_sac{
  GDCLASS(game_counter, godot_sac::Node_sac)

    private:
      game_global_variable* _g_var;

      score_data _score;

      double bumper_base_limit;
      double bumper_base_multiplier;
      double bumper_multiplier_decrement;

      int bumper_max_multiplier;

      void _on_new_game();
      void _update_bumper_score(double score);

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;
      
      int _bumper_multiplier;
      double _bumper_score;

    public:
      game_counter();

      void _physics_process(double delta) override;

      void add_score(const score_increment_data& increment_data);
      score_data* get_score_data();
      int get_combo_multiplier_bumper();
      double get_combo_val_bumper();

      void reset_score();
  };
}

#endif