#ifndef _VM_H_
#define _VM_H_

#include <cassert>

#include <iostream>
//#include <string>

#define STACK_MAX 256

enum class ObjectType{
  OBJ_INT,
    OBJ_PAIR
    };

typedef struct sObject{
  ObjectType type;
  unsigned char marked;
  sObject* next;//yay for light linked lists

  union{
    /*OBJ_INT*/
    int value;

    /*OBJ_PAIR*/
    struct{
      struct sObject* head;
      struct sObject* tail;
    };
  };
  
  void printObject(std::string prefix);
} Object;
void mark(Object* object);

class VM{
  Object* stack[STACK_MAX];
  int stackSize;
  Object* head;

  int currentAllocs;
public:
  int allocsUntilGC;

  VM():stackSize(0),head(nullptr),allocsUntilGC(10),currentAllocs(0){};
  ~VM()=default;

  void push(Object* value){
    assert(stackSize < STACK_MAX);
    stack[stackSize++] = value;
  };

  Object* pop(){
    assert(stackSize > 0);
    return stack[--stackSize];
  };

  void gc();

  Object* makeObject(ObjectType type){
    if(currentAllocs == allocsUntilGC) gc();

    Object* object = new Object();
    object->type = type;
    object->marked = 0;

    if(head ==nullptr) head = object;
    else{
      object->next = head;
      head = object;
    }

    currentAllocs++;
    

    return object;
  };

  void pushInt(int value){
    Object* object = makeObject(ObjectType::OBJ_INT);
    object->value = value;
    push(object);
  };

  //pop the top 2 objects makes a pair from them and then pushes that
  Object* pushPair(){
    Object* object = makeObject(ObjectType::OBJ_PAIR);
    object->tail = pop();
    object->head = pop();

    push(object);
    return object;
  };

  void markAll(){
    for(int i = 0; i<stackSize;i++){
      mark(stack[i]);
    }
  };

  void sweep(){
    Object**object = &head;
    while(*object){
      if(!(*object)->marked){
	Object* unreached = *object;
	*object = unreached->next;
	delete unreached;
	currentAllocs--;
      }else{
	(*object)->marked = 0;
	object = &(*object)->next;
      }
    }
  };

  void printDebug(){
    std::cout<<"VM STACK DEBUG INFO:"<<std::endl;

    std::cout<<"  Current Allocations:"<<currentAllocs<<std::endl;
    std::cout<<"  Number of Allocs at which GC will run:"<<allocsUntilGC<<std::endl;

    std::cout<<"  Stack:"<<std::endl;
    //print object tree
    for(int i = 0;i<stackSize;i++){
      stack[i]->printObject("   ");
    }

    std::cout<<"  Object list:"<<std::endl;
    //print object list
    Object* node = head;
    while(node){
      node->printObject("   ");
      node = node->next;
    }
  }
};



void VM::gc(){
  int temp = currentAllocs;
    markAll();
    sweep();
    allocsUntilGC = currentAllocs*2;
  };

void mark(Object* object){
  if(object->marked == 1) return;//to avoid cycles

  object->marked = 1;

  if(object->type == ObjectType::OBJ_PAIR){
    mark(object->head);
    mark(object->tail);
  }
}

void sObject::printObject(std::string prefix){
  if(type == ObjectType::OBJ_INT){
    std::cout<<prefix<<value<<std::endl;
  }else if(type == ObjectType::OBJ_PAIR){
    head->printObject(prefix+"  ");
    tail->printObject(prefix+"  ");
  }
}

#endif /* _VM_H_ */
