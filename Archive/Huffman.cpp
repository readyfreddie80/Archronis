//
// Created by ready on 17.05.2020.
//

#include "Huffman.h"

#ifdef DEBUG
#include "Debug.h"
#endif

#include "../Queue/Queue.h"

void Huffman::Compress(
        Vector<File> &files,
        std::fstream &archive) {

    size_t filesNumb = files.getSize();

#ifdef DEBUG
    assert(filesNumb != 0);
    assert(archive.is_open());
#endif

    // Frequency table for all ASCII chars in all the files for compressing
    //Array's indices correspond to ASCII code of each letter
    Vector<size_t>lettersFreqs(ALPHABET_SIZE, 0);

    for (size_t i = 0; i < filesNumb; ++i) {
        //Update the frequency table with the current file's statistics
        //Also update file size
        UpdateLettersFreqsAndFileSize(files[i], lettersFreqs);
    }

#ifdef DEBUG
    std::cout << "Files:" << std::endl;
    for(size_t i = 0; i < filesNumb; ++i) {
        std::cout << files[i].GetName()
                  << " : "
                  << files[i].GetSize()
                  << std::endl;
    }

    printForLetters(lettersFreqs, "Frequencies:");
#endif

    //Build Archive encode tree
    const Tree<Letter> *encodeTree = BuildEncodeTree(lettersFreqs);
    if (encodeTree == nullptr) {
        std::cerr << "All the files are empty! There's nothing to compress!";
        exit(1);
    }

#ifdef DEBUG
    std::cout << "Tree:" << std::endl;
    printTree(encodeTree->getRoot(), 0);
#endif

    //Count length of code for each letter
    //Array's indices correspond to ASCII code of each letter
    Vector<BYTE> codeLengths(ALPHABET_SIZE, 0);
    CountLengthsOfCodes(encodeTree->getRoot(), codeLengths, 0);

#ifdef DEBUG
    printForLetters(codeLengths, "Code lengths: ");
#endif

    // Max length of code among all the letters
    const BYTE maxLength = FindMaxCodeLength(codeLengths);

#ifdef DEBUG
    std::cout << "Max-code length: " << size_t(maxLength) << std::endl;
#endif

    //Count number of codes for each length
    //Array's indices correspond to length of code
    Vector<BYTE> numberOfCodes(maxLength + 1, 0);
    FillNumberOfCodesForLengths(codeLengths, numberOfCodes);

    //Distribute letters by code length
    //First indices correspond to length of code
    Vector<Vector<BYTE>> lettersByLength(maxLength + 1);
    FillLettersByLength(codeLengths, lettersFreqs, lettersByLength);

    // Compute first code for each length
    //First indices correspond to length of code
    Vector<BYTE> firstCodes(maxLength + 1, 0);
    ComputeFirstCodesForLengths(numberOfCodes, firstCodes);

#ifdef DEBUG
    printFirstCode(firstCodes);
#endif

    //Build table with the code for each letter
    Vector<Vector<bool>> table(ALPHABET_SIZE);
    BuildTable(numberOfCodes,
               firstCodes,
               lettersByLength,
               table);

#ifdef DEBUG
    printCodes(table);
#endif
    //Write all the encoding info into the beginning of the archive
    WriteEncodingInfo(numberOfCodes,
                      lettersByLength,
                      files,
                      archive);
    for(size_t i = 0; i < filesNumb; i++) {
        std::fstream stream;
        files[i].OpenFile(stream, std::ios::in | std::ios::binary);
        WriteEncoding(table, stream, archive);
    }

    delete encodeTree;
}

