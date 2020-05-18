//
// Created by ready on 17.05.2020.
//

#ifndef ARCHRONIS_FILE_H
#define ARCHRONIS_FILE_H

#include <string>
#include <fstream>
#include <iostream>

#include "../Vector/Vector.h"

class File {
protected:
    std::string  name_;
    std::size_t  size_;

public:
    explicit File(
            const std::string &name = "",
                  size_t      size  = 0);

    void OpenFile(
            std::fstream            &stream,
            std::ios_base::openmode mode) const;

    void SetName(const std::string &name);
    void SetSize(size_t size);

    size_t GetSize() const;
    const std::string & GetName() const;
    virtual ~File() = default;

};



class InfoFile : public File {
public:
    explicit InfoFile(
            const std::string &name = "",
                  size_t size       = 0)
            : File(name, size) {}

    ~InfoFile() override;

    void WriteFilesNamesInto(const Vector<std::string> &names) const;

    int ReadFileNames (Vector<std::string> &names) const;
};


#endif //ARCHRONIS_FILE_H
