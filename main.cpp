#include "ProgramInfo/ProgramInfo.h"
#include "Archive/Archiver.h"

int main(int argc, char *argv[]) {

    const std::string COMPRESS_COMMAND   = "-c";
    const std::string DECOMPRESS_COMMAND = "-d";

    const bool isCompress   = argv[1] == COMPRESS_COMMAND;
    const bool isDecompress = argv[1] == DECOMPRESS_COMMAND;

    if (argc < 3
        || (!isCompress && !isDecompress)
        || (isCompress && argc < 4)
        || (isDecompress && argc != 3)) {

        // Print how to use  info

        const std::string TITLE  = "Archonis archiver";
        const std::string AUTHOR = "sklyarova.ev@phystech.edu";
        const std::string DATE   = "2020";

        const std::string DESCRIPTION = "The program uses static Archive`s coding for:\n"
                                        " -compressing N files (N >= 1) to an archive\n"
                                        " -decompressing an archive";

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

    auto archiveNameIdx = static_cast<size_t >(argc - 1);

    Archiver archive;

    if (isCompress) {
        Vector<std::string> filesNames;
        for (size_t i = 2; i < archiveNameIdx; ++i) {
            filesNames.pushBack(argv[i]);
        }
        std::cout << "Compressing into "
                  << argv[archiveNameIdx]
                  << "..."
                  << std::endl;
        archive.Compress(filesNames, argv[archiveNameIdx]);
        std::cout << "Compressing finished successfully!"
                  << std::endl;;

    } else {
        std::cout << "Decompressing from "
                  << argv[archiveNameIdx]
                  << "..."
                  << std::endl;
        Vector<std::string> filesNames = archive.Decompress(argv[archiveNameIdx]);
        std::cout << "Decompressing finished successfully!"
                  << std::endl
                  << "Decompressed files:"
                  << std::endl;

        for (size_t i = 0; i < filesNames.getSize(); ++i) {
            std::cout << filesNames[i]
                      << std::endl;
        }
    }
        return 0;
}

