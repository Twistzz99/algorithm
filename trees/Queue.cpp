#ifndef QUEUE_CPP
#define QUEUE_CPP
#include "bt_node.cpp"
#include <malloc.h>
#include <string.h>
template <class T> class Queue {
private:
  T *data;
  int front;
  int rear;
  int maxsize;

public:
  Queue() {
    data = (T *)malloc(sizeof(T *) * 20);
    front = 0;
    rear = 0;
    maxsize = 20;
  }

  ~Queue() { free(this->data); }

  T get_front() {
    if (front != rear)
      return data[front];
    else
      return 0;
  }

  T get_rear() {
    if (front != rear)
      return data[rear];
    else
      return 0;
  }

  void enqueue(T target) {
    if ((rear + 1) % maxsize == front) {
      T *tmp = (T *)malloc(sizeof(T) * (maxsize + 10));
      memcpy(tmp, data, sizeof(T) * maxsize);
      maxsize = maxsize + 10;
    }
    data[rear] = target;
    rear = (rear + 1 % maxsize);
  }

  T dequeue() {
    if (rear == front)
      return 0;
    T tmp = data[front];
    front = (front + 1) % maxsize;
    return tmp;
  }

  int is_queue_empty() { return front == rear; }
};

#endif