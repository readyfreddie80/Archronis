
//
// Created by ready on 12.05.2020.
//

#ifndef ARCHRONIS_HUFFMANARCHIVER_H
#define ARCHRONIS_HUFFMANARCHIVER_H

#include<fstream>
using namespace std;

const size_t ARCHIVE_NAME_SIZE = 32;

size_t FileSize(const char *fileName) {
    assert(fileName != nullptr);
    ifstream file(fileName, ifstream::in | ifstream::binary);
    file.seekg(0, ifstream::end);
    return file.tellg();
}


int HuffmanCompress(const char *filesNames[], size_t filesNumber, const char *archiveName) {
    assert(filesNames);
    assert(filesNumber > 0);
    assert(archiveName);

    auto *fileStreams = new ifstream[filesNumber];
    for(size_t i = 0; i < filesNumber; i++) {
        assert(filesNames[i]);
        fileStreams[i].open(filesNames[i], ios::in | ios::binary);

        if (!fileStreams[i].is_open()) {
            cout << "Can't open file to compress: "
                 << filesNames[i] << endl;
            return 1;
        }
    }

    ifstream archiveStream(archiveName, ios::out | ios::binary);
    if (!archiveStream.is_open()) {
        cout << "Can't create an archive with the name: "
             << archiveName << endl;
        return 1;
    }



    for(size_t i = 0; i < filesNumber; i++) {
        fileStreams[i].close();

    }
    delete[] fileStreams;

    archiveStream.close();

    return 0;
}


#endif //ARCHRONIS_HUFFMANARCHIVER_H