void Huffman::Decompress(
              std::fstream &archive,
        const InfoFile     &infoFile) {

    //Count number of codes for each length
    //Array's indices correspond to length of code
    Vector<BYTE > numberOfCodes(1, 0);
    //Encoded letters
    Vector<BYTE> letters;
    Vector<size_t> fileSizes;

    //Distribute letters by code length
    //First indices correspond to length of code
    Vector<Vector<BYTE >> lettersByLength = ReadDecodingInfo(numberOfCodes,
                                                             letters,
                                                             fileSizes,
                                                             archive);
#ifdef DEBUG
    printVector(fileSizes, "File sizes:");
#endif

    Vector<BYTE> firstCodes(numberOfCodes.getSize(), 0);
    ComputeFirstCodesForLengths(numberOfCodes, firstCodes);

#ifdef DEBUG
    printFirstCode(firstCodes);
#endif

    Vector<Vector<bool>> table(ALPHABET_SIZE);
    BuildTable(numberOfCodes,
               firstCodes,
               lettersByLength,
               table);
#ifdef DEBUG
    printCodes(table);
#endif

    const Tree<Letter> *root = BuildDecodeTree(letters, table);

#ifdef DEBUG
    std::cout << "Tree:" << std::endl;
    printTree(root->getRoot(), 0);
#endif

    std::fstream infoFileStream;
    infoFile.OpenFile(infoFileStream,
                      std::ios::binary | std::ios::out);
    WriteDecoding(archive,
                 infoFileStream,
                 root->getRoot(),
                 fileSizes[0]);

    infoFileStream.close();


    Vector<std::string> fileNames(fileSizes.getSize() - 1);
    infoFile.ReadFileNames(fileNames);

#ifdef DEBUG
    printVector(fileNames, "File names:");
#endif
    for(size_t i = 0; i < fileNames.getSize(); i++) {
        std::fstream stream;
        File file{fileNames[i]};
        file.OpenFile(stream, std::ios::binary | std::ios::out);

        WriteDecoding(archive,
                     stream,
                     root->getRoot(),
                     fileSizes[i + 1]);
    }

    delete root;
}


void Huffman::UpdateLettersFreqsAndFileSize(
                                 File           &file,
                                 Vector<size_t> &lettersFreqs) {
#ifdef DEBUG
    assert(ALPHABET_SIZE == lettersFreqs.getSize());
#endif

    std::fstream stream;
    file.OpenFile(stream, std::ios::in | std::ios::binary);

    //Size of the file in bytes
    size_t byteSizeFile = 0;

    //Read the whole file and update the table's frequencies
    while (true) {
        auto c = static_cast<BYTE>(stream.get());

        if (stream.eof()) break;

        lettersFreqs[c]++;
        byteSizeFile++;
    }

    file.SetSize(byteSizeFile);
}


const Tree<Huffman::Letter> * Huffman::BuildEncodeTree(const Vector<size_t> &lettersFreqs) {

    size_t lettersNumb = lettersFreqs.getSize();
#ifdef DEBUG
    assert(lettersNumb != 0);
#endif

    // The increasing-ordered priority queue built on 3-ary heap
    Queue<Tree<Letter>> Q(lettersNumb, 3);

    // Put all the letters whose frequencies aren't equal to zero into the priority queue
    for (size_t i = 0; i < lettersNumb; i++) {
        if (lettersFreqs[i] != 0) {
            auto *tr = new Tree<Letter>(Letter(lettersFreqs[i],
                                        static_cast<BYTE>(i)));
            Q.enqueue(tr);
        }
    }
    // There is nothing to encode
    if (Q.isEmpty()) {
        return nullptr;
    }

    while(Q.getSize() > 1) {

        //Get 2 front elements from the increasing-ordered priority queue
        //Combine them into one
        //And put back into the queue

        Tree<Letter> *Left  = Q.dequeue();
        Tree<Letter> *Right = Q.dequeue();

        Letter leftLet  = Left->getRootData();
        Letter rightLet = Right->getRootData();

        Tree<Letter> *parent = new Tree<Letter>(Letter(leftLet.frequency + rightLet.frequency));
        parent->setLeft(Left->getRoot());
        parent->setRight(Right->getRoot());

        Q.enqueue(parent);
    }


    //Now the priority queue consists only one element which is the root of the resulting tree
    return Q.dequeue();
}


const Tree<Huffman::Letter> * Huffman::BuildDecodeTree(
        const Vector<BYTE>         &letters,
        const Vector<Vector<bool>> &table) {

    auto *root = new Tree<Letter>(Letter());

    for (size_t l = 0; l < letters.getSize(); ++l) {
        int size = table[letters[l]].getSize();
        Tree<Letter>::Node *cur = root->getRoot();
        for (int bit = size - 1; bit != -1; --bit) {
            if (table[letters[l]][bit]) { // go right
                if (!cur->rightChild) {
                    auto *R = new Tree<Letter>::Node(Letter());
                    cur->rightChild = R;
                }
                cur = cur->rightChild;
            }
            else { //go left
                if (!cur->leftChild) {
                    auto *L = new Tree<Letter>::Node(Letter());
                    cur->leftChild = L;
                }
                cur = cur->leftChild;
            }
        }
        cur->data.letter = letters[l]; //leaf
    }
    return root;
}


