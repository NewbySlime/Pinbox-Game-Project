#include "iostream"

using namespace std;


class I_test{
  public:
    virtual void test(){
      cout << "I_test" << endl;
    }
};

class test_class: I_test{
  public:
    void test() override{
      cout << "test" << endl;
    }
};

class not_test{
  public:
    
};


int main(){
  test_class t;
  not_test nt;
  I_test* it = (I_test*)&t;

  it->test();
}