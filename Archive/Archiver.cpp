//
// Created by ready on 17.05.2020.
//

#include "Archiver.h"

constexpr char Archiver::INFO_FILE_NAME[];

void Archiver::Compress(
        const Vector<std::string> &filesNames,
        const std::string         &archiveName) {

#ifdef DEBUG
    assert(filesNames.getSize() > 0);
    assert(!archiveName.empty());
#endif

    //Try to open an archive file
    std::fstream archiveStream;
    File archive{archiveName};
    archive.OpenFile(archiveStream, std::ios::out | std::ios::binary);

    InfoFile infoFile{INFO_FILE_NAME};
    infoFile.WriteFilesNamesInto(filesNames);

    // +1 is for the info file which will be compressed as well
    size_t filesToCompressNumb = filesNames.getSize() + 1;
    Vector<File> filesToCompress{filesToCompressNumb};

    //The first file for compressing is always the info file
    filesToCompress[0] = File{INFO_FILE_NAME};

    for(size_t i = 1; i < filesToCompressNumb; ++i) {
        filesToCompress[i] = File{filesNames[i - 1]};
    }

    huffman.Compress(filesToCompress, archiveStream);
}

void Archiver::Decompress(const std::string &archiveName) {
#ifdef DEBUG
    assert(!archiveName.empty());
#endif

    //Try to open an archive file
    std::fstream archiveStream;
    File archive{archiveName};
    archive.OpenFile(archiveStream, std::ios::in | std::ios::binary);

    InfoFile infoFile{INFO_FILE_NAME};

    huffman.Decompress(archiveStream, infoFile);

}