void Huffman::CountLengthsOfCodes(
                   const Tree<Huffman::Letter>::Node *root,
                         Vector<BYTE>                &codeLengths,
                         BYTE                        level) {
#ifdef DEBUG
    assert(root);
    assert(ALPHABET_SIZE == codeLengths.getSize());
#endif
    if (!root->leftChild && !root->rightChild) {
        codeLengths[root->data.letter] = std::max<BYTE>(level, 1);
    }

    if (root->leftChild) {
        CountLengthsOfCodes(root->leftChild,
                            codeLengths,
                            level + static_cast<BYTE>(1));
    }

    if (root->rightChild) {
        CountLengthsOfCodes(root->rightChild,
                            codeLengths,
                            level + static_cast<BYTE>(1));
    }
}

Huffman::BYTE Huffman::FindMaxCodeLength(const Vector<BYTE> &codeLengths) {
#ifdef DEBUG
    assert(codeLengths.getSize() != 0);
#endif

    BYTE maxLength = codeLengths[0];
    for (size_t i = 1; i < codeLengths.getSize(); ++i) {
        if (maxLength < codeLengths[i]) {
            maxLength = codeLengths[i];
        }
    }
    return maxLength;
}

void Huffman::FillNumberOfCodesForLengths(
        const Vector<BYTE> &codeLengths,
              Vector<BYTE> &numberOfCodes) {
#ifdef DEBUG
    assert(numberOfCodes.getSize() != 0);
#endif

    size_t codeLengthsSize = codeLengths.getSize();
    for (size_t i = 0; i < codeLengthsSize; ++i) {
        ++numberOfCodes[codeLengths[i]];
    }

    //Ignore zero-length codes
    numberOfCodes[0] = static_cast<BYTE>(0);
}


void Huffman::FillLettersByLength(
        const Vector<BYTE>         &codeLengths,
        const Vector<size_t>       &lettersFreqs,
              Vector<Vector<BYTE>> &lettersByLength) {
    size_t lettersFreqsSize = lettersFreqs.getSize();

#ifdef DEBUG
    assert(codeLengths.getSize() <= lettersFreqsSize);
#endif

    for (size_t i = 0; i < lettersFreqsSize; ++i) {
        if (lettersFreqs[i]) {
            lettersByLength[codeLengths[i]].pushBack(static_cast<BYTE>(i));
        }
    }
}

void Huffman::ComputeFirstCodesForLengths(
        const Vector<BYTE> &numberOfCodes,
              Vector<BYTE> &firstCode) {

    size_t numberOfCodesSize = numberOfCodes.getSize();
    // Compute first code for each length
    firstCode[numberOfCodesSize - 1] = 0;

    for (size_t i = numberOfCodesSize - 2; i != 0; --i) {
        firstCode[i] = (firstCode[i + 1] + numberOfCodes[i + 1]) >> 1;
    }
}

void Huffman::BuildTable(
                         const Vector<BYTE>         &numberOfCodes,
                         const Vector<BYTE>         &firstCode,
                         const Vector<Vector<BYTE>> &lettersByLength,
                               Vector<Vector<bool>> &table) {

    size_t numberOfCodesSize = numberOfCodes.getSize();

    //Build code from first code
    for (size_t i = 1; i < numberOfCodesSize; ++i) {
        if (numberOfCodes[i]) {
            BYTE counter = firstCode[i];

            for (size_t j = 0; j < numberOfCodes[i]; ++j) {
                //Get first i bits in reversed order
                for (BYTE bit = 0; bit < i; ++bit) {
                    table[lettersByLength[i][j]].pushBack(counter & (static_cast<BYTE>(1) << bit));
                }

                ++counter;
            }
        }
    }
}


void Huffman::WriteFilesSizes(
                           const Vector<File> &files,
                                 std::fstream &out) {
#ifdef DEBUG
    assert(out.is_open());
#endif
    size_t filesNumb = files.getSize();
    Binary binary;

    if (binary.WriteBinary(out, filesNumb)) {
        std::cerr << "Too many files" << std::endl;
        exit(1);
    }

    for(size_t i = 0; i < filesNumb; i++) {
        if (binary.WriteBinary(out, files[i].GetSize())) {
            std::cerr << "The file "
                      << files[i].GetName()
                      << " is too big" << std::endl;
            exit(1);
        }
    }
}

