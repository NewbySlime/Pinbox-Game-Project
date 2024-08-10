#ifndef ALGORITHM_DIRECTORY_HEADER
#define ALGORITHM_DIRECTORY_HEADER

#include "defines.h"
#include "godot_error.h"
#include "godot_macro.h"

#include "godot_cpp/classes/dir_access.hpp"
#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/variant/string.hpp"


namespace Game::Misc::Algorithm{
  /// @brief This is to get the extension of the file.
  /// @param path Path of the file, or the filename with the extension on it
  /// @param directory_splitter The directory seperator character
  /// @return returns extension of the file with "." prefix
  godot::String get_filename_extension(godot::String path, godot::String directory_splitter = "/");



  /// @brief  When there's an error, it will throw Game::Error::gdstr_exception with both message and the appropriate error code
  /// List of possible error code:
  ///  - ERR_SKIP (just a warning)
  ///  - ERR_FILE_BAD_PATH
  ///
  /// callback can throw an error, using gdstr_exception
  /// when an error is thrown, the function will store its message
  /// when the function is exiting, if there's an error, it will pass it to caller by throw it again
  /// @tparam parameter_type The data/class type the callback would use
  /// @param folder_path The folder to check
  /// @param callback The callback when the function finds the right files
  /// @param allowed_extension List of filters of allowed extension with a prefix "."
  /// @param param The parameter data the callback would use for feedback from the caller
  template<typename parameter_type> void recurse_folders(
    godot::String folder_path,
    void(*callback)(parameter_type*, godot::Ref<godot::FileAccess>),
    const godot::PackedStringArray& allowed_extension,
    parameter_type* param = NULL
  ){
    godot::String _tmp_error_msg = "";
    bool _error_occurred = false;

    godot::Array _paramarr;

    godot::Ref<godot::DirAccess> _folder_access = godot::DirAccess::open(folder_path);
    godot::Error _err = DirAccess::get_open_error();
    if(_err != godot::Error::OK){
      _paramarr.clear();{
        _paramarr.append(folder_path);
      }

      throw Game::Error::gdstr_exception(String("Cannot open folder. (Dir: '{0}')").format(_paramarr), godot::ERR_FILE_BAD_PATH);
    }

    // checks all folders
    godot::PackedStringArray _files_list = _folder_access->get_files();
    for(int i = 0; i < _files_list.size(); i++){
      godot::String _file_name = _files_list[i];
      godot::String _filename_ext = get_filename_extension(_file_name);
      if(allowed_extension.size() > 0 && !allowed_extension.find(_filename_ext))
        continue;

      _paramarr.clear();{
        _paramarr.append(folder_path);
        _paramarr.append(_file_name);
      }

      godot::String _file_dir = JOIN_PATH_STRING_HELPER.format(_paramarr);

      godot::Ref<godot::FileAccess> _file = FileAccess::open(_file_dir, godot::FileAccess::ModeFlags::READ);

      // store error message
      try{
        callback(param, _file);
      }
      catch(Game::Error::gdstr_exception e){
        _error_occurred = true;
        _paramarr.clear();{
          _paramarr.append(folder_path);
          _paramarr.append(e.what());
        }

        _tmp_error_msg += String("Error when parsing data in '{0}'.\nError Message: {1}\n").format(_paramarr);
      }
    }

    // recurse the folders
    godot::PackedStringArray _folder_list = _folder_access->get_directories();
    for(int i = 0; i < _folder_list.size(); i++){
      godot::String _folder_name = _folder_list[i];
      _paramarr.clear();{
        _paramarr.append(folder_path);
        _paramarr.append(_folder_name);
      }

      String _folder_dir = JOIN_PATH_STRING_HELPER.format(_paramarr);

      // store the error message
      try{
        //recurse_folder(_folder_dir, callback, allowed_extension, param);
      }
      catch(Game::Error::gdstr_exception e){
        _error_occurred = true;
        _tmp_error_msg += e.what() + "\n";
      }
    }

    if(_error_occurred)
      throw Game::Error::gdstr_exception(_tmp_error_msg, godot::ERR_SKIP);
  }
}

#endif