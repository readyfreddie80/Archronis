//
// Created by ready on 14.05.2020.
//

#ifndef ARCHRONIS_VECTOR_H
#define ARCHRONIS_VECTOR_H

#include <stdio.h>
#include <cstdlib>
#include <assert.h>
#include <algorithm>

#define POISON 666

#define VECTOR_OK() if (OK() != 0) {dump(); assert(!"ok");}


enum Err {
    E_OK,                   // Vector is valid;
    E_NULL_PTR_BUF,         // Vector is pointed by NULL;
    E_INVALID_SIZE,         // size is invalid;
    E_INVALID_MAXSIZE,      // maxSize is invalid;
};

const char* errMessage[] = {
        "vector is OK",
        "buffer of vector is nullptr",
        "maxSize is 0",
        "size is more than maxSize",
};

using namespace std;

template <class T>
struct Vector {
private:
    T *buf_;
    size_t size_;
    size_t maxSize_;

    int errNo_;
    int OK();
    void dump() const;

    static const size_t RESIZE_COEF = 2; //coef for resizing queue arrray

public:
    explicit Vector(size_t = 0);
    Vector(size_t size, const T & initialVal);
    ~Vector();
    T & operator [](int index);
    const T & operator [](int index) const;
    Vector(const Vector &that);

    Vector & operator =(const Vector& that) = delete;

    size_t getSize() const { return size_; }
    size_t getMaxSize() const { return maxSize_; }
    void pushBack(const T& d);
};




#endif //ARCHRONIS_VECTOR_H
