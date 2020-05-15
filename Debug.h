//
// Created by ready on 14.05.2020.
//

#ifndef ARCHRONIS_DEBUG_H
#define ARCHRONIS_DEBUG_H

#include <iostream>
#include <iomanip> /* setw() */
#include <string>
#include <bitset>

#include "Vector/Vector.h"
#include "Tree/Tree.h"
#include "HuffmanArchiver.h"

using namespace std;

template <class T>
void printForLetters(const Vector<T> &v,
                     const string    &msg) {

    cout << msg << endl;
    for (size_t c = 0; c < v.getSize(); ++c) {
        if (v[c]) {
            cout << char(c)
                 << ": "
                 << MY_SIZE_T (v[c])
                 << endl;
        }
    }
}


void printTree(const Tree<Letter>::Node * curr,
                     size_t               level) {

    if (curr) {
            printTree(curr->rightChild, level + 1);

            cout << setw(level * 5)
                 << char(curr->data.letter)
                 << ": "
                 << curr->data.frequency
                 << ": "
                 << level
                 << endl;
            printTree(curr->leftChild, level + 1);
        }
}


void printFirstCode(const Vector<MY_BYTE> &firstCode) {

    cout << "First codes: " << endl;
    for (int c = 1; c < firstCode.getSize(); ++c) {
        cout << c
             << ": ";
        for(int i = c; i >= 0; --i) {
            cout << ((firstCode[c] >> i) & 1);
        }
        cout << endl;
    }
}

void printCodes(const Vector<Vector<bool>> &table) {

    cout << "Codes: " << endl;
    for (int c = 0; c < table.getSize(); ++c) {
        if (table[c].getSize()) {
            cout << char(c)
                 << ": ";
            for (int n = table[c].getSize() - 1; n >= 0; --n) {
                cout << table[c][n];
            }
            cout << endl;
        }
    }
}

template <class T>
void printVector(const Vector<T> &v,
                 const string    &msg) {

    cout << msg << endl;
    cout << v;
}


#endif //ARCHRONIS_DEBUG_H
