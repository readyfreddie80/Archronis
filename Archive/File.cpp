//
// Created by ready on 17.05.2020.
//

#include "File.h"

File::File(
        const std::string &name,
              size_t      size)
        :
        name_(name),
        size_(size) {}

void File::OpenFile(
        std::fstream            &stream,
        std::ios_base::openmode mode = std::ios_base::out) const {
    stream.open(name_, mode);

    if (!stream.is_open()) {
        std::cerr << "Can't open file with the name: "
                  << name_
                  << std::endl;
        exit(1);
    }
}

void File::SetSize(size_t size) {
    size_ = size;
}

void File::SetName(const std::string &name) {
    name_ = name;
}

size_t File::GetSize() const {
    return size_;
}

const std::string & File::GetName() const {
    return name_;
}

InfoFile::~InfoFile() {
    if (remove(name_.c_str()) != 0) {
        std::cerr << "Error of removing file "
                  << name_
                  << std::endl;
    }
}

void InfoFile::WriteFilesNamesInto(const Vector<std::string> &names) const {
    //Try to open info file to save the names of the given files
    std::fstream infoFile;
    OpenFile(infoFile, std::ios::out | std::ios::binary);

    size_t namesNumb = names.getSize();
    for (size_t i = 0; i < namesNumb; ++i) {
        infoFile << names[i] << "|";
    }
}


int InfoFile::ReadFileNames (Vector<std::string> &names) const {
    std::fstream infoFile;
    OpenFile(infoFile, std::ios::in | std::ios::binary);

    for(size_t i = 0; i < names.getSize(); ++i) {
        std::string name;
        getline(infoFile, name, '|');
        if (name.empty()) {
            return 1;
        }
        names[i] = name;
    }
    return 0;
}
