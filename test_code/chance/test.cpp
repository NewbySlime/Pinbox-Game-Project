#include "chance_algorithm.h"
#include "string"

#include "time.h"

#include "iostream"


void _string_deinit(std::string obj){
  std::cout << "deinit " << obj << std::endl;
}


int main(){
  srand(time(NULL));

  random_chance_data<std::string>* _data = init_randomized_chance_data<std::string>();
  add_randomized_chance_data<std::string>(_data, 2, "test2");
  add_randomized_chance_data<std::string>(_data, 3, "test3");
  add_randomized_chance_data<std::string>(_data, 5, "test5");
  add_randomized_chance_data<std::string>(_data, 10, "test10");
  add_randomized_chance_data<std::string>(_data, 1, "test1");
  add_randomized_chance_data<std::string>(_data, 20, "test20");
  add_randomized_chance_data<std::string>(_data, 7, "test7");

  std::cout << "done initiating" << std::endl;
  std::cout << "score total: " << _data->score_total << std::endl;
  for(int i = 0; i < _data->score_total; i++){
    std::cout << "chance " << i << " " << get_randomized_chance(_data) << std::endl;
  }

  std::cout << "freeing" << std::endl;
  free_randomized_chance_data(_data, _string_deinit);

  std::cout << "done freeing" << std::endl;
}