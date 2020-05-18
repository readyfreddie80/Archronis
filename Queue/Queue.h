//
// Created by ready on 13.05.2020.
//

#ifndef ARCHRONIS_QUEUE_H
#define ARCHRONIS_QUEUE_H

#include <assert.h>
#include <iostream>


template<class T>
class Queue {
private:
    T **qArr;                //queue array
    size_t size;             //current size of queue array
    size_t last;             //last element in the queue
    size_t maxChildNumb;     //max number of children in the heap

    static const size_t RESIZE_COEF = 2; //coef for resizing queue arrray
    static const size_t POISON = 666;

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

template<class T>
Queue<T>::Queue(size_t initSize, size_t maxChN)
        :qArr(new T *[initSize]),
         size(initSize),
         last(0),
         maxChildNumb(maxChN) {
#ifdef DEBUG
    assert(qArr);
    assert(initSize > 0);
    assert(maxChN > 1);
#endif
}

template<class T>
Queue<T>::~Queue() {
    delete[] qArr;
    qArr = nullptr;
    size = POISON;
    last = POISON;
    maxChildNumb = POISON;
}

template<class T>
void Queue<T>::enqueue(T *newData) {
    if(last >= size) {
        size_t newSize = RESIZE_COEF * size;
        T **newQArr = new T *[newSize];
        for(size_t i = 0;i < size; ++i)
            newQArr[i] = qArr[i];
        delete[] qArr;
        qArr = newQArr;
        size = newSize;
    }

    qArr[last] = newData;
    reheapUp(0, last++);
}


template<class T>
T *Queue<T>::dequeue() {
    if(isEmpty()) {
        std::cerr << "Dequeue error: Queue is empty" << std::endl;
        exit(1);
    }

    T *result = qArr[0];
    --last;
    qArr[0] = qArr[last];

    if(last != 0) {
        reheapDown(0, last - 1);
    }
    return result;
}

template<class T>
bool Queue<T>::isEmpty() const {
    return last == 0;
}

template<class T>
size_t Queue<T>::getSize() const {
    return last;
}


template<class T>
void Queue<T>::reheapUp(size_t root, size_t bottom) {
    if(bottom > root) {
        //parent node of the bottom element
        size_t parent = (bottom - 1) / maxChildNumb;

        if(*qArr[parent] > *qArr[bottom]) {
            std::swap(qArr[parent], qArr[bottom]);
            reheapUp(root, parent);
        }
    }
}


template<class T>
void Queue<T>::reheapDown(size_t root, size_t bottom) {
    //the index of the first child of the root
    size_t firstChild = root * maxChildNumb + 1;

    if(firstChild <= bottom) {
        size_t minChild = firstChild;

        for(size_t i = 1; i < maxChildNumb; ++i) {
            size_t otherChild = firstChild + i;
            if(otherChild <= bottom &&
               *qArr[minChild] > *qArr[otherChild]) {
                minChild=otherChild;
            }
        }

        if(*qArr[root] > *qArr[minChild]) {
            std::swap(qArr[root], qArr[minChild]);
            reheapDown(minChild, bottom);
        }
    }
}

#endif //ARCHRONIS_QUEUE_H
