//
// Created by ready on 14.05.2020.
//

#ifndef ARCHRONIS_VECTOR_H
#define ARCHRONIS_VECTOR_H

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <algorithm> //max

#define VECTOR_OK() if (OK() != 0) {dump(); assert(!"ok");}

template <class T>
struct Vector {
protected:
    T *buf_;
    size_t size_;
    size_t maxSize_;

    int errNo_;
    int OK();
    void resize();
    void dump() const {}

    static const size_t POISON = 666;
    static const size_t RESIZE_COEF = 2; //coef for resizing arrray

public:
    explicit Vector(size_t = 0);
    Vector(size_t size, const T & initialVal);
    Vector(std::initializer_list<T> l);
    Vector(const Vector &that);
    ~Vector();
    Vector & operator =(const Vector& that);

    T & operator [](int index);
    const T & operator [](int index) const;

    template <class M>
    friend std::ostream& operator<<(std::ostream& os, const Vector<M>& v);

    size_t getSize() const { return size_; }
    size_t getMaxSize() const { return maxSize_; }

    void pushBack(const T& d);
    void pushFront(const T& d);
};


enum Err {
    E_OK,                   // Vector is valid;
    E_NULL_PTR_BUF,         // Vector is pointed by NULL;
    E_INVALID_SIZE,         // size is invalid;
    E_INVALID_MAXSIZE,      // maxSize is invalid;
};


template <class T>
Vector<T>::Vector(size_t size)
        :
        buf_( new T[std::max<size_t>(size, 1)] ),
        size_(size),
        maxSize_(std::max<size_t>(size, 1)),
        errNo_(E_OK) {


#ifdef DEBUG
    VECTOR_OK();
#else
    assert(buf_);
    assert(maxSize_ > 0);
#endif
}


template <class T>
Vector<T>:: Vector(std::initializer_list<T> l)
    : Vector(static_cast<size_t>(l.size())) {
        std::copy(l.begin(), l.end(), buf_);
}


template <class T>
Vector<T>::Vector(size_t size, const T & initialVal) : Vector(size) {
    assert(size > 0);
    for(size_t i = 0; i < size; ++i) {
        buf_[i] = initialVal;
    }
#ifdef DEBUG
    VECTOR_OK();
#endif
}


template <class T>
Vector<T>::~Vector() {

    delete [] buf_;
    buf_ = nullptr;
    size_ = POISON;
    maxSize_ = POISON;
}


template <class T>
T & Vector<T>::operator [](int index) {
#ifdef DEBUG
    VECTOR_OK();
#else
    assert(index >= 0);
    assert(index < size_);
#endif

    return buf_[index];
}


template <class T>
const T & Vector<T>::operator [](int index) const {
    assert(index >= 0);
    assert(index < size_);

    return buf_[index];
}


template <class T>
Vector<T>::Vector(const Vector<T> &that)
        :
        buf_( new T[that.getMaxSize()] ),
        size_(that.getSize()),
        maxSize_(that.getMaxSize()),
        errNo_(E_OK) {

#ifdef DEBUG
    VECTOR_OK();
#else
    assert(buf_ != nullptr);
    assert(maxSize_ > 0);
#endif
    for (size_t i = 0; i < size_; ++i) {
        buf_[i] = that[i];
    }
}


template <class T>
Vector<T> & Vector<T>::operator =(const Vector& that) {
    if (this == &that) return *this;

    if (that.getSize() <= maxSize_) {

        for (size_t i = 0; i < that.getSize(); ++i) {
            buf_[i] = that[i];
        }
        size_ = that.getSize();
        return *this;

    }

    T* newBuf = new T[that.size_];

    for (size_t i = 0; i < that.getSize(); ++i) {
        newBuf[i] = that[i];
    }

    delete [] buf_;
    buf_ = newBuf;
    size_ = that.getSize();
    maxSize_ = that.getSize();

    return *this;
 }


 template <class T>
 void Vector<T>::resize() {
    if(size_ >= maxSize_) {
        size_t newSize = RESIZE_COEF * maxSize_;
        auto newBuf = new T [newSize];
        for(size_t i = 0; i < maxSize_; ++i)
            newBuf[i] = buf_[i];
        delete[] buf_;
        buf_ = newBuf;
        maxSize_ = newSize;
    }
}


template <class T>
void Vector<T>::pushBack(const T &d) {

    resize();

    buf_[size_] = d;

    ++size_;
}


template <class T>
void Vector<T>::pushFront(const T &d) {

    resize();

    for (size_t i = size_; i != 0 ; i--) {
        buf_[i] = buf_[i - 1];
    }

    buf_[0] = d;

    size_++;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& v) {
    for(size_t i = 0; i < v.getSize(); ++i) {
        os << v[i] << std::endl;
    }
    return os;
}


template <class T>
int Vector<T>::OK () {
    if (buf_ == nullptr) {
        errNo_ = E_NULL_PTR_BUF;
        return E_NULL_PTR_BUF;
    }

    if (maxSize_  == 0) {
        errNo_ = E_INVALID_MAXSIZE;
        return E_INVALID_MAXSIZE;
    }

    if (size_ > maxSize_) {
        errNo_ = E_INVALID_SIZE;
        return E_INVALID_SIZE;
    }

    return E_OK;
}

#endif //ARCHRONIS_VECTOR_H
