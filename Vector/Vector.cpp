//
// Created by ready on 14.05.2020.
//

#include "Vector.h"


template <class T>
Vector<T>::Vector(size_t size)
        :
        buf_( (T *)calloc(size, sizeof(*buf_)) ),
        size_(size),
        maxSize_(max<size_t >(size, 1)),
        errNo_(E_OK) {


#ifdef DEBUG
    VECTOR_OK();
#else
    assert(buf_ != nullptr);
    assert(maxSize_ > 0);
#endif
}

template <class T>
Vector<T>::Vector(size_t size, const T & initialVal) : Vector(size) {
    for(size_t i = 0; i < size; ++i) {
        buf_[i] = initialVal;
    }
#ifdef DEBUG
    VECTOR_OK();
#endif
}


template <class T>
Vector<T>::~Vector() {

    free(buf_);
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
        buf_( (T *)calloc(that.getMaxSize(), sizeof(*buf_)) ),
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
void Vector<T>::pushBack(const T &d) {
    if(size_ >= maxSize_) {
        maxSize_ = maxSize_ * RESIZE_COEF;
        buf_ = (T *)realloc (buf_, maxSize_ * sizeof (*buf_));
    }

    buf_[size_] = d;

    ++size_;
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

template <class T>
void Vector<T>::dump () const {

    const char *filename = "dump.log";

    FILE *fdump = fopen (filename, "a+");

    if (fdump == nullptr) {
        printf ("Can't open file %s", filename);
        exit(1);
    }


    if (errNo_ == E_OK) {
        fprintf(fdump, "%s", errMessage[errNo_]);
    }
    else {
        fprintf(fdump, "!!!ERROR <%d>: %s !!!\n",
                errNo_,
                errMessage[errNo_]);
    }

    fprintf(fdump, " [%p] {\n"
                   "  current size     = %d\n"
                   "  maximum capacity = %d\n"
                   "  errno            = %d\n"
                   "  data[%d]         = [%p]:\n",
            this,
            size_,
            maxSize_,
            errNo_,
            size_,
            buf_);


    if (buf_ != nullptr) {
        for (int i = 0; i < maxSize_; i++) {
            if (i < size_) {
                fprintf(fdump, "   *[%d] = %d\n", i, buf_[i]);
            } else {
                fprintf(fdump, "    [%d] = %d\n", i, buf_[i]);
            }
        }
    }
    else {
        fprintf(fdump, "   NO DATA\n");
    }

    fprintf(fdump, "}\n");

    fclose (fdump);
}
