#ifndef ALGORITHM_RANDOM_HEADER
#define ALGORITHM_RANDOM_HEADER

#include "godot_cpp/classes/random_number_generator.hpp"

#include "stdint.h"
#include "cstdlib"
#include "utility"
#include "vector"


namespace Game::Misc::Algorithm{
  /// @brief Chance data used for randomizing chances with other chances using get_randomized_chance function.
  /// @tparam T The value representing the result of the chance
  template<typename T> class random_chance_data{
    public:
      /// @brief The total score to compare with sub-chance scores
      double score_total;
      /// @brief The list of chances, with the score and the chance result
      std::vector<std::pair<double, T>> algorithm_data;
  };

  /// @brief Initialize random_chance_data function
  /// @tparam T The value type to be used within random_chance_data
  /// @return Returns the Initialized data
  template<typename T> random_chance_data<T>* init_randomized_chance_data(){
    random_chance_data<T>* _data = new random_chance_data<T>();
    _data->score_total = 0;

    return _data;
  }

  /// @brief Function to remove/free the random_chance_data class.
  /// @tparam T The value type used within random_chance_data
  /// @param data The random_chance_data to free
  /// @param deinit_callback Callback getting every value object being removed, use this when the value is a dynamic memory
  template<typename T> void free_randomized_chance_data(random_chance_data<T>* data, void(*deinit_callback)(T obj) = NULL){
    if(data){
      if(deinit_callback){
        for(int i = 0; i < data->algorithm_data.size(); i++)
          deinit_callback(data->algorithm_data[i].second);
      }

      delete data;
    }
  }

  /// @brief Function to add another chance
  /// @tparam T The value type to be used within random_chance_data
  /// @param data The random_chance_data to add the chance
  /// @param score The score of the chance
  /// @param obj The value type of the chance
  template<typename T> void add_randomized_chance_data(random_chance_data<T>* data, double score, T obj){
    data->score_total += score;
    data->algorithm_data.insert(data->algorithm_data.end(), {data->score_total, obj});
  }

  /// @brief Function to get random chance within the data
  /// @tparam T The value type to be used within random_chance_data
  /// @param chance_data The random_chance_data to get randomized chance from
  /// @param _rng The RNG class for randomizing function
  /// @return The value of the chance
  template<typename T> T get_randomized_chance(const random_chance_data<T>* chance_data, godot::RandomNumberGenerator* _rng){
    double _r_val = _rng->randf_range(0, chance_data->score_total);

    int32_t _high = chance_data->algorithm_data.size()-1;
    int32_t _low = 0;
    int32_t _middle = 0;

    while(_low <= _high){
      _middle = (_high + _low)/2;
      if(chance_data->algorithm_data[_middle].first > _r_val){
        int _check_index = _middle-1;
        if(_check_index >= 0 && chance_data->algorithm_data[_check_index].first < _r_val)
          break;
        
        _high = _middle - 1;
      }
      else
        _low = _middle + 1;
    }

    return chance_data->algorithm_data[_middle].second;
  }
}

#endif