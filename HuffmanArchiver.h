
//
// Created by ready on 12.05.2020.
//

#ifndef ARCHRONIS_HUFFMANARCHIVER_H
#define ARCHRONIS_HUFFMANARCHIVER_H

#include <assert.h>
#include <fstream>
#include <limits>
#include <algorithm>
#include <string>
#include <cstdio>

#include "Tree/Tree.h"
#include "Queue/Queue.h"
#include "Vector/Vector.h"



using namespace std;


// Type name of variables holding filesizes in bytes
typedef unsigned long int MY_SIZE_T;

// ASCII chars byte
typedef unsigned char MY_BYTE;

//Number of ASCII chars
const size_t ALPHABET_SIZE = 1 + std::numeric_limits<MY_BYTE>::max();

const size_t BIT_SIZE = 8;

const size_t BYTE_STEP = 256;

//Name for the file with file names
const char INFO_FILE_NAME[] = "HuffmanArchiverInfo";

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

int HuffmanCompress(
        const Vector<string> &filesNames,
        const string         &archiveName);



int HuffmanDecompress(const string &archiveName);


MY_SIZE_T UpdateLettersFreqs(
        ifstream           &file,
        Vector<MY_SIZE_T>  &lettersFreqs);

const Tree<Letter> *BuildEncodeTree(
        const Vector<MY_SIZE_T>  &lettersFreqs,
              size_t             alphSize);

void ComputeFirstCodesForLengths(
        const Vector<MY_BYTE> &numberOfCodes,
              Vector<MY_BYTE> &firstCode);

void BuildTable(
        const Vector<MY_BYTE>          &numberOfCodes,
        const Vector<MY_BYTE>          &firstCode,
        const Vector<Vector<MY_BYTE >> &lettersByLength,
        Vector<Vector<bool>>     &table);

void CountLengthsOfCodes(
        const Tree<Letter>::Node       *root,
        Vector<MY_BYTE>          &codeLengths,
        MY_BYTE                  level);

void WriteFileNames(
        std::ofstream  &out,
        const Vector<string> &names);

int WriteBinary(
        std::ofstream &out,
        MY_SIZE_T     size);


void WriteFilesSizes(
        std::ofstream     &out,
        const Vector<MY_SIZE_T> &sizes,
        const Vector<string>    &names);


void WriteEncodingInfo(
        const Vector<MY_BYTE>          &numberOfCodes,
        const Vector<Vector<MY_BYTE >> &lettersByLength,
        std::ofstream            &out);


void WriteEncoding(
        std::ifstream        &in,
        std::ofstream        &out,
        const Vector<Vector<bool>> &table);


#endif //ARCHRONIS_HUFFMANARCHIVER_H

