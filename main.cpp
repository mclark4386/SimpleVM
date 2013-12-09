#include "VM.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  VM vm;

  cout<<"push 5"<<endl;
  vm.pushInt(5);

  cout<<"push 6"<<endl;
  vm.pushInt(6);

  cout<<"push 7"<<endl;
  vm.pushInt(7);

  cout<<"push pair(6,7)"<<endl;
  vm.pushPair();

  vm.printDebug();

  vm.pop();
  cout<<"after POP"<<endl;

  vm.printDebug();

  vm.gc();

  cout<<"after GC"<<endl;
  vm.printDebug();
  
  return 0;
}
