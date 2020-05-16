#include "HuffmanArchiver.h"

#include <cstring>
#include <sstream>

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

int main(int argc, char *argv[]) {

    const std::string COMPRESS_COMMAND   = "-c";
    const std::string DECOMPRESS_COMMAND = "-d";

    if (argc < 3 ||
        (argv[1] != COMPRESS_COMMAND && argv[1] != DECOMPRESS_COMMAND)) {

        const std::string TITLE  = "Archonis archiver";
        const std::string AUTHOR = "sklyarova.ev@phystech.edu";
        const std::string DATE   = "2020";

        const std::string DESCRIPTION = "The program uses Huffman`s algorithm for compressing of N files (N >= 1) and decompressing of an archive.";

        const Vector<ProgramInfo::Option> OPTIONS {
                {"Compressing mode",   COMPRESS_COMMAND,   "in-file1 [in-file2 ... in-fileN] new-archive-name"},
                {"Decompressing mode", DECOMPRESS_COMMAND, "archive-to-decompress"}
        };

        const ProgramInfo PROGRAM_INFO{
                TITLE,
                AUTHOR,
                DATE,
                DESCRIPTION,
                OPTIONS
        };

        PROGRAM_INFO.coutProgramInfo(argv[0]);
        return 1;
    }

    if (argv[1] == COMPRESS_COMMAND) {
        Vector<std::string> filesNames;
        auto archiveNameIdx = static_cast<size_t >(argc - 1);
        for (size_t i = 2; i < archiveNameIdx; ++i) {
            filesNames.pushBack(argv[i]);
        }

        HuffmanCompress(filesNames, argv[archiveNameIdx]);

    } else {
        HuffmanDecompress(argv[2]);
    }
        return 0;
}

