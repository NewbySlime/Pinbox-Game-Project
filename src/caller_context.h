#ifndef CALLER_CONTEXT_HEADER
#define CALLER_CONTEXT_HEADER


#include "I_caller_context.h"
#include "thread_context_data.h"


namespace GameUtils::Function{
  class caller_context: public GameUtils::Function::I_caller_context, private GameUtils::Threading::thread_context_data<GameUtils::Function::caller_data*>{
    private:
      GameUtils::Function::caller_data* _create_data(const GameUtils::Function::caller_data& from_data);
      void _delete_data(GameUtils::Function::caller_data* _caller_data);

    protected:
      void _on_this_deleted_data_iteration(GameUtils::Function::caller_data* _caller_data) override;

    public:
      caller_context(): thread_context_data(NULL){}

      void add_current_context(const GameUtils::Function::caller_data& caller_context) override;
      void delete_current_context() override;

      const caller_data* get_current_context() override;
  };
}

#endif