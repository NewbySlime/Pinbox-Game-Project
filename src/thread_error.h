#ifndef THREAD_ERROR_HEADER
#define THREAD_ERROR_HEADER

#include "thread_context_data.h"


namespace Game::Error{
  template<typename error_object> class thread_error: private GameUtils::Threading::thread_context_data<error_object>{
    public:
      thread_error(error_object default_err): thread_context_data(default_err){}

      void add_last_error(error_object err){
        add_last_data(err);
      }
    
      error_object get_last_error(){
        return get_last_data();
      }
  };
}

#endif