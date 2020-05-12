#include <iostream>
#include <assert.h>
#include "HuffmanArchiver.h"

using namespace std;


int main(int argc, char *argv[]) {

    if (argc == 1) {
        cout << "Please, input at least one filename to compress" << endl;
        return 1;
    }

    char archName[ARCHIVE_NAME_SIZE];
    cout << "Please, input the name of the archive" << endl;
    cin >> archName;

    HuffmanCompress(*argv[1], argc - 1, archName);

    return 0;
}