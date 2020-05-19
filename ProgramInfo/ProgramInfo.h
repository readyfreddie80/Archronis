//
// Created by ready on 17.05.2020.
//

#ifndef ARCHRONIS_PROGRAMINFO_H
#define ARCHRONIS_PROGRAMINFO_H

#include <iostream>
#include <string>

#include "../Vector/Vector.h"

struct ProgramInfo {
    struct Option {
        std::string description;
        std::string token;
        std::string arguments;
    };

    void coutProgramInfo(const std::string &programName) const;

    ProgramInfo(
            const std::string    &newTitle,
            const std::string    &newAuthor,
            const std::string    &newDate,
            const std::string    &newDescription,
            const Vector<Option> &newOptions)
            : title(newTitle),
              author(newAuthor),
              date(newDate),
              description(newDescription),
              options(newOptions) {}

private:
    std::string    title;
    std::string    author;
    std::string    date;
    std::string    description;
    Vector<Option> options;

};


void coutStringStartingWithLattice(const std::string &str);


#endif //ARCHRONIS_PROGRAMINFO_H
