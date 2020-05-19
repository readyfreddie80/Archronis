//
// Created by ready on 17.05.2020.
//

#ifndef ARCHRONIS_ARCHIVER_H
#define ARCHRONIS_ARCHIVER_H

#include <fstream>
#include <string>

#include "../Vector/Vector.h"
#include "File.h"
#include "Huffman.h"

class Archiver {
public:

    void Compress(
            const Vector<std::string> &filesNames,
            const std::string         &archiveName);

    Vector<std::string> Decompress(const std::string &archiveName);

    Archiver() = default;

private:
    //Name for the file with file names
    static constexpr char INFO_FILE_NAME[] = "ArchronisInfo";

    Huffman huffman;

};

#endif //ARCHRONIS_ARCHIVER_H
