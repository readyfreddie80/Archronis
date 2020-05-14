//
// Created by ready on 13.05.2020.
//

#ifndef ARCHRONIS_QUEUE_H
#define ARCHRONIS_QUEUE_H

#include <stddef.h>
#include <assert.h>
#include <iostream>

using namespace std;

#define POISON 666

template<class T>
class Queue {
private:
    T **qArr;                //queue array
    size_t size;             //current size of queue array
    size_t last;             //last element in the queue
    size_t maxChildNumb;     //max number of children in the heap

    static const size_t RESIZE_COEF = 2; //coef for resizing queue arrray

    void reheapUp(size_t root, size_t bottom);   // reheap upwards
    void reheapDown(size_t root, size_t bottom); // reheap downwards

public:
    explicit Queue(size_t initSize, size_t maxChN = 2);
    ~Queue();
    Queue(const Queue &) = delete;
    const Queue & operator=(const Queue &) = delete;

    void enqueue(T *);       //to push
    T *dequeue();            //to pop
    bool isEmpty() const;    //is empty?
    size_t getSize() const ; // get Size
};


#endif //ARCHRONIS_QUEUE_H
