#ifndef STACK_H
#define STACK_H
#include "bt_node.cpp"
#include <stdlib.h>
#include <string.h>

template <class T> class Stack {
private:
  T *data;
  int top;
  int capacity;

public:
  T* get_data(int * length){
    *length=top+1;
    T* temp=(T*)malloc(sizeof(T)*(top+1));
    memcpy(temp,data,(top+1)*sizeof(T));
    return data;
  }

  Stack() {
    data = (T *)malloc(sizeof(T) * 10);
    capacity = 10;
    top = -1;
  }
  ~Stack() { free(data); }
  void push(T data) {
    if (top >= capacity - 1) {
      T *n = (T *)malloc(sizeof(T) * (capacity + 10));
      memcpy(n, this->data, (top + 1) * sizeof(T));
      free(this->data);
      this->data = n;
      capacity += 10;
    }
    top++;
    this->data[top] = data;
  }

  T pop() {
    if (top == -1)
      return 0;
    T result = data[top];
    top--;
    return result;
  }

  int is_stack_empty() { return top == -1; }

  T peek() {
    if (top <= 0)
      return 0;
    else
      return data[top];
  }

  int stack_deepth() { return top + 1; }
};

#endif