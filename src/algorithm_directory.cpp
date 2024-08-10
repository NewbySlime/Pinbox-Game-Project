#include "algorithm_directory.h"


using namespace Game::Misc::Algorithm;
using namespace godot;


String Game::Misc::Algorithm::get_filename_extension(String path, String directory_splitter){
  int64_t _index = path.rfind(directory_splitter);
  if(_index > 0)
    path = path.erase(0, _index+1);

  String _res = "";
  _index = path.find(".");
  if(_index > 0)
    _res = path.erase(0, _index);

  return _res;
}