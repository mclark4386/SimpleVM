#include <iostream>
#include "VM.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  VM vm;

  int choice = -1;
  while(choice != 0){
    cout<<"Welcome to Simple VM! Pick your poison:"<<endl;
    cout<<"1) pop top of stack"<<endl;
    cout<<"2) push int on stack"<<endl;
    cout<<"3) push pair"<<endl;
    cout<<"4) force GC"<<endl;
    cout<<"5) print debug info"<<endl;
    cout<<"0) quit"<<endl;
    cin>>choice;

    int tempInt = 0;
    Object* tempObj = nullptr;

    switch(choice){
    case 0:
      break;
    case 1:
      tempObj = vm.pop();
      cout<<"popped object:";
      tempObj->printObject(" ");
      break;
    case 2:
      cout<<"Please enter int to push:"<<endl;
      cin>>tempInt;
      vm.pushInt(tempInt);
      break;
    case 3:
      vm.pushPair();
      break;
    case 4:
      vm.gc();
      break;
    case 5:
      vm.printDebug();
      break;
    default:
      cout<<"Please enter valid option!"<<endl;
    }
  }

  return 0;
}
