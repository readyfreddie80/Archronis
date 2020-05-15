#include <iostream>

#include "HuffmanArchiver.h"


using namespace std;


int main(int argc, char *argv[]) {

    if (argc == 1) {
        cout << "Please, input at least one filename to compress" << endl;
        return 1;
    }

    Vector<string> filesNames;
    for(size_t i = 1; i < argc; ++i) {
        filesNames.pushBack(string(argv[i]));
    }

    cout << "Please, input the name of the archive" << endl;
    string archName;
    cin >> archName;

    HuffmanCompress(filesNames, archName);

    return 0;
}