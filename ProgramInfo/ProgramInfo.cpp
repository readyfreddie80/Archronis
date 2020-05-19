//
// Created by ready on 17.05.2020.
//

#include "ProgramInfo.h"

void coutStringStartingWithLattice(const std::string &str) {
    std::cout << "# " << str << std::endl;
}

void ProgramInfo::coutProgramInfo(const std::string &programName) const {

    /* # [TITLE]
     * # (c)[AUTHOR], [DATE]
     *
     * # [DESCRIPTION]
     *
     * # HOW TO USE:
     * # [OPTION_1 DESCRIPTION]
     * # [PROGRAM NAME] [OPTION_1] [OPTION_1 ARGUMENTS]
     * # [OPTION_2 DESCRIPTION]
     * # [PROGRAM NAME] [OPTION_2] [OPTION_2 ARGUMENTS]
     * # **********************************************
     * # [OPTION_N DESCRIPTION]
     * # [PROGRAM NAME] [OPTION_N] [OPTION_N ARGUMENTS]
     */


    coutStringStartingWithLattice(title);

    std::string copyright = "(c)"
                            + author
                            + ", "
                            + date;

    coutStringStartingWithLattice(copyright);

    std::cout  << std::endl;

    coutStringStartingWithLattice(description);

    std::cout  << std::endl;

    coutStringStartingWithLattice("HOW TO USE:");

    // Cout how to use for each option
    size_t optionsNum = options.getSize();
    for(size_t i = 0; i < optionsNum; ++i) {
        ProgramInfo::Option option = options[i];

        coutStringStartingWithLattice(option.description);

        std::string command = programName
                              + " "
                              + option.token
                              + " "
                              + option.arguments;

        coutStringStartingWithLattice(command);
    }
}
