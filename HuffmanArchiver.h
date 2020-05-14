
//
// Created by ready on 12.05.2020.
//

#ifndef ARCHRONIS_HUFFMANARCHIVER_H
#define ARCHRONIS_HUFFMANARCHIVER_H

#include <assert.h>
#include <fstream>
#include <limits>
#include <algorithm>

#include "Tree/Tree.h"
#include "Queue/Queue.h"
#include "Vector/Vector.h"

using namespace std;


// Type name of variables holding filesizes in bytes
typedef unsigned long long MY_SIZE_T;

// ASCII chars byte
typedef unsigned char MY_BYTE;

//Number of ASCII chars
const size_t ALPHABET_SIZE = 1 + std::numeric_limits<MY_BYTE>::max();

// Size of thr name of the archive file
const size_t ARCHIVE_NAME_SIZE = 32;

const char *INFO_FILE_NAME = "info.txt";

int HuffmanCompress(const char *filesNames[], size_t filesNumber, const char *archiveName);


///////////////////////////////////////////////////////

struct Letter {
    MY_SIZE_T frequency;
    MY_BYTE letter;

    explicit Letter(MY_SIZE_T freq = 0, MY_BYTE let = '\0')
            :frequency(freq),
             letter(let){}
    bool operator>(const Letter & L) const {
        return frequency > L.frequency;
    }

};

void CountLengthsOfCodes(const Tree<Letter>::Node *root,
                         Vector<MY_BYTE> & codeLengths,
                         MY_BYTE level);


MY_SIZE_T FileSize(ifstream *file);

const Tree<Letter> *BuildEncodeTree(const Vector<MY_SIZE_T> & lettersFreqs,
                                    size_t alphSize);


/*
MY_SIZE_T FileSize(ifstream *file) {
    assert(file);
    assert(file->is_open());

    file->seekg(0, ifstream::end);
    MY_SIZE_T size = file->tellg();
    file->seekg(0, ifstream::beg);

    return size;
}
*/

MY_SIZE_T UpdateLettersFreqs(ifstream *file,
                             Vector<MY_SIZE_T> & lettersFreqs) {
    assert(file);
    assert(file->is_open());

    file->seekg(0, ifstream::beg); //Reset get() pointer to beginning
    file->clear();                 //Clear EOF flag

    //Size of the file in bytes
    MY_SIZE_T byteSizeFile = 0;

    //Read the whole file and update the table's frequencies
    while (!file->eof()) {
        auto c = static_cast<MY_BYTE>(file->get());
        lettersFreqs[c]++;

        byteSizeFile++;
    }
    file->seekg(0, ifstream::beg); //Reset get() pointer to beginning
    file->clear();                 //Clear EOF flag

    return byteSizeFile;
}

int HuffmanCompress(const char *filesNames[],
                    size_t filesNumber,
                    const char *archiveName) {
    assert(filesNames);
    assert(filesNumber > 0);
    assert(archiveName);

    //Stream to associate with archive file
    ifstream archive(archiveName, ios::out | ios::binary);
    if (!archive.is_open()) {
        cerr << "Can't create an archive with the name: "
             << archiveName << endl;
        return 1;
    }
    //Streams to associate with each of the files for compressing

    Vector<ifstream> fileStreams(filesNumber);
    // Frequency table for all ASCII chars in all the files for compressing
    //Array's indices correspond to ASCII code of each letter
    Vector<MY_SIZE_T>lettersFreqs(ALPHABET_SIZE, 0);

    // For all the files for compressing
    for(size_t i = 0; i < filesNumber; i++) {
        assert(filesNames[i]);
        // Open the current file
        fileStreams[i].open(filesNames[i], ios::in | ios::binary);

        if (!fileStreams[i].is_open()) {
            cerr << "Can't open file to compress: "
                 << filesNames[i] << endl;
            return 1;
        }
        //Update the frequency table with the current file's statistics
        UpdateLettersFreqs(&fileStreams[i], lettersFreqs);
    }
    //Build Huffman encode tree
    const Tree<Letter> *encodeTree = BuildEncodeTree(lettersFreqs, ALPHABET_SIZE);

    // If at least one file isn't empty
    if (encodeTree != nullptr) {

        //Find length of code for each letter
        //Array's indices correspond to ASCII code of each letter
        Vector<MY_BYTE> codeLengths(ALPHABET_SIZE, 0);
        CountLengthsOfCodes(encodeTree->getRoot(), codeLengths, 0);

        // Max length of code among all the letters
        MY_BYTE maxLength = codeLengths[0];
        for(size_t i = 1; i < ALPHABET_SIZE; ++i) {
            if (maxLength > codeLengths[i]) {
                maxLength = codeLengths[i];
            }
        }

        //Find number of codes for each length
        //Array's indices correspond to length of code
        Vector<MY_BYTE> numberOfCodes(maxLength + 1, 0);
        for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
            ++numberOfCodes[codeLengths[i]];
        }

        //Distribute letters by code length
        //First indices correspond to length of code
        Vector<Vector<MY_BYTE>> LettersByLength(maxLength + 1);
        for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
            if (lettersFreqs[i]) {
                LettersByLength[i].pushBack(static_cast<MY_BYTE>(i));
            }
        }




    }


    archive.close();

    for(size_t i = 0; i < filesNumber; i++) {
        fileStreams[i].close();
    }

    delete encodeTree;

    return 0;
}


void CountLengthsOfCodes(const Tree<Letter>::Node *root,
                         Vector<MY_BYTE> & codeLengths,
                         MY_BYTE level) {
    assert(root);

    if (!root->leftChild && !root->rightChild) {
        codeLengths[root->data.letter] = max<MY_BYTE >(level, 1);
    }

    if (root->leftChild)
        CountLengthsOfCodes(root->leftChild,  codeLengths, level + 1);

    if (root->rightChild)
        CountLengthsOfCodes(root->rightChild, codeLengths, level + 1);

}
const Tree<Letter> *BuildEncodeTree(const Vector<MY_SIZE_T> & lettersFreqs,
                                    size_t alphSize) {
    assert(alphSize > 0);

    // The increasing-ordered priority queue built on 3-ary heap
    Queue<Tree<Letter>> Q(alphSize, 3);
    // Put all the letters whose frequencies aren't equal to zero into the priority queue
    for (size_t i = 0; i < alphSize; i++) {
        if (lettersFreqs[i] != 0) {
            auto *tr = new Tree<Letter>(Letter(lettersFreqs[i], static_cast<MY_BYTE>(i)));
            Q.enqueue(tr);
        }
    }
    // There is nothing to encode
    if (Q.isEmpty()) {
        return nullptr;
    }

    while(Q.getSize() > 1) {

        //Get 2 front elements from the increasing-ordered priority queue
        //Combine them into one
        //And put back into the queue

        Tree<Letter> *Left  = Q.dequeue();
        Tree<Letter> *Right = Q.dequeue();

        Letter leftLet  = Left->getRootData();
        Letter rightLet = Right->getRootData();

        Tree<Letter> *parent = new Tree<Letter>(Letter(leftLet.frequency + rightLet.frequency));
        parent->setLeft(Left->getRoot());
        parent->setRight(Right->getRoot());

    }


    //Now the priority queue consists only one element which is the root of the resulting tree
    return Q.dequeue();
}




#endif //ARCHRONIS_HUFFMANARCHIVER_H

