//
// Created by ready on 14.05.2020.
//

#ifndef ARCHRONIS_DEBUG_H
#define ARCHRONIS_DEBUG_H

#include <iostream>
#include <iomanip> /* setw() */
#include <string>
#include <bitset>

#include "../Vector/Vector.h"
#include "../Tree/Tree.h"
#include "Huffman.h"


template <class T>
void printForLetters(
        const Vector<T>      &v,
        const std::string    &msg) {

    std::cout << msg << std::endl;
    for (size_t c = 0; c < v.getSize(); ++c) {
        if (v[c]) {
            std::cout << char(c)
                 << ": "
                 << size_t (v[c])
                 << std::endl;
        }
    }
}


void printTree(
        const Tree<Huffman::Letter>::Node * curr,
               size_t                       level) {

    if (curr) {
            printTree(curr->rightChild, level + 1);

            std::cout << std::setw(level * 5)
                 << char(curr->data.letter)
                 << ": "
                 << curr->data.frequency
                 << ": "
                 << level
                 << std::endl;
            printTree(curr->leftChild, level + 1);
        }
}


void printFirstCode(const Vector<Huffman::BYTE> &firstCode) {

    std::cout << "First codes: " << std::endl;
    for (int c = 1; c < firstCode.getSize(); ++c) {
        std::cout << c
             << ": ";
        for(int i = c; i >= 0; --i) {
            std::cout << ((firstCode[c] >> i) & 1);
        }
        std::cout << std::endl;
    }
}


void printCodes(const Vector<Vector<bool>> &table) {

    std::cout << "Codes: " << std::endl;
    for (int c = 0; c < table.getSize(); ++c) {
        if (table[c].getSize()) {
            std::cout << char(c)
                 << ": ";
            for (int n = table[c].getSize() - 1; n >= 0; --n) {
                std::cout << table[c][n];
            }
            std::cout << std::endl;
        }
    }
}


template <class T>
void printVector(const Vector<T> &v,
                 const std::string    &msg) {

    std::cout << msg << std::endl;
    std::cout << v;
}


#endif //ARCHRONIS_DEBUG_H
