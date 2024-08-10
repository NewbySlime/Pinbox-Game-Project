#include "algorithm_bezier.h"
#include "logger.h"

using namespace godot;
using namespace Game::Misc::Algorithm;


Vector2 Game::Misc::Algorithm::bezier_curve_function(PackedVector2Array* array, double t, bool inverse, bool clamped){
  switch(array->size()){
    break; case 0:
      return Vector2();
    
    break; case 1:
      return array->operator[](0);
  }

  if(clamped){
    if(t < 0)
      return array->operator[](0);
    else if(t > 1)
      return array->operator[](array->size()-1);
  }

  if(inverse)
    t = 1 - t;

  PackedVector2Array *_newarray = new PackedVector2Array();
  for(int i = 1; i < array->size(); i++)
    _newarray->append(array->operator[](i-1).lerp(array->operator[](i), t));

  Vector2 _result = bezier_curve_function(_newarray, t);
  delete _newarray;

  return _result;
}