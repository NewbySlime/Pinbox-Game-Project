#include "custom_memory.h"

#include "memory"
#include "iostream"


using namespace GameUtils::Memory;
using namespace std;


class test_data: public custom_data{
  public:
    test_data(){
      cout << "constructing test data" << endl;
    }

    ~test_data(){
      cout << "deconstructing test data" << endl;
    }
};

class test_data2: public test_data{
  private:
    int test = 123;

  public:
    test_data2(){
      cout << "constructing test data 2" << endl;
    }

    ~test_data2(){
      cout << "test " << test << endl;
      cout << "deconstructing test data 2" << endl;
    }
};

class test_class: public custom_data{
  private:
    int test_int = 12345;

  public:
    test_class(){
      cout << "this is test class" << endl;
    }

    ~test_class(){
      cout << "test class will be deconstructing" << endl;
    }

    int get_test(){
      return test_int;
    }
};


int main(){
  std::shared_ptr<custom_memory> _cm = custom_memory::create_shared<test_data>();
  cout << "custom size " << _cm->get_data_size() << endl;
  _cm->change_data<test_class>();
  cout << "custom size " << _cm->get_data_size() << endl;
  test_class* _test = _cm->get_data<test_class>();
  cout << "test int " << _test->get_test() << endl;
  _cm->change_data<test_data2>();
  cout << "custom size " << _cm->get_data_size() << endl;
}