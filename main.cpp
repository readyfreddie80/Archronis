#include <iostream>

#include "HuffmanArchiver.h"
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) {

    if (   argc == 1
        || (strcmp(argv[1], "-c") != 0 && strcmp(argv[1], "-d") != 0)) {
        cout << "To compress:   Archronis -c [files]" << endl;
        cout << "To decompress: Archronis -d [archive-file]";
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0) {

        if (argc < 2)  {
            cout << "Please, input at least one filename";
            return 1;
        }

        Vector<string> filesNames;
        for (size_t i = 2; i < argc; ++i) {
            filesNames.pushBack(string(argv[i]));
        }

        cout << "Please, input the name of the archive" << endl;
        string archName;
        cin >> archName;

        HuffmanCompress(filesNames, archName);
    }

    else {
        string archName(argv[2]);
        HuffmanDecompress(archName);
    }

    return 0;
}