void Huffman::WriteEncodingInfo(
                       const Vector<BYTE>          &numberOfCodes,
                       const Vector<Vector<BYTE >> &lettersByLength,
                       const Vector<File>          &files,
                             std::fstream          &out) {
#ifdef DEBUG
    assert(out.is_open());
#endif
    WriteFilesSizes(files, out);

    out.put(static_cast<char>(numberOfCodes.getSize() - 1));
    for (size_t i = 1; i < numberOfCodes.getSize(); ++i) {
        out.put(numberOfCodes[i]);
    }

    for (size_t i = 1; i < lettersByLength.getSize(); ++i) {
        for (size_t p = 0; p < numberOfCodes[i]; ++p)
            out.put(lettersByLength[i][p]);
    }
}

Vector<Vector<Huffman::BYTE>> Huffman::ReadDecodingInfo(
        Vector<BYTE>   &numberOfCodes,
        Vector<BYTE>   &letters,
        Vector<size_t> &sizes,
        std::fstream   &in) {

#ifdef DEBUG
    assert(in.is_open());
#endif

    Binary binary;
    size_t fileNumb = binary.ReadBinary(in);

    for(size_t i = 0; i < fileNumb; ++i) {
        sizes.pushBack(binary.ReadBinary(in));
    }


    auto maxLength = static_cast<BYTE>(in.get());
    for (BYTE i = 1; i <= maxLength; ++i) {
        numberOfCodes.pushBack(static_cast<BYTE>(in.get()));
    }

    Vector<Vector<BYTE >> lettersByLength(maxLength + 1);

    for (size_t l = 1; l < lettersByLength.getSize(); ++l) {
        for (BYTE i = 0; i != numberOfCodes[l]; ++i) {
            auto c = static_cast<BYTE>(in.get());
            lettersByLength[l].pushBack(c);
            letters.pushBack(c);
        }
    }

    return lettersByLength;
}


void Huffman::WriteEncoding(
        const Vector<Vector<bool>> &table,
              std::fstream         &in,
              std::fstream         &out) {
#ifdef DEBUG
    assert(in.is_open());
    assert(out.is_open());
#endif

    //Buffer size of byte to output the encoding sequence byte by byte
    char buf   = 0;
    //How many bits in the buffer are currently complete
    char count = 0;

    //For the whole file
    while (true) {

        auto c = static_cast<BYTE>(in.get());

        if(in.eof()) break;

        for(int i = table[c].getSize() - 1; i >= 0; --i) {
            buf |=  table[c][i] << (BYTE_SIZE - 1 - count);
            ++count;

            if (count == BYTE_SIZE)  {
                out.put(buf);
                buf = 0;
                count = 0;
            }
        }
    }

    //If there's last incomplete buffer
    if (count) {
        out.put(buf);
    }
}

void Huffman::WriteDecoding (
              std::fstream       &in,
              std::fstream       &out,
        const Tree<Letter>::Node *root,
              size_t             size) {
#ifdef DEBUG
    assert(root);
    assert(in.is_open());
    assert(out.is_open());
#endif

    if (size == 0) {
        return;
    }

    const Tree<Letter>::Node *cur = root;

    size_t count = 0;
    size_t fileSize = 0;

    auto byte = static_cast<BYTE >(in.get());
    while (!in.eof()) {
        bool b = static_cast<bool>(byte & (1 << (BYTE_SIZE- 1 - count)));
        if (b) {
            cur = cur->rightChild;
        }
        else {
            cur = cur->leftChild;
        }
        if (!cur->leftChild && !cur->rightChild) {
            out.put(cur->data.letter);
            ++fileSize;
            cur = root;
        }
        ++count;

        if (fileSize == size)
            break;

        if (count == BYTE_SIZE) {
            count = 0;
            byte = static_cast<BYTE>(in.get());
        }
    }
}

int Huffman::Binary::WriteBinary(
        std::fstream  &out,
        size_t        size) {
#ifdef DEBUG
    assert(out.is_open());
#endif
    if (size > ULONG_MAX) {
        std::cerr << "Error in func <WriteBinary>: var <size> max-value is "
                  << ULONG_MAX << std::endl;
        return 1;
    }

    for(int k = 3; k != -1; --k) {
        out.put(static_cast<char>((size >> (BYTE_SIZE * k)) % Huffman::BYTE_COUNT));
    }
    return 0;
}


size_t Huffman::Binary::ReadBinary(std::fstream &in){
#ifdef DEBUG
    assert(in.is_open());
#endif
    size_t num = 0;
    size_t c = 0;
    for(int k = 3; k != -1; --k) {
        c = static_cast<Huffman::BYTE>(in.get());
        num += c * (1 << (BYTE_SIZE * k));
    }
    return num;
}

