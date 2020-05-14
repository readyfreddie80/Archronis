//
// Created by ready on 13.05.2020.
//

#include "Queue.h"

template<class T>
Queue<T>::Queue(size_t initSize, size_t maxChN)
        :qArr(new T *[initSize]),
         size(initSize),
         last(0),
         maxChildNumb(maxChN) {
    assert(qArr);
    assert(initSize > 0);
    assert(maxChN > 1);
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
        cerr << "Dequeue error: Queue is empty" << endl;
        exit(1);
    }

    T *result = qArr[0];
    --last;
    qArr[0] = qArr[last];

    reheapdown(0, last - 1);

    return result;
}

template<class T>
bool Queue<T>::isEmpty() const {
    return last == 0;
}

template<class T>
size_t Queue<T>::getSize() const {
    return size;
}


template<class T>
void Queue<T>::reheapUp(size_t root, size_t bottom) {
    if(bottom > root) {
        //parent node of the bottom element
        size_t parent = (bottom - 1) / maxChildNumb;

        if(*qArr[parent] > *qArr[bottom]) {
            swap(qArr[parent], qArr[bottom]);
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
            swap(qArr[root], qArr[minChild]);
            reheapDown(minChild, bottom);
        }
    }
}

