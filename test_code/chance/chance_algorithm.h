#ifndef CHANCE_ALGORITHM_HEADER
#define CHANCE_ALGORITHM_HEADER

#include "stdint.h"

#include "stdlib.h"
#include "vector"

#include "iostream"


template<typename T> class random_chance_data{
  public:
    double score_total;
    std::vector<std::pair<double, T>> algorithm_data;
};

template<typename T> random_chance_data<T>* init_randomized_chance_data(){
  random_chance_data<T>* _data = new random_chance_data<T>();
  _data->score_total = 0;

  return _data;
}

template<typename T> void free_randomized_chance_data(random_chance_data<T>* data, void(*deinit_callback)(T obj) = NULL){
  if(data){
    if(deinit_callback){
      for(int i = 0; i < data->algorithm_data.size(); i++)
        deinit_callback(data->algorithm_data[i].second);
    }

    delete data;
  }
}

template<typename T> void add_randomized_chance_data(random_chance_data<T>* data, double score, T obj){
  data->score_total += score;
  data->algorithm_data.insert(data->algorithm_data.end(), {data->score_total, obj});
}

template<typename T> T get_randomized_chance(const random_chance_data<T>* chance_data){
  double _r_val = (double)rand()/RAND_MAX * chance_data->score_total;

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


#endif