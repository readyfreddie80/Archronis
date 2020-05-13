
//
// Created by ready on 12.05.2020.
//

#ifndef ARCHRONIS_HUFFMANARCHIVER_H
#define ARCHRONIS_HUFFMANARCHIVER_H

#include <assert.h>
#include <fstream>
#include <limits>

#include "Tree/Tree.h"
#include "Queue/Queue.h"

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

MY_SIZE_T CountLettersFreqs(ifstream *file, MY_SIZE_T *lettersFreqs);

MY_SIZE_T FileSize(ifstream *file);

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

MY_SIZE_T FileSize(ifstream *file) {
    assert(file);
    assert(file->is_open());

    file->seekg(0, ifstream::end);
    MY_SIZE_T size = file->tellg();
    file->seekg(0, ifstream::beg);

    return size;
}

MY_SIZE_T UpdateLettersFreqs(ifstream *file, MY_SIZE_T *lettersFreqs) {
    assert(file);
    assert(file->is_open());
    assert(lettersFreqs);

    file->seekg(0, ifstream::beg); //Reset get() pointer to beginning
    file->clear();                 //Clear EOF flag

    //Size of the file in bytes
    MY_SIZE_T byteSizeFile = 0;

    //Read the whole file and update the table's frequencies
    while (!file->eof()) {
        MY_BYTE c = file->get();
        lettersFreqs[c]++;

        byteSizeFile++;
    }
    file->seekg(0, ifstream::beg); //Reset get() pointer to beginning
    file->clear();                 //Clear EOF flag

    return byteSizeFile;
}

int HuffmanCompress(const char *filesNames[], size_t filesNumber, const char *archiveName) {
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
    auto *fileStreams = new ifstream[filesNumber];
    // Frequency table for all ASCII chars in all the files for compressing
    auto *lettersFreqs = new MY_SIZE_T[ALPHABET_SIZE];
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        lettersFreqs[i] = 0;
    }
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
        UpdateLettersFreqs(fileStreams + i, lettersFreqs);
    }

    archive.close();

    for(size_t i = 0; i < filesNumber; i++) {
        fileStreams[i].close();

    }

    delete[] lettersFreqs;

    return 0;
}

const Tree<Letter> *BuildEncodeTree(const MY_SIZE_T *lettersFreqs) {
    Queue<Tree<Letter>> Q(ALPHABET_SIZE, 3);
}




#endif //ARCHRONIS_HUFFMANARCHIVER_H

